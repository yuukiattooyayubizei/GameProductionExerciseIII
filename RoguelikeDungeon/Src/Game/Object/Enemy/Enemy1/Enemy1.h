#pragma once
#include"../Enemy.h"

class CEnemy1 : public CEnemy {
public:
    CEnemy1(CEnemyModelManager* modelManager);
    ~CEnemy1() override;

protected:
    void InitStatus() override;
    void InitModelType() override;
};