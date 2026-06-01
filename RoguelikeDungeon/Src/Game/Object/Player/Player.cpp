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
	m_IsStomping = false;
	m_IsMove = false;
	m_hndl = -1;
}

void CPlayer::Load() {
	if (m_hndl == -1) {

		m_hndl = MV1LoadModel("Data/Model/Player.x");

		MV1SetScale(m_hndl, VGet(0.5f, 0.5f, 0.5f));

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
	//if (IsInputTrg(KEY_W)) {
	//	m_Direction = DIRECTION_UP;
	//	m_IsMove = true;
	//}
	//if (IsInputTrg(KEY_S)) {
	//	m_Direction = DIRECTION_DOWN;
	//	m_IsMove = true;
	//}
	//if (IsInputTrg(KEY_A)) {
	//	m_Direction = DIRECTION_LEFT;
	//	m_IsMove = true;
	//}
	//if (IsInputTrg(KEY_D)) {
	//	m_Direction = DIRECTION_RIGHT;
	//	m_IsMove = true;
	//}

	if (IsInputRep(KEY_G))
		m_IsMove = false;

	if (IsInputTrg(KEY_SPACE))
		DrawInventoryDebug();

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
//	DrawBox(centerX + 4, centerY + 4, centerX - 4, centerY - 4, GetColor(255, 255, 255), TRUE);

	float x = -m_Pos.x * TILE_SIZE;
	float z = m_Pos.y * TILE_SIZE;

	VECTOR pos1 = VGet(x - 50.0f, 150, z - 50.0f);
	VECTOR pos2 = VGet(x + 50.0f, 150 + 100.0f, z + 50.0f);

//	DrawCube3D(pos1, pos2, GetColor(128,128,128), GetColor(128, 128, 128), TRUE);

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

//アイテムの消去
bool CPlayer::EraseItem(int index) {
	//配列外なら消さない
	if (index < 0 || index >= static_cast<int>(m_Inventory.size()))
		return false;

	m_Inventory.erase(m_Inventory.begin() + index);
	return true;
}