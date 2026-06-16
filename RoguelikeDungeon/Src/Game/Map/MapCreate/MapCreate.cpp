#include"MapCreate.h"
#include<iostream>
#include<algorithm>
#include<DxLib.h>

static constexpr int ROOM_COUNT_MIN = 2;

void CMapCreate::DigCorridor(CMapData& mapData, Int2 a, Int2 b)
{
	int x = a.x;
	int y = a.y;

	// 横方向に掘る
	while (a.x != b.x)
	{
		if (mapData.GetTile(a) != TILE_ROOM)
			mapData.SetTile(a, TILE_CORRIDOR);
		//目標が右か左か調べ、その方向に1マス移動
		a.x += (b.x > x) ? 1 : -1;
	}

	// 縦方向に掘る
	while (a.y != b.y)
	{
		if (mapData.GetTile(a) != TILE_ROOM)
			mapData.SetTile(a, TILE_CORRIDOR);
		//目標が上か下か調べ、その方向に1マス移動
		a.y += (b.y > a.y) ? 1 : -1;
	}

	// 最後のマス
	if (mapData.GetTile(a) != TILE_ROOM)
		mapData.SetTile(a, TILE_CORRIDOR);
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
	//マップを消去
	mapData.Init();

	//3個から5個の部屋を作成
	if (CreateRoom(mapData,GetRand(ROOM_MAX - ROOM_MIN) + ROOM_MIN) == false)return;
	CreateCorridor(mapData);
	CreateStairs(mapData);
}

bool CMapCreate::CreateCorridor(CMapData& mapData)
{
	int roomCount = static_cast<int>(mapData.GetRoomNum());

	if (roomCount < ROOM_COUNT_MIN)
		return false;

	std::vector<RoomEdge> edges;


	for (int i = 0; i < roomCount; i++)
	{
		for (int k = i + 1; k < roomCount; k++)
		{
			int ax = static_cast<int>(mapData.GetRoom(i).GetCenter().x);
			int ay = static_cast<int>(mapData.GetRoom(i).GetCenter().y);

			int bx = static_cast<int>(mapData.GetRoom(k).GetCenter().x);
			int by = static_cast<int>(mapData.GetRoom(k).GetCenter().y);

			int dx = ax - bx;
			int dy = ay - by;

			RoomEdge edge;
			edge.roomA = i;
			edge.roomB = k;
			edge.distance = dx * dx + dy * dy;

			edges.push_back(edge);
		}
	}

	std::sort(edges.begin(), edges.end(),[](const RoomEdge& a, const RoomEdge& b){
			return a.distance < b.distance;
		});

	UnionFind uf(roomCount);

	int corridorCount = 0;

	for (const RoomEdge& edge : edges)
	{
		if (uf.Same(edge.roomA, edge.roomB))
			continue;

		uf.Unite(edge.roomA, edge.roomB);

		const CRoom& roomA = mapData.GetRoom(edge.roomA);
		const CRoom& roomB = mapData.GetRoom(edge.roomB);

		Int2 centerA{
			static_cast<int>(roomA.GetCenter().x),
			static_cast<int>(roomA.GetCenter().y)
		};

		Int2 centerB{
			static_cast<int>(roomB.GetCenter().x),
			static_cast<int>(roomB.GetCenter().y)
		};

		if (GetRand(1) == 0)
		{
			Int2 mid{ centerB.x, centerA.y };
			DigCorridor(mapData,centerA, mid);
			DigCorridor(mapData,mid, centerB);
		}
		else
		{
			Int2 mid{ centerA.x, centerB.y };
			DigCorridor(mapData,centerA, mid);
			DigCorridor(mapData,mid, centerB);
		}

		corridorCount++;

		if (corridorCount >= roomCount - 1)
			break;
	}

	return true;
}