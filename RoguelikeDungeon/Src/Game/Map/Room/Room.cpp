#include"Room.h"

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

bool CRoom::CollsionRoom(Int2 i) {
	if (i.x > m_Pos.x && i.x < m_Pos.x + m_Size.x && i.y > m_Pos.y && i.y < m_Pos.y + m_Size.y)
		return true;
	return false;
}