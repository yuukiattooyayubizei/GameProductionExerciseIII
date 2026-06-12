#include"MapData.h"
#include <DxLib.h>
#include <random>

using namespace std;

void CMapData::Init() {
	//部屋情報を消去
	m_Room.clear();

	for (int i = 0;i < MAP_Y;i++)
	{
		for (int k = 0;k < MAP_X;k++)
		{
			//最初は壁で埋める
			m_Map[i][k] = TILE_WALL;
		}
	}
}

void CMapData::Exit() {
	//部屋情報を消去
	m_Room.clear();

	for (int i = 0;i < MAP_Y;i++)
	{
		for (int k = 0;k < MAP_X;k++)
		{
			//最初は壁で埋める
			m_Map[i][k] = TILE_WALL;
		}
	}
}
void CMapData::Draw() {

}

TILE CMapData::GetTile(Int2 i) {
	//配列外にアクセスしようとしていたら
	if (i.x < 0 || i.x >= MAP_X || i.y < 0 || i.y >= MAP_Y) {
		//無を返す
		return TILE_NON;
	}
	return m_Map[i.y][i.x];
}

CRoom CMapData::GetStartRoom() {
	//もしRoomが存在しないなら初期化されたRoomで返す
	if (m_Room.empty()) {
		return CRoom{};
	}
	//最初ののRoomで返す
	return m_Room.front();
}

int CMapData::GetRoomNum(Int2 i) {

	int num = 0;

	for (std::vector<CRoom>::iterator ite = m_Room.begin(); ite != m_Room.end();ite++, num++)
	{
		CRoom& room2 = *ite;


		if (room2.CollsionRoom(i) == true)
		{
			return num;
		}

	}
	return -1;
}

int CMapData::GetFieldOfVision(Int2 i, DIRECTION dir) {
	int num = 0;
	Int2 NextPos = i;
	while (true)
	{
		switch (dir)
		{
		case DIRECTION_NON:
			return -1;
		case DIRECTION_UP:
			NextPos.y--;
			break;
		case DIRECTION_LEFT:
			NextPos.x--;
			break;
		case DIRECTION_DOWN:
			NextPos.y++;
			break;
		case DIRECTION_RIGHT:
			NextPos.x++;
			break;
		default:
			return -1;
		}

		//NextPosがマップ外かチェック
		if (InvestigationMapOutside(NextPos) == true)return num;

		//マップ内ならその座標が部屋もしくは廊下かチェック
		if (GetTile(NextPos) == TILE_ROOM || GetTile(NextPos) == TILE_CORRIDOR) {
			//部屋か廊下なら次に進む
			num++;
		}
		else
			//違うなら終了
			return num;
	}
}

Int2 CMapData::GetRoomPos()
{
	//どの部屋に階段を置くかランダムで決定

	int size = m_Room.size();
	int choiceCreateRoom;

	if (size <= 1) {
		Int2 p{};
		p.x = -1;
		p.y = -1;
		return p;
	}
	choiceCreateRoom = GetRand(size - 1);

	//階段を億部屋の位置と大きさを取得
	Int2 Pos = m_Room.at(choiceCreateRoom).GetPos();
	Int2 Size = m_Room.at(choiceCreateRoom).GetSize();

	//どこに置くかを決定
	Int2 SetPos;
	//部屋の端には置けないようにする
	//GetRand(Size.x) + Pos.x;だと部屋の外に飛び出る可能性があるため1だけ減らす
	//yの方も同様
	SetPos.x = GetRand(Size.x - 1) + Pos.x;
	SetPos.y = GetRand(Size.y - 1) + Pos.y;

	return SetPos;
}

void CMapData::RoomSave(const CRoom& room) {
	//他の部屋と衝突していないので現在の作成した部屋の情報を保存
	m_Room.push_back(room);

	int X = room.GetSize().x;
	int Y = room.GetSize().y;
	int StartX = room.GetPos().x;
	int StartY = room.GetPos().y;

	//部屋を生成
	for (int i = 0;i < Y;i++)
	{
		for (int k = 0;k < X;k++)
		{
			m_Map[StartY + i][StartX + k] = TILE_ROOM;
		}
	}
}

