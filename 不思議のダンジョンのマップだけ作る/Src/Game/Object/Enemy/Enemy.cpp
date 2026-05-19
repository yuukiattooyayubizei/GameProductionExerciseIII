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
	m_Atk = 2;
}

void CEnemy::Load() {
}

void CEnemy::Step(CanMove canmove) {
	//‚Æ‚è‚ ‚¦‚¸ƒ‰ƒ“ƒ_ƒ€‚Å
	int rand = GetRand(3);
	if (rand == 0 && canmove.Up == true) {
		m_Direction = DIRECTION_UP;
	}
	else if (rand == 1 && canmove.Down == true) {
		m_Direction = DIRECTION_DOWN;
	}
	else if (rand == 2 && canmove.Left == true) {
		m_Direction = DIRECTION_LEFT;
	}
	else if (rand == 3 && canmove.Right == true) {
		m_Direction = DIRECTION_RIGHT;
	}
	else
		m_Direction = DIRECTION_NON;
}

void CEnemy::Draw() {
	int centerX = 8 + m_Pos.x * 16;
	int centerY = 8 + m_Pos.y * 16;
	DrawBox(centerX + 4, centerY + 4, centerX - 4, centerY - 4, GetColor(0, 0, 0), TRUE);
}

void CEnemy::Exit() {

}

