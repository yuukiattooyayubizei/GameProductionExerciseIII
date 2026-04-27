#include"Player.h"
#include "../../../lib/Input/input.h"
#include "../../Common.h"



CPlayer::~CPlayer(){
}

void CPlayer::Init(){
	m_Pos = VGet(0,0,0);
	m_Kind = KIND_PLAYER;
}

void CPlayer::Load() {
}

void CPlayer::Step(CanMove canmove) {
	m_Direction = DIRECTION_NON;

	if (IsInputTrg(KEY_W) && canmove.Up == true) {
		m_Pos.y--;
		m_Direction = DIRECTION_DOWN;
	}
	if (IsInputTrg(KEY_S) && canmove.Down == true){
		m_Pos.y++;
		m_Direction = DIRECTION_UP;
	}
	if (IsInputTrg(KEY_A) && canmove.Left == true) {
		m_Pos.x--;
		m_Direction = DIRECTION_LEFT;
	}
	if (IsInputTrg(KEY_D) && canmove.Right == true) {
		m_Pos.x++;
		m_Direction = DIRECTION_RIGHT;
	}
}

void CPlayer::Draw() {
	DrawFormatString(32, 32, GetColor(255, 255, 255), "posx = %f", m_Pos.x);
	DrawFormatString(32, 64, GetColor(255, 255, 255), "posy = %f", m_Pos.y);
}

void CPlayer::Exit() {

}