bool CMapData::CollisionRoomToRoom(const CRoom& room) {
	for (std::vector<CRoom>::iterator ite = m_Room.begin(); ite != m_Room.end();ite++)
	{
		CRoom& room2 = *ite;

		//座標を指定
		int room1Left = room.GetPos().x - ROOM_MARGIN;
		int room1Right = room.GetPos().x + room.GetSize().x - 1 + ROOM_MARGIN;
		int room1Up = room.GetPos().y - ROOM_MARGIN;
		int room1Down = room.GetPos().y + room.GetSize().y - 1 + ROOM_MARGIN;

		int room2Left = room2.GetPos().x;
		int room2Right = room2.GetPos().x + room2.GetSize().x - 1;
		int room2Up = room2.GetPos().y;
		int room2Down = room2.GetPos().y + room2.GetSize().y - 1;

		//部屋どうしが衝突しているかの判定
		if (room1Left <= room2Right &&
			room2Left <= room1Right &&
			room1Up <= room2Down &&
			room2Up <= room1Down) return true;
	}
	return false;
}

void CMapData::DigCorridor(Int2 a, Int2 b)
{
	int x = a.x;
	int y = a.y;

	// 横方向に掘る
	while (x != b.x)
	{
		if (m_Map[y][x] != TILE_ROOM)
			m_Map[y][x] = TILE_CORRIDOR;
		//目標が右か左か調べ、その方向に1マス移動
		x += (b.x > x) ? 1 : -1;
	}

	// 縦方向に掘る
	while (y != b.y)
	{
		if (m_Map[y][x] != TILE_ROOM)
			m_Map[y][x] = TILE_CORRIDOR;
		//目標が上か下か調べ、その方向に1マス移動
		y += (b.y > y) ? 1 : -1;
	}

	// 最後のマス
	if (m_Map[y][x] != TILE_ROOM)
		m_Map[y][x] = TILE_CORRIDOR;
}

bool CMapData::InvestigationMapOutside(Int2 i) {
	if (i.y >= 0 && i.y < MAP_Y && i.x >= 0 && i.x < MAP_X) return false;

	return true;
}

bool CMapData::CreateCorridor()
{
	if (m_Room.size() < 2)
		return false;

	std::vector<RoomEdge> edges;

	int roomCount = static_cast<int>(m_Room.size());

	for (int i = 0; i < roomCount; i++)
	{
		for (int k = i + 1; k < roomCount; k++)
		{
			int ax = static_cast<int>(m_Room[i].GetCenter().x);
			int ay = static_cast<int>(m_Room[i].GetCenter().y);

			int bx = static_cast<int>(m_Room[k].GetCenter().x);
			int by = static_cast<int>(m_Room[k].GetCenter().y);

			int dx = ax - bx;
			int dy = ay - by;

			RoomEdge edge;
			edge.roomA = i;
			edge.roomB = k;
			edge.distance = dx * dx + dy * dy;

			edges.push_back(edge);
		}
	}

	std::sort(edges.begin(), edges.end(),
		[](const RoomEdge& a, const RoomEdge& b)
		{
			return a.distance < b.distance;
		});

	UnionFind uf(roomCount);

	int corridorCount = 0;

	for (const RoomEdge& edge : edges)
	{
		if (uf.Same(edge.roomA, edge.roomB))
			continue;

		uf.Unite(edge.roomA, edge.roomB);

		const CRoom& roomA = m_Room[edge.roomA];
		const CRoom& roomB = m_Room[edge.roomB];

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
			DigCorridor(centerA, mid);
			DigCorridor(mid, centerB);
		}
		else
		{
			Int2 mid{ centerA.x, centerB.y };
			DigCorridor(centerA, mid);
			DigCorridor(mid, centerB);
		}

		corridorCount++;

		if (corridorCount >= roomCount - 1)
			break;
	}

	return true;
}

