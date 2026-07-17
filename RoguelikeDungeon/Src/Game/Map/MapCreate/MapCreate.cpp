#include"MapCreate.h"
#include<iostream>
#include<algorithm>
#include<DxLib.h>

UnionFind::UnionFind(int size)
{
	//要素数を部屋数と同じにする
	m_Parent.resize(size);

	//自分自身を親にする
	for (int i = 0; i < size; i++)
	{
		m_Parent[i] = i;
	}
}

//親を探す
int UnionFind::Find(int x)
{
	//親が自分自身なら自分自身を返す
	if (m_Parent[x] == x)
		return x;

	//親が自分自身じゃないなら、その親の親を探す
	//これを、自分自身が親のものが出るまで繰り返す
	//さらに、その自分自身が親のものが出た時にそれを保存して、次回以降の探索を短縮
	m_Parent[x] = Find(m_Parent[x]);
	return m_Parent[x];
}

//2つが同じグループかどうかを返す
bool UnionFind::Same(int a, int b)
{
	if (Find(a) == Find(b))return true;
	return false;
}

//2つを同じグループにする
//Aが親としてつなぐ
void UnionFind::Unite(int a, int b)
{
	int rootA = Find(a);
	int rootB = Find(b);

	//既に同じグループなら(親が同じなら)つながない
	if (rootA == rootB)
		return;

	m_Parent[rootB] = rootA;
}

void CMapCreate::DigCorridor(CMapData& mapData,const std::vector<Int2>& route){

	//routeの座標を一つづつ確認
	for (const Int2& pos : route)
	{
		TILE tile = mapData.GetTile(pos);

		// 部屋マスは廊下に書き換えない
		if (tile == TILE_ROOM ||tile == TILE_CORRIDOR_ADJACENT_ROOM)
		{
			continue;
		}

		mapData.SetTile(pos, TILE_CORRIDOR);
	}
}

void CMapCreate::CreateStairs(CMapData& mapData)
{
	//ランダムな部屋マスを取得
	Int2 pos = mapData.GetRoomPos();
	//エラーの場合-1が帰ってくる
	if (pos.x == -1)
		return;

	//階段を置く
	mapData.SetStairsPos(pos);

	std::cout << pos.x << "," << pos.y << "に階段を生成" << std::endl;
}

CRoom CMapCreate::RoomSizeDecision() {

	int X, Y, StartX, StartY, EndX, EndY;
	X = Y = StartX = StartY = EndX = EndY = 0;

	float CenterX, CenterY;
	CenterX = CenterY = 0.0f;

	//部屋の大きさをランダムで決定
	X = GetRand(MAP_SIZE_MAX - MAP_SIZE_MIN) + MAP_SIZE_MIN;
	Y = GetRand(MAP_SIZE_MAX - MAP_SIZE_MIN) + MAP_SIZE_MIN;

	//どこに生成するかを決定
	StartX = GetRand(MAP_X - X);
	StartY = GetRand(MAP_Y - Y);

	EndX = StartX + X;
	EndY = StartY + Y;

	//中心点を計算
	CenterX = (EndX + StartX) / 2.0f;
	CenterY = (EndY + StartY) / 2.0f;

	CRoom R;
	R.SetSize(X, Y);
	R.SetPos(StartX, StartY);
	R.SetCenter(CenterX, CenterY);

	return R;
}

bool CMapCreate::CreateRoom(CMapData& mapData,int CreateNum) {
	for (int index = 0;index < CreateNum;index++)
	{
		CRoom room{};
		bool CreateSuccess = false;


		for (int Retry = 0;Retry < RETRY_MAX;Retry++)
		{
			room = RoomSizeDecision();

			//ここで他の部屋との当たり判定を行い、衝突していたら作り直し
			if (mapData.CollisionRoomToRoom(room))
				continue;

			mapData.RoomSave(room);
			//ここまで来たら、部屋の作成は完了している
			CreateSuccess = true;
			break;
		}
		if (CreateSuccess == false)
			//ここに来たら、もう部屋の置き場がないと判断
			return false;
	}
	return true;
}

void CMapCreate::CreateFloor(CMapData& mapData) {
	//新しくマップを作り直すため、マップを消去
	mapData.Init();

	//5個から7個の部屋を作成
	if (CreateRoom(mapData,GetRand(ROOM_MAX - ROOM_MIN) + ROOM_MIN) == false)return;
	//廊下を作る
	CreateCorridor(mapData);
	mapData.CreateRoomGraph();

	//階段を作る
	CreateStairs(mapData);
}

