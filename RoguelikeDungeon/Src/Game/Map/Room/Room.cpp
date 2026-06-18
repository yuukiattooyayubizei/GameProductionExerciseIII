#include"Room.h"

CRoom::CRoom() {
	m_Center.x = 0;
	m_Center.y = 0;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Size.x = 0;
	m_Size.y = 0;
	m_CloseRoom = -1;
	m_ConnectAheadRoom = {};

	m_IsConnectRoom = false;
}

bool CRoom::CollisionRoom(Int2 i) {
	if (i.x >= m_Pos.x && i.x < m_Pos.x + m_Size.x && i.y >= m_Pos.y && i.y < m_Pos.y + m_Size.y)
		return true;
	return false;
}

//Œq‚ª‚Á‚Ä‚¢‚é•”‰®‚ÌID‚ð’Ç‰Á
void CRoom::AddConnectAheadRoom(int id, Int2 Pos) {
	ConnectRoomData Data = {};
	Data.ConnectRoom = id;
	Data.ConnectCorridorPos = Pos;
	m_ConnectAheadRoom.push_back(Data);
}

std::vector<int> CRoom::GetConnectAheadRoomID() {
	std::vector<int> ret = {};
	//m_ConnectAheadRoom‚ÌConnectRoom‚ð‚·‚×‚Ä“ü‚ê‚é
	for (int i = 0;i < m_ConnectAheadRoom.size();i++) {
		ret.push_back(m_ConnectAheadRoom[i].ConnectRoom);
	}

	return ret;
}

Int2 CRoom::GetConnectAheadRoomPos(int id) {
	//”z—ñŠO‚ÌID‚¾‚Æ-1,-1‚ð•Ô‚·
	if (id < 0 || id >= m_ConnectAheadRoom.size()) {
		Int2 ret = {-1,-1};
		return ret;
	}
	return m_ConnectAheadRoom[id].ConnectCorridorPos; }