#include"map.h"
#include"iostream"
#include <DxLib.h>
#include"cmath"

using namespace std;

CRoom::CRoom() {
	m_Center.x = 0;
	m_Center.y = 0;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Size.x = 0;
	m_Size.y = 0;
	m_CloseRoom = -1;
	m_IsConnectRoom = false;
}

void CMap::Init() {
	for (int i = 0;i < MAP_Y;i++)
	{
		for (int k = 0;k < MAP_X;k++)
		{
			//最初は壁で埋める
			m_Map[i][k] = TILE_WALL;
		}
	}
}

bool CMap::CreateRoom() {

	CRoom room{};

	for (int Retry = 0;Retry < RETRY_MAX;Retry++)
	{
		room = RoomSizeDecision();

		//ここで他の部屋との当たり判定を行い、衝突していたら作り直し
		if (CollisionRoom(room))
			continue;

		RoomSave(room);
		//ここまで来たら、部屋の作成は完了している
		return true;
	}
	//ここに来たら、もう部屋の置き場がないと判断
	return false;
}

CRoom CMap::RoomSizeDecision() {

	int X, Y, StartX, StartY,  EndX, EndY;
	X = Y = StartX = StartY =  EndX = EndY =  0;

	float CenterX, CenterY;
	CenterX = CenterY = 0.0f;

	//部屋の大きさをランダムで決定
	//縦も横も3~7マスの間になる
	//同時に部屋の隣の壁の判別も行う

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
	R.SetSize(X,Y);
	R.SetPos(StartX, StartY);
	R.SetCenter(CenterX, CenterY);

	return R;
}

void CMap::RoomSave (const CRoom& room) {
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
			//端の部分は部屋に隣接する壁とする(通常の壁と機能は変わらず、部屋どうしが隣接しないために使う)
			if (i == 0 || i == Y - 1 || k == 0 || k == X - 1)
				m_Map[StartY + i][StartX + k] = TILE_WALL_EDGE;
			else
				m_Map[StartY + i][StartX + k] = TILE_ROOM;
		}
	}
}


