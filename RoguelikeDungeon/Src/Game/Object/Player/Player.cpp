#include"Player.h"
#include "../../../lib/Input/Input.h"
#include "../../Common.h"
#include"../../Item/Item.h"
#include<iostream>
#include<algorithm>
#include"../../UI/UI.h"
#include<string>
#include "../../UI/Log.h"
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
	m_Exp = 0;
	m_Lv = 1;
	m_IsStomping = false;
	m_IsMove = false;
	m_hndl = -1;
	m_NextNecessaryExp = 20;
	m_MoveLongPress = 0;
	m_CanLongPress = true;
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
		//押し直したから長押しの判定を復活
		m_CanLongPress = true;
		//こっちでも動いたら長押しの判定を戻す
		if(m_MoveLongPress >= 20)
			m_MoveLongPress = 20;
		else
			m_MoveLongPress = 0;
	}
	if (IsInputTrg(KEY_S) && canmove.Down == true) {
		m_Direction = DIRECTION_DOWN;
		m_IsMove = true;
		m_CanLongPress = true;
		if (m_MoveLongPress >= 20)
			m_MoveLongPress = 20;
		else
			m_MoveLongPress = 0;
	}
	if (IsInputTrg(KEY_A) && canmove.Left == true) {
		m_Direction = DIRECTION_LEFT;
		m_IsMove = true;
		m_CanLongPress = true;
		if (m_MoveLongPress >= 20)
			m_MoveLongPress = 20;
		else
			m_MoveLongPress = 0;
	}
	if (IsInputTrg(KEY_D) && canmove.Right == true) {
		m_Direction = DIRECTION_RIGHT;
		m_IsMove = true;
		m_CanLongPress = true;
		if (m_MoveLongPress >= 20)
			m_MoveLongPress = 20;
		else
			m_MoveLongPress = 0;
	}

	//長押しで判定できる状況なら(敵に攻撃した時などにm_CanLongPressがfalseになる)
	//一度移動キーから離して、もう一度押し直しなら長押しの判定がたまるようになる
	//移動キーを押していたら
	if(m_CanLongPress == true)
	{
		if (IsInputRep(KEY_W) || IsInputRep(KEY_A) || IsInputRep(KEY_S) || IsInputRep(KEY_D))
			//長押しの判定が溜まる
			m_MoveLongPress++;
		else
			m_MoveLongPress = 0;
	}
	else
		m_MoveLongPress = 0;

	//長押しが続いたらその方向に移動
	if (m_MoveLongPress >= 30){
		if (IsInputRep(KEY_W) && canmove.Up == true) {
			m_Direction = DIRECTION_UP;
			m_IsMove = true;
			m_MoveLongPress = 20;
		}
		if (IsInputRep(KEY_S) && canmove.Down == true) {
			m_Direction = DIRECTION_DOWN;
			m_IsMove = true;
			m_MoveLongPress = 20;
		}
		if (IsInputRep(KEY_A) && canmove.Left == true) {
			m_Direction = DIRECTION_LEFT;
			m_IsMove = true;
			m_MoveLongPress = 20;
		}
		if (IsInputRep(KEY_D) && canmove.Right == true) {
			m_Direction = DIRECTION_RIGHT;
			m_IsMove = true;
			m_MoveLongPress = 20;
		}
	}

	if (IsInputRep(KEY_G))
		m_IsMove = false;

	if (IsInputTrg(KEY_SPACE))
		DrawInventoryDebug();

	if (IsInputTrg(KEY_F))
		m_IsStomping = true;
	else
		m_IsStomping = false;

	MV1SetPosition(m_hndl, VGet(-m_Pos.x * TILE_SIZE,151, m_Pos.y * TILE_SIZE));

	if (m_Exp >= m_NextNecessaryExp)
		LvUp();
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
	CLog* Log = CLog::GetInstance();
	// 最大数以上は持てない
	if (IsInventoryFull())
		return false;

	// 同じアイテムでもまとめずに、そのまま1個追加する
	m_Inventory.push_back(item);

	std::cout << "アイテム追加完了" << std::endl;
	std::string rog;
	switch (item.m_Type)
	{
	case ITEM_1:
		rog = "アイテム1を入手した";
		Log->AddLog(rog);
		break;
	case ITEM_2:
		rog = "アイテム2を入手した";
		Log->AddLog(rog);
		break;
	case ITEM_3:
		rog = "アイテム3を入手した";
		Log->AddLog(rog);
		break;
	case ITEM_4:
		rog = "アイテム4を入手した";
		Log->AddLog(rog);
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

		switch (invectory.m_Type)
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

//レベルアップ時の処理
void CPlayer::LvUp() {
	CLog* Log = CLog::GetInstance();

	m_Lv++;
	m_MaxHP += 3;
	m_HP += 3;
	m_Atk += 1;
	m_Exp = m_NextNecessaryExp;
	m_NextNecessaryExp += 10;

	std::string rog = std::to_string(m_Lv) + "レベルになった";
	Log->AddLog(rog);
}