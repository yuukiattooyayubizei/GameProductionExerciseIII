#include"Enemy1.h"

CEnemy1::CEnemy1(CEnemyModelManager* modelManager)
    : CEnemy(modelManager)
{
    
}

CEnemy1::~CEnemy1() {
}

void CEnemy1::InitModelType() {
    m_EnemyType = ENEMY_1;
}

void CEnemy1::InitStatus() {
    m_HP = 5;
    m_MaxHP = 5;
    m_Atk = 2;
    m_Addexp = 3;
}

void CEnemy1::Attackeffect(CPlayer& player) {

}