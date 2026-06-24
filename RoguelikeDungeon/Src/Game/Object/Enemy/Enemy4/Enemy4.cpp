#include"Enemy4.h"

CEnemy4::CEnemy4(CEnemyModelManager* modelManager)
    : CEnemy(modelManager)
{
   
}

CEnemy4::~CEnemy4() {
}

void CEnemy4::InitModelType() {
    m_EnemyType = ENEMY_4;
}

void CEnemy4::InitStatus() {
    m_HP = 25;
    m_MaxHP = 25;
    m_Atk = 2;
    m_Addexp = 8;
}