#include "Enemy3.h"

CEnemy3::CEnemy3(CEnemyModelManager* modelManager)
    : CEnemy(modelManager)
{
    Init();
}

CEnemy3::~CEnemy3() {
}

void CEnemy3::InitModelType() {
    m_EnemyType = ENEMY_3;
}

void CEnemy3::InitStatus() {
    m_HP = 5;
    m_MaxHP = 5;
    m_Atk = 3;
}