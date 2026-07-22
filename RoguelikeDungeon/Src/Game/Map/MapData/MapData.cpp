#include"MapData.h"
#include<DxLib.h>
#include <queue>

using namespace std;

void CMapData::Init() {
	//部屋情報を消去
	m_Room.clear();
	// 廊下の接続情報を削除
	m_RoomLinks.clear();

	// 部屋グラフを削除
	m_RoomGraph.clear();

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
	// 廊下の接続情報を削除
	m_RoomLinks.clear();

	// 部屋グラフを削除
	m_RoomGraph.clear();

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

void CMapData::AddLineToRoute(std::vector<Int2>& route,Int2 start,Int2 end,bool includeStart){
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

//各部屋の距離を計算
//const std::vector<std::vector<int>>& graphはそれぞれの部屋がどの部屋と直通しているか
//graphがgraph[0] = { 1, 3 };graph[1] = { 0,2 };graph[2] = { 1 };graph[3] = { 0 };
//の場合0と1、0と3、1と2が繋がっている
std::vector<int> CMapData::CalcRoomDistance(int startRoom) const
{

		//全部屋を-1で初期化
		std::vector<int> distance(m_RoomGraph.size(), -1);
		//キュー
		std::queue<int> RoomQueue;

		//最初の部屋を0にする
		distance[startRoom] = 0;
		//肺所の部屋を入れる
		RoomQueue.push(startRoom);

		while (!RoomQueue.empty())
		{
			//最初の部屋を見て消す
			int currentRoom = RoomQueue.front();
			RoomQueue.pop();

			//その部屋と直通する部屋数だけ回す
			for (int i = 0; i < m_RoomGraph[currentRoom].size(); ++i)
			{
				int nextRoom = m_RoomGraph[currentRoom][i];
				//すでに値が入っていたらなにもしない
				if (distance[nextRoom] != -1) { continue; }

				//前の部屋より1大きい値を入れる
				distance[nextRoom] = distance[currentRoom] + 1;
				//次の部屋を入れる
				RoomQueue.push(nextRoom);
			}
		}

		return distance;
}

int CMapData::GetRoomID(Int2 i) {

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
	while (GetRoomID(ret) == GetRoomID(PlPos) || GetTile(ret) == TILE_CORRIDOR_ADJACENT_ROOM)
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

//部屋どうしの接続を登録
void CMapData::CreateRoomGraph()
{
	// 部屋数と同じ数だけ隣接リストを作る
	m_RoomGraph.clear();
	m_RoomGraph.resize(m_Room.size());

	//それぞれの部屋の接続を登録
	for (int i = 0; i < static_cast<int>(m_RoomLinks.size()); i++)
	{
		const RoomLink& link = m_RoomLinks[i];

		//範囲外なら何もしない
		if (link.m_RoomA < 0 || link.m_RoomA >= static_cast<int>(m_Room.size()))continue;
		if (link.m_RoomB < 0 || link.m_RoomB >= static_cast<int>(m_Room.size()))continue;

		//両方の部屋の直通部屋リストに追加
		m_RoomGraph[link.m_RoomA].push_back(link.m_RoomB);
		m_RoomGraph[link.m_RoomB].push_back(link.m_RoomA);
	}
}

//どの部屋を通ればいいか調べる
std::vector<int> CMapData::FindRoomRoute(int startRoom,int goalRoom) const{

	//どのルートでいけばいいかの返り値
	std::vector<int> route;

	//部屋の数を取得
	const int roomCount = static_cast<int>(m_RoomGraph.size());

	//範囲外ならやめる
	if (startRoom < 0 || startRoom >= roomCount) return route;
	if (goalRoom < 0 || goalRoom >= roomCount) return route;
	
	//次に調べるもの
	std::queue<int> roomQueue;

	//どの部屋から来たか
	std::vector<int> previous(roomCount, -1);

	//すでに訪問しているか
	std::vector<bool> visited(roomCount, false);

	//開始地点を入れる
	visited[startRoom] = true;
	roomQueue.push(startRoom);

	//中身が空じゃないなら続ける
	while (!roomQueue.empty())
	{
		//調べる部屋IDを取得
		const int currentRoom = roomQueue.front();
		roomQueue.pop();

		//調べる部屋がゴールなら終わり
		if (currentRoom == goalRoom) break;

		for (int i = 0; i < static_cast<int>(m_RoomGraph[currentRoom].size()); i++)
		{
			int nextRoom = m_RoomGraph[currentRoom][i];

			//範囲外なら何もしない
			if (nextRoom < 0 || nextRoom >= roomCount) continue;
			
			//訪問済みなら何もしない
			if (visited[nextRoom])continue;

			//ここまできたら、ちゃんと存在した未訪問の部屋

			//訪問済みにする
			visited[nextRoom] = true;
			//次に訪問する部屋にする
			previous[nextRoom] = currentRoom;
			roomQueue.push(nextRoom);
		}
	}

	//ゴールまでの経路がないなら終わる
	if (!visited[goalRoom]) return route;
	
	//前の部屋を記録
	int currentRoom = goalRoom;

	//前の部屋がない状態になったら終了
	while (currentRoom != -1)
	{
		//前の部屋を見る
		route.push_back(currentRoom);

		//最初の部屋と同じになったら終了
		if (currentRoom == startRoom)break;
		
		//前の部屋のさらに前の部屋を見る
		currentRoom = previous[currentRoom];
	}

	//そもそもルートがないか、一番最後がスタートと一致していないなら何かがおかしいのでやめる
	if (route.empty() || route.back() != startRoom)
	{
		route.clear();
		return route;
	}

	//順番が逆なので反転
	std::reverse(route.begin(), route.end());

	return route;
}

const RoomLink* CMapData::FindRoomLink(int currentRoomID,int nextRoomID){
	//全部のLinkを見る
	for (int i = 0; i < static_cast<int>(m_RoomLinks.size()); i++)
	{
		const RoomLink& link = m_RoomLinks[i];
		//前方向と後方向からの両方を見る
		bool forward = link.m_RoomA == currentRoomID && link.m_RoomB == nextRoomID;
		bool backward = link.m_RoomB == currentRoomID && link.m_RoomA == nextRoomID;

		//どちらかでも当たっていたらその経路を返す
		if (forward == true || backward == true) return &link;
	}

	//見つからないならnullを返す
	return nullptr;
}

//どちらの部屋から来ているか
Int2 CMapData::GetStartGate(const RoomLink& link,int currentRoomID){
	if (link.m_RoomA == currentRoomID) return link.m_GateA;

	return link.m_GateB;
}

const RoomLink* CMapData::FindRoomLinkByPosition(const Int2& position) const
{
	//全てのリンクを調べる
	for (const RoomLink& link : m_RoomLinks)
	{
		//この廊下のリンクに調べたい座標があるか調べる
		if (IsPositionOnRoomLink(link, position))
		{
			return &link;
		}
	}

	return nullptr;
}

bool CMapData::IsPositionOnRoomLink(const RoomLink& link,const Int2& position) const
{
	//目的地と座標が一致していたら、その座標は廊下のリンクである
	if (position.x == link.m_GateA.x &&position.y == link.m_GateA.y)
	{
		return true;
	}
	if (position.x == link.m_GateB.x &&position.y == link.m_GateB.y)
	{
		return true;
	}

	//巡回ルートの座標を一つ一つ調べる
	for (const Int2& routePosition : link.m_Route)
	{
		//巡回ルートの中に調べたい座標と一致するものがあれば、その座標は廊下のリンクである
		if (position.x == routePosition.x &&position.y == routePosition.y)
		{
			return true;
		}
	}

	return false;
}