CorridorInfo CMapData::ConnectHallwayToRoom(const CRoom& room, SpecifiedRoomInformation close)
{
	Int2 StartPos{};
	CorridorInfo i{};
	for (int Retry = 0;Retry < RETRY_MAX;Retry++)
	{
		switch (close.m_Direction)
		{
		case DIRECTION_UP:
			//部屋の北に作成
			StartPos.y = room.GetPos().y;
			//どのマスにするかはランダム
			//roomの端は壁なので端にならないように調整
			StartPos.x = GetRand(room.GetSize().x) + room.GetPos().x;
			break;
		case DIRECTION_LEFT:
			//部屋の西に作成
			//どのマスにするかはランダム
			StartPos.y = GetRand(room.GetSize().y) + room.GetPos().y;
			StartPos.x = room.GetPos().x;
			break;
		case DIRECTION_DOWN:
			//部屋の南に作成
			StartPos.y = room.GetPos().y + room.GetSize().y - 1;
			//どのマスにするかはランダム
			StartPos.x = GetRand(room.GetSize().x) + room.GetPos().x;
			break;
		case DIRECTION_RIGHT:
			//部屋の東に作成
			//どのマスにするかはランダム
			StartPos.y = GetRand(room.GetSize().y) + room.GetPos().y;
			StartPos.x = room.GetPos().x + room.GetSize().x - 1;
			break;
		default:
			break;
		}

		//部屋マスが隣接していたら返す
		if (StartPos.y < MAP_Y - 1)
		{
			if (m_Map[StartPos.y + 1][StartPos.x] == TILE_ROOM)
			{
				i.StratPos = StartPos;
				i.MovePos.x = close.m_DistanceX;
				i.MovePos.y = close.m_DistanceY;
				return i;
			}
		}
		if (StartPos.y > 0)
		{
			if (m_Map[StartPos.y - 1][StartPos.x] == TILE_ROOM)
			{
				i.StratPos = StartPos;
				i.MovePos.x = close.m_DistanceX;
				i.MovePos.y = close.m_DistanceY;
				return i;
			}
		}
		if (StartPos.x < MAP_X - 1)
		{
			if (m_Map[StartPos.y][StartPos.x + 1] == TILE_ROOM)
			{
				i.StratPos = StartPos;
				i.MovePos.x = close.m_DistanceX;
				i.MovePos.y = close.m_DistanceY;
				return i;
			}
		}
		if (StartPos.x > 0)
		{
			if (m_Map[StartPos.y][StartPos.x - 1] == TILE_ROOM)
			{
				i.StratPos = StartPos;
				i.MovePos.x = close.m_DistanceX;
				i.MovePos.y = close.m_DistanceY;
				return i;
			}
		}
	}

	Int2 j;
	j.x = 0;
	j.y = 0;
	Int2 k;
	k.x = 0;
	k.y = 0;
	i.MovePos = j;
	i.StratPos = k;
	return i;
}

SpecifiedRoomInformation CMapData::SpecifiedRoom(const CRoom& room)
{
	//最初は何があってもこれより距離が短くなる値を入れる
	//マップの端から端までの長さが最も長くなる
	float MinDistance = sqrt(MAP_X * MAP_X + MAP_Y * MAP_Y);
	int MinNum = -1;
	int num = 0;
	float DistanceX = 0, DistanceY = 0, Distance = 0, BestDistanceX = 0, BestDistanceY = 0;
	//それぞれの部屋との距離を全て調べる
	for (std::vector<CRoom>::iterator ite = m_Room.begin(); ite != m_Room.end();ite++, num++)
	{
		CRoom& room2 = *ite;

		//調べる部屋の座標が一致していたらそれは同じ部屋を調べているから調べない
		if (room2.GetCenter().x == room.GetCenter().x && room2.GetCenter().y == room.GetCenter().y)continue;

		//すでに廊下がある部屋は、そこにつなげるとつながらない部屋がでる可能性があるため調べない
		if (room2.GetConnectRoom() == true)continue;

		//マイナスの値になる可能性もあるが、このあと2乗するので問題ない
		DistanceX = room2.GetCenter().x - room.GetCenter().x;
		DistanceY = room2.GetCenter().y - room.GetCenter().y;

		Distance = sqrt(DistanceX * DistanceX + DistanceY * DistanceY);

		//今までで最短が見つかったら最短の所を変更
		if (MinDistance > Distance)
		{
			MinDistance = Distance;
			MinNum = num;
			BestDistanceX = DistanceX;
			BestDistanceY = DistanceY;
		}

	}

	SpecifiedRoomInformation ret;

	ret.m_CloseRoomID = MinNum;
	ret.m_DistanceX = BestDistanceX;
	ret.m_DistanceY = BestDistanceY;

	//Xがマイナスなら東にあり、Yがマイナスなら北にある
	if (BestDistanceX >= 0)
	{
		//南西にある
		if (BestDistanceY >= 0)
		{
			if (BestDistanceY >= BestDistanceX)
				ret.m_Direction = DIRECTION_DOWN;
			else
				ret.m_Direction = DIRECTION_RIGHT;
		}
		//北西にある
		else
		{
			if (abs(BestDistanceY) >= BestDistanceX)
				ret.m_Direction = DIRECTION_UP;
			else
				ret.m_Direction = DIRECTION_RIGHT;
		}
	}
	else
	{
		//南東にある
		if (BestDistanceY >= 0)
		{
			if (abs(BestDistanceY) >= BestDistanceX)
				ret.m_Direction = DIRECTION_DOWN;
			else
				ret.m_Direction = DIRECTION_LEFT;
		}
		//北東にある
		else
		{
			if (abs(BestDistanceY) >= abs(BestDistanceX))
				ret.m_Direction = DIRECTION_UP;
			else
				ret.m_Direction = DIRECTION_LEFT;
		}
	}


	return ret;
}

bool CMapData::CollisionStairs(Int2 i) {

	if (GetStairsPos().x == i.x && GetStairsPos().y == i.y)
	{
		return true;
	}

	return false;
};