//廊下の作成
bool CMapCreate::CreateCorridor(CMapData& mapData)
{
	//部屋の数を取得
	int roomCount =static_cast<int>(mapData.GetRoomNum());

	//部屋の数が少なすぎたらやめる
	if (roomCount < ROOM_COUNT_MIN)return false;

	std::vector<RoomEdge> edges;

	for (int i = 0; i < roomCount; i++)
	{
		for (int k = i + 1; k < roomCount; k++)
		{
			//部屋の中心座標を取得
			Int2 roomA{
				ChangeInt2ToFloat2(
					mapData.GetRoom(i).GetCenter()
				)
			};

			Int2 roomB{
				ChangeInt2ToFloat2(
					mapData.GetRoom(k).GetCenter()
				)
			};

			//内容を記録
			RoomEdge edge;

			edge.roomA = i;
			edge.roomB = k;

			//2部屋の距離を計算
			Int2 distance = SubInt2(roomA, roomB);
			edge.distance = distance.x * distance.x + distance.y * distance.y;

			edges.push_back(edge);
		}
	}

	//2部屋の距離が短い順に並べる
	std::sort(edges.begin(),edges.end(),[](const RoomEdge& a, const RoomEdge& b)
		{
			return a.distance < b.distance;
		}
	);

	UnionFind uf(roomCount);

	//実際に作った廊下の数
	int corridorCount = 0;

	for (const RoomEdge& edge : edges)
	{
		//すでに2部屋が行き来可能ならやめる
		if (uf.Same(edge.roomA, edge.roomB))
		{
			continue;
		}

		const CRoom& roomA =mapData.GetRoom(edge.roomA);
		const CRoom& roomB =mapData.GetRoom(edge.roomB);

		Int2 centerA = ChangeInt2ToFloat2(roomA.GetCenter());
		Int2 centerB = ChangeInt2ToFloat2(roomB.GetCenter());

		Int2 mid;

		// ランダムで横→縦か縦→横のどちらかに
		if (GetRand(1) == 0)
			// 横→縦
			mid = {centerB.x,centerA.y};
		else
			// 縦→横
			mid = {centerA.x,centerB.y};
		

		// centerAからcenterBまでの全経路を作る
		std::vector<Int2> fullRoute = CreateCorridorRoute(centerA,centerB,mid);

		// 経路の通りに掘る
		DigCorridor(mapData,fullRoute);

		RoomLink link{};

		link.m_RoomA = edge.roomA;
		link.m_RoomB = edge.roomB;

		link.m_CenterA = centerA;
		link.m_CenterB = centerB;

		link.m_Bend = mid;

		// 掘った後の経路からGateを調べる
		bool foundGate = FindCorridorGates(mapData, fullRoute, link.m_GateA, link.m_GateB, link.m_Route);

		mapData.AddRoomLink(link);

		corridorCount++;

		//2部屋をつなげる
		uf.Unite(edge.roomA,edge.roomB);

		//必要本数出来たらやめる
		if (corridorCount >= roomCount - 1)
		{
			break;
		}
	}

	return corridorCount == roomCount - 1;
}

void CMapCreate::AddLineToRoute(std::vector<Int2>& route,Int2 start,Int2 end,bool includeStart){

	Int2 pos = start;

	if (includeStart)
	{
		route.push_back(pos);
	}

	while (pos.x != end.x || pos.y != end.y)
	{
		if (pos.x < end.x)
		{
			pos.x++;
		}
		else if (pos.x > end.x)
		{
			pos.x--;
		}
		else if (pos.y < end.y)
		{
			pos.y++;
		}
		else if (pos.y > end.y)
		{
			pos.y--;
		}

		route.push_back(pos);
	}
}

std::vector<Int2> CMapCreate::CreateCorridorRoute(Int2 gateA, Int2 gateB, Int2 bend){

	std::vector<Int2> route;

	// gateAからbendまで
	AddLineToRoute(route, gateA, bend, true);

	// bendはすでに追加されているので、
	// 2本目では開始地点を追加しない
	AddLineToRoute(route, bend, gateB, false);

	return route;
}

bool CMapCreate::IsRoomLikeTile(TILE tile)
{
	if(tile == TILE_ROOM || tile == TILE_CORRIDOR_ADJACENT_ROOM)return true;

	return false;
}

bool CMapCreate::IsCorridorTile(TILE tile)
{
	if (tile == TILE_CORRIDOR)return true;

	return false;
}

bool CMapCreate::FindCorridorGates(CMapData& mapData,const std::vector<Int2>& fullRoute,Int2& gateA,Int2& gateB,std::vector<Int2>& gateRoute){

	//経路が短過ぎたらやめる
	if (fullRoute.size() < 2) return false;

	int gateAIndex = -1;
	int gateBIndex = -1;

	// centerA側からGateAを探す
	for (int i = 0;i + 1 < static_cast<int>(fullRoute.size());i++){

		//今と次のタイルを取得
		TILE currentTile = mapData.GetTile(fullRoute[i]);
		TILE nextTile = mapData.GetTile(fullRoute[i + 1]);

		//現在が部屋で一歩先が廊下ならそこが境界
		if (IsRoomLikeTile(currentTile) &&IsCorridorTile(nextTile)){
			gateAIndex = i;
			break;
		}
	}

	// centerB側からGateBを探す
	for (int i = static_cast<int>(fullRoute.size()) - 1; i - 1 >= 0; i--)
	{
		//今と前のタイルを取得
		TILE currentTile = mapData.GetTile(fullRoute[i]);
		TILE previousTile = mapData.GetTile(fullRoute[i - 1]);

		//現在が部屋で一歩後ろが廊下ならそこが境界
		if (IsRoomLikeTile(currentTile) && IsCorridorTile(previousTile)){
			gateBIndex = i;
			break;
		}
	}

	//境界が見つかっていないなら終わる
	if (gateAIndex < 0 || gateBIndex < 0 || gateAIndex >= gateBIndex) return false;
	
	//実際の座標を取得
	gateA = fullRoute[gateAIndex];
	gateB = fullRoute[gateBIndex];

	//そのタイルを、廊下に隣接した部屋マスに変える
	mapData.SetTile(gateA,TILE_CORRIDOR_ADJACENT_ROOM);
	mapData.SetTile(gateB,TILE_CORRIDOR_ADJACENT_ROOM);

	//敵のルートを今見つけたルートに書き換え
	gateRoute.assign(fullRoute.begin() + gateAIndex,fullRoute.begin() + gateBIndex + 1);

	return true;
}