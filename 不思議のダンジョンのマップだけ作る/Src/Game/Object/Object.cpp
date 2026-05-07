#include "Object.h"

CObject::CObject() {
	m_Kind = KIND_NON;
	m_Pos = VGet(0,0,0);
	m_HP = 0;
	m_MaxHP = 0;
}