bool CMap::CollisionRoom(const CRoom& room) {
	for (std::vector<CRoom>::iterator ite = m_Room.begin(); ite != m_Room.end();ite++)
	{
		CRoom& room2 = *ite;

		//座標を指定
		int room1Left = room.GetPos().x;
		int room1Right = room.GetPos().x + room.GetSize().x - 1;
		int room1Up = room.GetPos().y;
		int room1Down = room.GetPos().y + room.GetSize().y - 1;

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

void CMap::DigCorridor(Int2 a, Int2 b)
{
	int x = a.x;
	int y = a.y;

	// 横方向に掘る
	while (x != b.x)
	{
		if (m_Map[y][x] != TILE_ROOM)
			m_Map[y][x] = TILE_CORRIDOR;
		x += (b.x > x) ? 1 : -1;
	}

	// 縦方向に掘る
	while (y != b.y)
	{
		if (m_Map[y][x] != TILE_ROOM)
			m_Map[y][x] = TILE_CORRIDOR;
		y += (b.y > y) ? 1 : -1;
	}

	// 最後のマス
	if (m_Map[y][x] != TILE_ROOM)
		m_Map[y][x] = TILE_CORRIDOR;
}

//廊下の生成
bool CMap::CreateCorridor()
{

	if (m_Room.size() < 2) return false;

	for (size_t i = 0; i < m_Room.size() - 1; i++)
	{
		const CRoom& roomA = m_Room[i];
		const CRoom& roomB = m_Room[i + 1];

		Int2 centerA{
			(int)roomA.GetCenter().x,
			(int)roomA.GetCenter().y
		};

		Int2 centerB{
			(int)roomB.GetCenter().x,
			(int)roomB.GetCenter().y
		};

		// ランダムで掘る順番を変える
		if (GetRand(1) == 0)
		{
			// 横 → 縦
			Int2 mid{ centerB.x, centerA.y };
			DigCorridor(centerA, mid);
			DigCorridor(mid, centerB);
		}
		else
		{
			// 縦 → 横
			Int2 mid{ centerA.x, centerB.y };
			DigCorridor(centerA, mid);
			DigCorridor(mid, centerB);
		}
	}

	return true;

}

void CMap::Draw() {
	for (int i = 0;i < MAP_Y;i++)
	{
		for (int k = 0;k < MAP_X;k++)
		{
			switch (m_Map[i][k])
			{
			case TILE_WALL:
				//"壁"だと遠くから見て違いがわからず、■だと文字の大きさの関係でマップが崩れてしまうため、パッと見で■に見える口を採用
				cout << "口";
				break;
			case TILE_WALL_EDGE:
				cout << "隣";
				break;
			case TILE_ROOM:
				cout << "部";
				break;
			case TILE_CORRIDOR:
				cout << "廊";
				break;
			case TILE_STAIRS:
				cout << "階";
				break;
			default:
				break;
			}
		}
		cout << endl;
	}
}

SpecifiedRoomInformation CMap::SpecifiedRoom(const CRoom& room)
{
	//最初は何があってもこれより距離が短くなる値を入れる
	float MinDistance = 10000;
	int MinNum = -1;
	int num = 0;
	float DistanceX = 0, DistanceY = 0, Distance = 0, BestDistanceX = 0, BestDistanceY = 0;
	//それぞれの部屋との距離を全て調べる
	for (std::vector<CRoom>::iterator ite = m_Room.begin(); ite != m_Room.end();ite++,num++)
	{
		CRoom& room2 = *ite;

		//調べる部屋の座標が一致していたらそれは同じ部屋を調べているから調べない
		if (room2.GetCenter().x == room.GetCenter().x && room2.GetCenter().y == room.GetCenter().y)continue;

		//すでに廊下がある部屋は、そこにつなげるとつながらない部屋がでる可能性があるため調べない
		if (room2.GetConnectRoom() == true)continue;

		//マイナスの値になる可能性もあるが、このあと2乗するので問題ない
		DistanceX = room2.GetCenter().x - room.GetCenter().x;
		DistanceY = room2.GetCenter().y - room.GetCenter().y;

		Distance =  sqrt(DistanceX * DistanceX + DistanceY * DistanceY);

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
	ret.m_DistanceX = DistanceX;
	ret.m_DistanceY = DistanceY;

	//Xがマイナスなら東にあり、Yがマイナスなら北にある
	if (DistanceX >= 0)
	{
		//南西にある
		if (DistanceY >= 0)
		{
			if (DistanceY >= DistanceX)
				ret.m_Direction = DIRECTION_DOWN;
			else
				ret.m_Direction = DIRECTION_RIGHT;
		}
		//北西にある
		else
		{
			if (abs(DistanceY) >= DistanceX)
				ret.m_Direction = DIRECTION_UP;
			else
				ret.m_Direction = DIRECTION_RIGHT;
		}
	}
	else
	{
		//南東にある
		if (DistanceY >= 0)
		{
			if (abs(DistanceY) >= DistanceX)
				ret.m_Direction = DIRECTION_DOWN;
			else
				ret.m_Direction = DIRECTION_LEFT;
		}
		//北東にある
		else
		{
			if (abs(DistanceY) >= abs(DistanceX))
				ret.m_Direction = DIRECTION_UP;
			else
				ret.m_Direction = DIRECTION_LEFT;
		}
	}


	return ret;
}

CriateCorridor CMap::ConnectHallwayToRoom(const CRoom& room, SpecifiedRoomInformation close)
{
	Int2 StartPos{};
	CriateCorridor i{};
	for (int Retry = 0;Retry < RETRY_MAX;Retry++)
	{
		switch (close.m_Direction)
		{
		case 0:
			//部屋の北に作成
			StartPos.y = room.GetPos().y;
			//どのマスにするかはランダム
			//roomの端は壁なので端にならないように調整
			StartPos.x = GetRand(room.GetSize().x) + room.GetPos().x;
			break;
		case 1:
			//部屋の西に作成
			//どのマスにするかはランダム
			StartPos.y = GetRand(room.GetSize().y) + room.GetPos().y;
			StartPos.x = room.GetPos().x;
			break;
		case 2:
			//部屋の南に作成
			StartPos.y = room.GetPos().y + room.GetSize().y - 1;
			//どのマスにするかはランダム
			StartPos.x = GetRand(room.GetSize().x) + room.GetPos().x;
			break;
		case 3:
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

void CMap::CreateStairs()
{
	//どの部屋に階段を置くかランダムで決定

	int size = m_Room.size();
	int choiceCreateRoom;

	if (size <= 1)return;
	choiceCreateRoom = GetRand(size - 1);

	//階段を億部屋の位置と大きさを取得
	Int2 Pos = m_Room.at(choiceCreateRoom).GetPos();
	Int2 Size = m_Room.at(choiceCreateRoom).GetSize();

	//どこに置くかを決定
	Int2 SetPos;
	//部屋の端には置けないようにする
	SetPos.x = GetRand(Size.x - 2) + 1 + Pos.x;
	SetPos.y = GetRand(Size.y - 2) + 1 + Pos.y;

	//階段を置く
	m_Map[SetPos.y][SetPos.x] = TILE_STAIRS;
}