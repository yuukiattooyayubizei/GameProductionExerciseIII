#include "Object.h"

CObject::CObject() {
	m_Atk = 0;
	m_Direction = DIRECTION_NON;
	m_Kind = KIND_NON;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_HP = 0;
	m_MaxHP = 0;
}