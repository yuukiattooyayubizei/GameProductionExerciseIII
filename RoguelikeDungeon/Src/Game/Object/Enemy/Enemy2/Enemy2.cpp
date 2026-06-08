#include "Enemy2.h"

CEnemy2::CEnemy2(CEnemyModelManager* modelManager)
    : CEnemy(modelManager)
{
   
}

CEnemy2::~CEnemy2() {
}

void CEnemy2::InitModelType() {
    m_EnemyType = ENEMY_2;
}

void CEnemy2::InitStatus() {
    m_HP = 7;
    m_MaxHP = 7;
    m_Atk = 2;
}