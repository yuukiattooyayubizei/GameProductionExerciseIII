#include"map.h"
#include"iostream"
#include <DxLib.h>
#include"cmath"
#include <algorithm>
#include <random>

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
	//部屋情報を消去
	m_Room.clear();
	m_Item.clear();

	m_StairsPos = {};

	for (int i = 0;i < MAP_Y;i++)
	{
		for (int k = 0;k < MAP_X;k++)
		{
			//最初は壁で埋める
			m_Map[i][k] = TILE_WALL;
		}
	}
}

bool CMap::CreateRoom(int CreateNum) {
	for (int index = 0;index < CreateNum;index++)
	{
		CRoom room{};

		for (int Retry = 0;Retry < RETRY_MAX;Retry++)
		{
			room = RoomSizeDecision();

			//ここで他の部屋との当たり判定を行い、衝突していたら作り直し
			if (CollisionRoom(room))
				continue;

			RoomSave(room);
			//ここまで来たら、部屋の作成は完了している
			break;
		}
		////ここに来たら、もう部屋の置き場がないと判断
		//return false;
	}
	return true;
}

CRoom CMap::RoomSizeDecision() {

	int X, Y, StartX, StartY,  EndX, EndY;
	X = Y = StartX = StartY =  EndX = EndY =  0;

	float CenterX, CenterY;
	CenterX = CenterY = 0.0f;

	//部屋の大きさをランダムで決定
	//大きさは5~9マスだが、端の2マスは壁と同じなため、
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
			////端の部分は部屋に隣接する壁とする(通常の壁と機能は変わらず、部屋どうしが隣接しないために使う)
			//if (i == 0 || i == Y - 1 || k == 0 || k == X - 1)
			//	m_Map[StartY + i][StartX + k] = TILE_WALL_EDGE;
			//else
				m_Map[StartY + i][StartX + k] = TILE_ROOM;
		}
	}
}


