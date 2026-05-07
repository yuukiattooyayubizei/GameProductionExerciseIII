#include"Player.h"
#include "../../../lib/Input/input.h"
#include "../../Common.h"
#include "../../Item/Item.h"



CPlayer::~CPlayer(){
}

void CPlayer::Init(){
	m_Pos = VGet(0,0,0);
	m_Kind = KIND_PLAYER;
	m_HP = HP_MAX;
	m_MaxHP = HP_MAX;
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

bool CPlayer::AddItem(const Item& item)
{
	// 最大数以上は持てない
	if (IsInventoryFull())
		return false;

	// 同じアイテムでもまとめずに、そのまま1個追加する
	m_Inventory.push_back(item);

	return true;
}

bool CPlayer::IsInventoryFull() const
{
	return static_cast<int>(m_Inventory.size()) >= INVENTORY_MAX;
}

void CPlayer::DrawInventoryDebug()
{
	int x = 820;
	int y = 20;

	DrawString(x, y, "Inventory", GetColor(255, 255, 255));
	y += 20;

	for (int i = 0; i < static_cast<int>(m_Inventory.size()); i++)
	{
		DrawFormatString(
			x,
			y,
			GetColor(255, 255, 255),
			"%02d: %s",
			i,
			/*GetItemName(m_Inventory[i].type)*/
			"a"
		);

		y += 20;
	}
}