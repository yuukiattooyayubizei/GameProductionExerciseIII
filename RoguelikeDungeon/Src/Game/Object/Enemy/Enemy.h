#pragma once
#include "../Object.h"
#include "EnemyModelManager.h"

enum EnemyType {
    ENEMY_1,  
    ENEMY_2,  
    ENEMY_3, 
    ENEMY_4,  

    ENEMY_TYPE_NUM,
};

enum EnemyMoveType {
    MOVE_WALK,
    MOVE_TRACK,
};

class CEnemy : public CObject {
protected:
    EnemyType m_EnemyType;
    CEnemyModelManager* m_ModelManager = nullptr;
    EnemyMoveType m_MoveType;

public:
    CEnemy(CEnemyModelManager* modelManager);
    virtual ~CEnemy() override;

    virtual void Init() override;
    virtual void Load() override;
    virtual void Step(CanMove canmove, Int2 playerPos) override;
    virtual void Draw() override;
    virtual void Exit() override;

protected:
    virtual void InitStatus() = 0;
    virtual void InitModelType() = 0;
};