bool CMap::CollisionRoom(const CRoom& room) {
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

bool CMap::IsItemExist(int x, int y){
		for (const FieldItem& fieldItem : m_Item)
		{
			if (fieldItem.pos.x == x && fieldItem.pos.y == y)
			{
				return true;
			}
		}
		return false;
	};

bool CMap::CollisionItem(FieldItem& item){

	// まず、現在の座標にアイテムがなければそのまま置ける
	if (!IsItemExist(item.pos.x, item.pos.y))
	{
		return true;
	}

	//アイテムの位置が被っていた場合1マス移動させる
	// 優先順位
	// ⑤③⑥
	// ①〇②
	// ⑦④⑧
	const Int2 movePos[] =
	{
		{-1,  0},	//左
		{ 1,  0},	//右
		{ 0, -1},	//上
		{ 0,  1},	//下
		{-1, -1},	//左上
		{ 1, -1},	//右上
		{-1,  1},	//左下
		{ 1,  1},	//右下
	};

	for (const Int2& move : movePos)
	{
		//調べる座標を決定
		int nextX = item.pos.x + move.x;
		int nextY = item.pos.y + move.y;

		//マップの範囲外は置けない
		if (nextX < 0 || nextX >= MAP_X ||
			nextY < 0 || nextY >= MAP_Y)
		{
			continue;
		}

		//部屋以外のマスには置けない
		if (m_Map[nextY][nextX] != TILE_ROOM)
		{
			continue;
		}

		//そこにもアイテムが落ちていたら置けない
		if (IsItemExist(nextX, nextY))
		{
			continue;
		}

		//ここまで来たらそのマスには置けるので移動
		item.pos.x = nextX;
		item.pos.y = nextY;

		return true;
	}

	//周囲8マスすべて置けなかったらfalseを返す
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

bool CMap::CreateCorridor()
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

void CMap::Draw(int x, int y) {

	for (int i = 0;i < MAP_Y;i++)
	{
		for (int k = 0;k < MAP_X;k++)
		{
			int centerX = 8 + 16 * k;
			int centerY = 8 + 16 * i;


			switch (m_Map[i][k])
			{
			case TILE_WALL:
				DrawBox(centerX + 8, centerY + 8, centerX - 8, centerY - 8, GetColor(0, 0, 0), TRUE);
				break;
			case TILE_ROOM:
				DrawBox(centerX + 8, centerY + 8, centerX - 8, centerY - 8, GetColor(0, 0, 255), TRUE);
				break;
			case TILE_CORRIDOR:
				DrawBox(centerX + 8, centerY + 8, centerX - 8, centerY - 8, GetColor(0, 255, 0), TRUE);
				break;
			default:
				break;
			}
		}
	}
	//階段の描画
	int centerX = 8 + 16 * m_StairsPos.x;
	int centerY = 8 + 16 * m_StairsPos.y;
	DrawBox(centerX + 8, centerY + 8, centerX - 8, centerY - 8, GetColor(255, 0, 0), TRUE);


	for_each(m_Item.begin(), m_Item.end(), [this](FieldItem item) {
		//床落ちアイテムの座標を取得
		int X = 8 + item.pos.x * 16;
		int Y = 8 + item.pos.y * 16;
		//種類によって色を変えておく
		switch (item.item.type)
		{
		case ITEM_1:
			DrawBox(X + 4, Y + 4, X - 4, Y - 4, GetColor(255, 0, 0), TRUE);
			break;
		case ITEM_2:
			DrawBox(X + 4, Y + 4, X - 4, Y - 4, GetColor(0, 255, 255), TRUE);
			break;
		case ITEM_3:
			DrawBox(X + 4, Y + 4, X - 4, Y - 4, GetColor(255, 0, 255), TRUE);
			break;
		case ITEM_4:
			DrawBox(X + 4, Y + 4, X - 4, Y - 4, GetColor(255, 255, 0), TRUE);
			break;
		default:
			DrawBox(X + 4, Y + 4, X - 4, Y - 4, GetColor(128, 128, 128), TRUE);
			break;
		}
		
	});


	//プレイヤーの描画
	centerX = 8 + x * 16;
	centerY = 8 + y * 16;
	DrawBox(centerX + 4, centerY + 4, centerX - 4, centerY - 4, GetColor(255, 255, 255), TRUE);
						
}

SpecifiedRoomInformation CMap::SpecifiedRoom(const CRoom& room)
{
	//最初は何があってもこれより距離が短くなる値を入れる
	//マップの端から端までの長さが最も長くなる
	float MinDistance = sqrt(MAP_X * MAP_X + MAP_Y * MAP_Y);
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

CorridorInfo CMap::ConnectHallwayToRoom(const CRoom& room, SpecifiedRoomInformation close)
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

void CMap::CreateStairs()
{
	//ランダムな部屋マスを取得
	Int2 pos = GetRoomPos();
	//エラーの場合-1が帰ってくる
	if (pos.x == -1)
		return;

	//階段を置く
	m_StairsPos.x = pos.x;
	m_StairsPos.y = pos.y;
}


void CMap::CreateItem(int CreateNum)
{
	for (int index = 0;index < CreateNum;index++)
	{
		//ランダムな部屋マスを取得
		Int2 pos = GetRoomPos();
		//エラーの場合-1が帰ってくる
		if (pos.x == -1)
			return;

		FieldItem item{};

		//座標を入力
		item.pos.x = pos.x;
		item.pos.y = pos.y;
		//アイテムの種類をランダムで決定
		int i = GetRand(ITEM_NUM - 1);
		item.item.type = static_cast<ITEM_TYPE>(i);

		// 置けなかった場合は追加しない
		if (!CollisionItem(item))
			continue;

		item.item.type = static_cast<ITEM_TYPE>(i);

		m_Item.push_back(item);
	}
}

void CMap::DeleteAll() {
	//やることはInitと変わらない
	Init();
}

CRoom CMap::GetStartRoom() {
	//もしRoomが存在しないなら初期化されたRoomで返す
	if (m_Room.empty()) {
		return CRoom{};
	}
	//最初ののRoomで返す
	return m_Room.front();
}

TILE CMap::GetTile(int x, int y) {
	//配列外にアクセスしようとしていたら
	if (x < 0 || x >= MAP_X || y < 0 || y >= MAP_Y) {
		//無を返す
		return TILE_NON;
	}
	return m_Map[y][x];
}

Int2 CMap::GetRoomPos()
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