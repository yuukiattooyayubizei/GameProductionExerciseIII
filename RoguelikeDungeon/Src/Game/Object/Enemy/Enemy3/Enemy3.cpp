#include"Enemy3.h"

CEnemy3::CEnemy3(CEnemyModelManager* modelManager)
    : CEnemy(modelManager)
{
    
}

CEnemy3::~CEnemy3() {
}

void CEnemy3::InitModelType() {
    m_EnemyType = ENEMY_3;
}

void CEnemy3::InitStatus() {
    m_HP = 8;
    m_MaxHP = 8;
    m_Atk = 3;
    m_Addexp = 6;
}