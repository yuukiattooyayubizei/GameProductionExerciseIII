#include"MapData.h"
#include<DxLib.h>

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


		if (room2.CollisionRoom(i) == true)
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
		TILE tile = GetTile(NextPos);
		if (tile == TILE_ROOM || tile == TILE_CORRIDOR || tile == TILE_CORRIDOR_ADJACENT_ROOM) {
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

	int size = (int)m_Room.size();
	int choiceCreateRoom;

	if (size <= 0) {
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
	//GetRand(Size.x) + Pos.x;だと部屋の外に飛び出る可能性があるため1だけ減らす
	//yの方も同様
	SetPos.x = GetRand(Size.x - 1) + Pos.x;
	SetPos.y = GetRand(Size.y - 1) + Pos.y;

	//廊下に隣接していたらやり直し
	if (GetTile(SetPos) == TILE_CORRIDOR_ADJACENT_ROOM)
		SetPos = GetRoomPos();

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
			if (m_Map[StartY + i][StartX + k] != TILE_CORRIDOR_ADJACENT_ROOM)
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

bool CMapData::InvestigationMapOutside(Int2 i) {
	if (i.y >= 0 && i.y < MAP_Y && i.x >= 0 && i.x < MAP_X) return false;

	return true;
}

bool CMapData::CollisionStairs(Int2 i) {

	if (GetStairsPos().x == i.x && GetStairsPos().y == i.y)
	{
		return true;
	}

	return false;
};

Int2 CMapData::GetNotHerePlayerRoomPos(Int2 PlPos)
{
	//部屋が1つしかないなら無限ループして落ちるからやめる
	if (m_Room.size() <= 1) {
		Int2 i = { -1,-1 };
		return i;
	}

	//ランダムで座標を取得
	Int2 ret = GetRoomPos();
	//プレイヤーと同じ部屋か、廊下と隣接しているなら再抽選
	while (GetRoomNum(ret) == GetRoomNum(PlPos) || GetTile(ret) == TILE_CORRIDOR_ADJACENT_ROOM)
	{
		ret = GetRoomPos();
	}
	return ret;
}

bool CMapData::IsAdjacentTile(Int2 pos, TILE tile) {

	//範囲外はWALLとみなす
	if (tile == TILE_WALL) {
		if (pos.x <= 0 || pos.x >= MAP_X || pos.y <= 0 || pos.y >= MAP_X) {
			return true;
		}
	}

	if (pos.x > 0) {
		pos.x--;
		if (GetTile(pos) == tile)
			return true;
		pos.x++;
	}
	if (pos.x < MAP_X) {
		pos.x++;
		if (GetTile(pos) == tile)
			return true;
		pos.x--;
	}
	if (pos.y > 0) {
		pos.y--;
		if (GetTile(pos) == tile)
			return true;
		pos.y++;
	}
	if (pos.y < MAP_Y) {
		pos.y++;
		if (GetTile(pos) == tile)
			return true;
		pos.y--;
	}

	return false;
}