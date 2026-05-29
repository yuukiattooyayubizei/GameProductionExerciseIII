#include"Enemy.h"
#include "../../../lib/Input/input.h"
#include <iostream>
#include "../../Map/Map.h"

CEnemy::CEnemy() {
	Init();
}

CEnemy::~CEnemy() {
}

void CEnemy::Init() {
	m_EnemyType = static_cast<EnemyType>(GetRand(ENEMY_TYPE_NUM - 1));
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Kind = KIND_ENEMY;
	switch (m_EnemyType)
	{
	case ENEMY_1:
		m_HP = 5;
		m_MaxHP = 5;
		m_Atk = 2;
		break;
	case ENEMY_2:
		m_HP = 7;
		m_MaxHP = 7;
		m_Atk = 2;
		break;
	case ENEMY_3:
		m_HP = 5;
		m_MaxHP = 5;
		m_Atk = 3;
		break;
	case ENEMY_4:
		m_HP = 15;
		m_MaxHP = 15;
		m_Atk = 1;
		break;
	case ENEMY_TYPE_NUM:
		break;
	default:
		break;
	}
	for (int i = 0;i < 3;i++) {
		m_Enemyhndl[i] = -1;

	}

	m_Enemyhndl[0] = MV1LoadModel("Data/Model/Enemy1.x");
	m_Enemyhndl[1] = MV1LoadModel("Data/Model/Enemy2.x");
	m_Enemyhndl[2] = MV1LoadModel("Data/Model/Enemy3.x");
	m_Enemyhndl[3] = MV1LoadModel("Data/Model/Enemy4.x");

	MV1SetScale(m_Enemyhndl[0], VGet(0.5f, 0.5f, 0.5f));
	MV1SetScale(m_Enemyhndl[1], VGet(0.5f, 0.5f, 0.5f));
	MV1SetScale(m_Enemyhndl[2], VGet(0.5f, 0.5f, 0.5f));
	MV1SetScale(m_Enemyhndl[3], VGet(0.5f, 0.5f, 0.5f));
}

void CEnemy::Load() {
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

void CEnemy::Draw() {
	int centerX = 8 + m_Pos.x * 16;
	int centerY = 8 + m_Pos.y * 16;
	int color = 0;


	float x = -m_Pos.x * TILE_SIZE;
	float z = m_Pos.y * TILE_SIZE;

	VECTOR pos1 = VGet(x - 50.0f, 150, z - 50.0f);
	VECTOR pos2 = VGet(x + 50.0f, 150 + 100.0f, z + 50.0f);


	switch (m_EnemyType)
	{
	case ENEMY_1:
	//	DrawBox(centerX + 4, centerY + 4, centerX - 4, centerY - 4, GetColor(0, 0, 0), TRUE);
	//	DrawCube3D(pos1, pos2, GetColor(128, 255, 255), GetColor(128, 128, 128), TRUE);
		MV1SetPosition(m_Enemyhndl[0], VGet(-m_Pos.x * TILE_SIZE, 151, m_Pos.y * TILE_SIZE));
		MV1DrawModel(m_Enemyhndl[0]);
		break;
	case ENEMY_2:
	//	DrawBox(centerX + 4, centerY + 4, centerX - 4, centerY - 4, GetColor(128, 0, 0), TRUE);
	//	DrawCube3D(pos1, pos2, GetColor(128, 128, 255), GetColor(128, 128, 128), TRUE);
		MV1SetPosition(m_Enemyhndl[1], VGet(-m_Pos.x * TILE_SIZE, 151, m_Pos.y * TILE_SIZE));
		MV1DrawModel(m_Enemyhndl[1]);
		break;
	case ENEMY_3:
	//	DrawBox(centerX + 4, centerY + 4, centerX - 4, centerY - 4, GetColor(0, 128, 0), TRUE);
	//	DrawCube3D(pos1, pos2, GetColor(128, 255, 128), GetColor(128, 128, 128), TRUE);
		MV1SetPosition(m_Enemyhndl[2], VGet(-m_Pos.x * TILE_SIZE, 151, m_Pos.y * TILE_SIZE));
		MV1DrawModel(m_Enemyhndl[2]);
		break;
	case ENEMY_4:
	//	DrawBox(centerX + 4, centerY + 4, centerX - 4, centerY - 4, GetColor(0, 0, 128), TRUE);
	//	DrawCube3D(pos1, pos2, GetColor(255, 128, 128), GetColor(128, 128, 128), TRUE);

		MV1SetPosition(m_Enemyhndl[3], VGet(-m_Pos.x * TILE_SIZE, 151, m_Pos.y * TILE_SIZE));
		MV1DrawModel(m_Enemyhndl[3]);
		break;
	case ENEMY_TYPE_NUM:
		break;
	default:
		break;
	}
}

void CEnemy::Exit() {
	for (int i = 0;i < 3;i++) {
		MV1DeleteModel(m_Enemyhndl[i]);
		m_Enemyhndl[i] = -1;

	}
}

