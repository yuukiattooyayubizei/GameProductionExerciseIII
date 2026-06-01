#pragma once
#include "../Enemy.h"

class CEnemy3 : public CEnemy {
public:
    CEnemy3(CEnemyModelManager* modelManager);
    ~CEnemy3() override;

protected:
    void InitStatus() override;
    void InitModelType() override;
};