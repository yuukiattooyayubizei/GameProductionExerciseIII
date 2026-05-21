#include"Player.h"
#include "../../../lib/Input/input.h"
#include "../../Common.h"
#include "../../Item/Item.h"
#include <iostream>
#include <algorithm>
using namespace std;



CPlayer::~CPlayer(){
}

void CPlayer::Init(){
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Kind = KIND_PLAYER;
	m_HP = HP_MAX;
	m_MaxHP = HP_MAX;
	m_Atk = 5;
}

void CPlayer::Load() {
}

void CPlayer::Step(CanMove canmove, Int2 playerPos) {
	m_Direction = DIRECTION_NON;

	if (IsInputTrg(KEY_W) && canmove.Up == true) {
		m_Direction = DIRECTION_UP;
	}
	if (IsInputTrg(KEY_S) && canmove.Down == true) {
		m_Direction = DIRECTION_DOWN;
	}
	if (IsInputTrg(KEY_A) && canmove.Left == true) {
		m_Direction = DIRECTION_LEFT;
	}
	if (IsInputTrg(KEY_D) && canmove.Right == true) {
		m_Direction = DIRECTION_RIGHT;
	}

	if (IsInputTrg(KEY_SPACE))
		DrawInventoryDebug();
}

void CPlayer::Draw() {
	DrawFormatString(32, 32, GetColor(255, 255, 255), "posx = %d", m_Pos.x);
	DrawFormatString(32, 64, GetColor(255, 255, 255), "posy = %d", m_Pos.y);
	DrawFormatString(32, 96, GetColor(255, 255, 255), "HP = %d / %d", m_HP, m_MaxHP);


	//プレイヤーの描画
	int centerX = 8 + m_Pos.x * 16;
	int centerY = 8 + m_Pos.y * 16;
	DrawBox(centerX + 4, centerY + 4, centerX - 4, centerY - 4, GetColor(255, 255, 255), TRUE);
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

	std::cout << "アイテム追加完了" << std::endl;
	switch (item.type)
	{
	case ITEM_1:
		std::cout << "アイテム1" << std::endl;
		break;
	case ITEM_2:
		std::cout << "アイテム2" << std::endl;
		break;
	case ITEM_3:
		std::cout << "アイテム3" << std::endl;
		break;
	case ITEM_4:
		std::cout << "アイテム4" << std::endl;
		break;
	default:
		break;
	}

	return true;
}

bool CPlayer::IsInventoryFull() const
{
	return static_cast<int>(m_Inventory.size()) >= INVENTORY_MAX;
}

void CPlayer::DrawInventoryDebug()
{
	std::cout << "Inventory" << std::endl;
	for_each(m_Inventory.begin(), m_Inventory.end(), [&](Item invectory) {

		switch (invectory.type)
		{
		case ITEM_1:
			std::cout << "アイテム1" << std::endl;
			break;
		case ITEM_2:
			std::cout << "アイテム2" << std::endl;
			break;
		case ITEM_3:
			std::cout << "アイテム3" << std::endl;
			break;
		case ITEM_4:
			std::cout << "アイテム4" << std::endl;
			break;
		default:
			break;
		}

	});
}