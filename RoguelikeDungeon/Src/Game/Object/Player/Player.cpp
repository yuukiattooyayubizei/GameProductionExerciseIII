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
	m_HP = INITIAL_HP;
	m_MaxHP = INITIAL_HP;
	m_Atk = INITIAL_ATK;
	m_IsStomping = false;
	m_IsMove = false;
	m_hndl = -1;
}

void CPlayer::Load() {
	if (m_hndl == -1) {
		m_hndl = LoadModelWithScale("Data/Model/Player.x", VGet(0.5f, 0.5f, 0.5f));
	}	
}

void CPlayer::Step(CanMove canmove, Int2 playerPos) {
	m_IsMove = false;

	if (IsInputTrg(KEY_W) && canmove.Up == true) {
		m_Direction = DIRECTION_UP;
		m_IsMove = true;
	}
	if (IsInputTrg(KEY_S) && canmove.Down == true) {
		m_Direction = DIRECTION_DOWN;
		m_IsMove = true;
	}
	if (IsInputTrg(KEY_A) && canmove.Left == true) {
		m_Direction = DIRECTION_LEFT;
		m_IsMove = true;
	}
	if (IsInputTrg(KEY_D) && canmove.Right == true) {
		m_Direction = DIRECTION_RIGHT;
		m_IsMove = true;
	}

	if (IsInputRep(KEY_G))
		m_IsMove = false;

	if (IsInputTrg(KEY_F))
		m_IsStomping = true;
	else
		m_IsStomping = false;


		MV1SetPosition(m_hndl, VGet(-m_Pos.x * TILE_SIZE,151, m_Pos.y * TILE_SIZE));
	
}

void CPlayer::Draw() {
	DrawFormatString(32, 32, GetColor(255, 255, 255), "posx = %d", m_Pos.x);
	DrawFormatString(32, 64, GetColor(255, 255, 255), "posy = %d", m_Pos.y);
	DrawFormatString(32, 96, GetColor(255, 255, 255), "HP = %d / %d", m_HP, m_MaxHP);

	//プレイヤーの描画
	int centerX = 8 + m_Pos.x * 16;
	int centerY = 8 + m_Pos.y * 16;

	float x = -m_Pos.x * TILE_SIZE;
	float z = m_Pos.y * TILE_SIZE;

	VECTOR pos1 = VGet(x - 50.0f, 150, z - 50.0f);
	VECTOR pos2 = VGet(x + 50.0f, 150 + 100.0f, z + 50.0f);

	MV1DrawModel(m_hndl);
}

void CPlayer::Exit() {
	if (m_hndl != -1)
	{
		MV1DeleteModel(m_hndl);
		m_hndl = -1;
	}
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

//アイテムの消去
bool CPlayer::EraseItem(int index) {
	//配列外なら消さない
	if (index < 0 || index >= static_cast<int>(m_Inventory.size()))
		return false;

	m_Inventory.erase(m_Inventory.begin() + index);
	return true;
}