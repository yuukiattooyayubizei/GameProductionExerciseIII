#include"MapCreate.h"
#include<iostream>
#include<algorithm>
#include<DxLib.h>

Int2 CMapCreate::DigCorridor(CMapData& mapData, Int2 start, Int2 end)
{
	Int2 startpos = start;
	Int2 ret = { -1,-1 };

	// 横方向に掘る
	while (start.x != end.x)
	{
		if (mapData.GetTile(start) != TILE_ROOM && mapData.GetTile(start) != TILE_CORRIDOR_ADJACENT_ROOM)
			mapData.SetTile(start, TILE_CORRIDOR);

		//目標が右か左か調べ、その方向に1マス移動
		if (start.x < end.x)
			start.x++;
		else
			start.x--;
	}

	// 縦方向に掘る
	while (start.y != end.y)
	{
		if (mapData.GetTile(start) != TILE_ROOM && mapData.GetTile(start) != TILE_CORRIDOR_ADJACENT_ROOM)
			mapData.SetTile(start, TILE_CORRIDOR);
		//目標が上か下か調べ、その方向に1マス移動
		if (start.y < end.y)
			start.y++;
		else
			start.y--;
	}

	// 最後のマス
	if (mapData.GetTile(start) != TILE_ROOM)
		mapData.SetTile(start, TILE_CORRIDOR);

	//廊下を作ったら、もう一度作った廊下を確認し、部屋のまま変わっていないかつ隣に廊下があるならそこを廊下に隣接した部屋マスに変える
	start = startpos;

	while (start.x != end.x)
	{
		if (mapData.GetTile(start) == TILE_ROOM && mapData.IsAdjacentTile(start, TILE_CORRIDOR) == true)
		{
			mapData.SetTile(start, TILE_CORRIDOR_ADJACENT_ROOM);
			ret = start;
		}

		//目標が右か左か調べ、その方向に1マス移動
		if (start.x < end.x)
			start.x++;
		else
			start.x--;
	}

	while (start.y != end.y)
	{
		if (mapData.GetTile(start) == TILE_ROOM && mapData.IsAdjacentTile(start, TILE_CORRIDOR) == true)
		{
			mapData.SetTile(start, TILE_CORRIDOR_ADJACENT_ROOM);
			ret = start;
		}

		//目標が上か下か調べ、その方向に1マス移動
		if (start.y < end.y)
			start.y++;
		else
			start.y--;
	}

	return ret;
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
	CreateCorridor(mapData);
	CreateStairs(mapData);
}

//廊下の作成
bool CMapCreate::CreateCorridor(CMapData& mapData)
{
	//部屋の数を取得
	int roomCount = static_cast<int>(mapData.GetRoomNum());

	//部屋の数が少なすぎたらやめる
	if (roomCount < ROOM_COUNT_MIN)
		return false;

	std::vector<RoomEdge> edges;


	for (int i = 0; i < roomCount; i++)
	{
		for (int k = i + 1; k < roomCount; k++)
		{
			//部屋の中心座標を取得
			Int2 RoomA{ ChangeInt2ToFloat2(mapData.GetRoom(i).GetCenter()) };
			Int2 RoomB{ ChangeInt2ToFloat2(mapData.GetRoom(k).GetCenter()) };
			Int2 distance{ SubInt2(RoomA,RoomB) };

			RoomEdge edge;
			edge.roomA = i;
			edge.roomB = k;
			//直線距離でどれくらい距離が離れているか計算
			edge.distance = distance.x * distance.x + distance.y * distance.y;

			edges.push_back(edge);
		}
	}

	//直線距離が短い順に並べ替える
	std::sort(edges.begin(), edges.end(),[](const RoomEdge& a, const RoomEdge& b){
			return a.distance < b.distance;
		});

	UnionFind uf(roomCount);

	int corridorCount = 0;

	//直線距離が短い順に計算
	for (const RoomEdge& edge : edges)
	{
		//2つがすでに同じグループだったら計算しない
		if (uf.Same(edge.roomA, edge.roomB))
			continue;

		const CRoom& roomA = mapData.GetRoom(edge.roomA);
		const CRoom& roomB = mapData.GetRoom(edge.roomB);

		Int2 centerA{ ChangeInt2ToFloat2(roomA.GetCenter()) };
		Int2 centerB{ ChangeInt2ToFloat2(roomB.GetCenter()) };

		//左右→上下の順に廊下を作るか上下→左右の順に廊下を作るかをランダムで決定
		if (GetRand(1) == 0)
		{
			Int2 mid{ centerB.x, centerA.y };

			Int2 GateA = DigCorridor(mapData,centerA, mid);
			Int2 GateB = DigCorridor(mapData,mid, centerB);

			RoomLink link;
			link.m_RoomA = edge.roomA;
			link.m_RoomB = edge.roomB;

			link.m_CenterA = centerA;
			link.m_CenterB = centerB;

			link.m_GateA = GateA;
			link.m_GateB = GateB;

			link.m_Bend = mid;

			// 接続情報を保存
			mapData.AddRoomLink(link);
		}
		else
		{
			Int2 mid{ centerB.x, centerA.y };

			Int2 GateA = DigCorridor(mapData,centerA, mid);
			Int2 GateB = DigCorridor(mapData,mid, centerB);

			RoomLink link;
			link.m_RoomA = edge.roomA;
			link.m_RoomB = edge.roomB;

			link.m_CenterA = centerA;
			link.m_CenterB = centerB;

			link.m_GateA = GateA;
			link.m_GateB = GateB;

			link.m_Bend = mid;

			link.m_Route = CreateCorridorRoute(link.m_GateA,link.m_Bend,link.m_GateB);

			// 接続情報を保存
			mapData.AddRoomLink(link);
		}

		//作り終わったらカウントを進める
		corridorCount++;

		//作成が終わったら、2つの部屋を結びつける
		uf.Unite(edge.roomA, edge.roomB);

		//規定数まで作ったら終わる
		if (corridorCount >= roomCount - 1)
			break;
	}

	return true;
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