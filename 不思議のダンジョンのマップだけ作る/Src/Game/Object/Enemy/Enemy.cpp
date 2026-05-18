#include"Enemy.h"
#include "../../../lib/Input/input.h"
#include <iostream>

CEnemy::CEnemy() {
	Init();
}

CEnemy::~CEnemy() {
}

void CEnemy::Init() {
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Kind = KIND_ENEMY;
	m_HP = 5;
	m_MaxHP = 5;
}

void CEnemy::Load() {
}

void CEnemy::Step(CanMove canmove) {

}

void CEnemy::Draw() {
	int centerX = 8 + m_Pos.x * 16;
	int centerY = 8 + m_Pos.y * 16;
	DrawBox(centerX + 4, centerY + 4, centerX - 4, centerY - 4, GetColor(0, 0, 0), TRUE);
}

void CEnemy::Exit() {

}

