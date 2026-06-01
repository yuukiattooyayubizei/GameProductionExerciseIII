//#pragma once
//#include"../Object.h"
//#include <DxLib.h>
//#include "../../Common.h"
//#include "../../Item/Item.h"
//#include <vector>
//#include "EnemyModelManager.h"
//
//enum EnemyType {
//    ENEMY_1,  
//    ENEMY_2,  
//    ENEMY_3, 
//    ENEMY_4,  
//
//    ENEMY_TYPE_NUM,
//};
//
//class CEnemy : public CObject {
//private:
//    EnemyType m_EnemyType;
//    CModelManager* m_ModelManager = nullptr;
//
//public:
//    CEnemy(CModelManager* modelManager);
//    ~CEnemy()override;
//
//    void Init()override;
//    void Load()override;
//    void Step(CanMove canmove, Int2 playerPos)override;
//    void Draw()override;
//    void Exit()override;
//public:
//
//};

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

class CEnemy : public CObject {
protected:
    EnemyType m_EnemyType;
    CEnemyModelManager* m_ModelManager = nullptr;

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