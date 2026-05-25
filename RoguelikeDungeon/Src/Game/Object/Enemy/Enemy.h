#pragma once
#include"../Object.h"
#include <DxLib.h>
#include "../../Common.h"
#include "../../Item/Item.h"
#include <vector>

enum EnemyType {
    ENEMY_1,  
    ENEMY_2,  
    ENEMY_3, 
    ENEMY_4,  

    ENEMY_TYPE_NUM,
};

class CEnemy : public CObject {
private:
    EnemyType m_EnemyType;
public:
    CEnemy();
    ~CEnemy()override;

    void Init()override;
    void Load()override;
    void Step(CanMove canmove, Int2 playerPos)override;
    void Draw()override;
    void Exit()override;
public:

};
