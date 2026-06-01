#include"Enemy.h"
#include "../../../lib/Input/input.h"
#include <iostream>
#include "../../Map/Map.h"



CEnemy::CEnemy(CEnemyModelManager* modelManager)
    : m_ModelManager(modelManager)
{
}



CEnemy::~CEnemy() {
}

void CEnemy::Init() {
    m_Pos.x = 0;
    m_Pos.y = 0;
    m_Kind = KIND_ENEMY;

    InitModelType();
    InitStatus();
}

void CEnemy::Load() {
}

void CEnemy::Draw() {
    if (m_ModelManager == nullptr) {
        return;
    }

    int hndl = m_ModelManager->GetModel(m_EnemyType);

    if (hndl == -1) {
        return;
    }

    MV1SetPosition(hndl, VGet(-m_Pos.x * TILE_SIZE, 151, m_Pos.y * TILE_SIZE));

    MV1DrawModel(hndl);
}

void CEnemy::Step(CanMove canmove, Int2 playerPos) {

	CMap* Map = CMap::GetInstance();

	//プレイヤーと敵のいる部屋を探す
	int PlayerRoomNum = Map->GetRoomNum(playerPos);
	int EnemyRoomNum = Map->GetRoomNum(m_Pos);
	Int2 SubPos = SubInt2(playerPos, m_Pos);

	//プレイヤーが隣にいるならそっちに移動(攻撃)
	if(SubPos.x == 1 && SubPos.y == 0)
		m_Direction = DIRECTION_RIGHT;
	else if (SubPos.x == -1 && SubPos.y == 0)
		m_Direction = DIRECTION_LEFT;
	else if (SubPos.x== 0 && SubPos.y == 1)
		m_Direction = DIRECTION_DOWN;
	else if (SubPos.x == 0 && SubPos.y == -1)
		m_Direction = DIRECTION_UP;
	//同じ部屋にいるなら、プレイヤーの方向に向かう
	//-1は部屋にいないという意味になるので除外
	else if (PlayerRoomNum != -1 && PlayerRoomNum == EnemyRoomNum)
	{
		

		//xかyのどちらかが同じなら直線的に進む
		if (SubPos.x == 0)
		{
			if (SubPos.y >= 0)
				m_Direction = DIRECTION_DOWN;
			else
				m_Direction = DIRECTION_UP;
		}
		else if (SubPos.y == 0)
		{
			if (SubPos.x >= 0)
				m_Direction = DIRECTION_RIGHT;
			else
				m_Direction = DIRECTION_LEFT;
		}
		//ここまで来たら、xとyどちらも合っていない
		else
		{
			if (SubPos.x > 0 && SubPos.y > 0)
			{
				//プレイヤーが右下にいるとき
				//ランダムで右か下に
				if(GetRand(1) == 0)
					m_Direction = DIRECTION_RIGHT;
				else
					m_Direction = DIRECTION_DOWN;
			}
			if (SubPos.x < 0 && SubPos.y > 0)
			{
				//プレイヤーが左下にいるとき
				//ランダムで左か下に
				if (GetRand(1) == 0)
					m_Direction = DIRECTION_LEFT;
				else
					m_Direction = DIRECTION_DOWN;
			}
			if (SubPos.x > 0 && SubPos.y < 0)
			{
				//プレイヤーが右上にいるとき
				//ランダムで右か上に
				if (GetRand(1) == 0)
					m_Direction = DIRECTION_RIGHT;
				else
					m_Direction = DIRECTION_UP;
			}
			if (SubPos.x < 0 && SubPos.y < 0)
			{
				//プレイヤーが左上にいるとき
				//ランダムで左か上に
				if (GetRand(1) == 0)
					m_Direction = DIRECTION_LEFT;
				else
					m_Direction = DIRECTION_UP;
			}
		}
	}
	else
	{
		//部屋が違うならとりあえずランダムで
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

}

void CEnemy::Exit() {
}