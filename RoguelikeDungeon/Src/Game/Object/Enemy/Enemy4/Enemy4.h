#pragma once
#include "../Enemy.h"

class CEnemy4 : public CEnemy {
public:
    CEnemy4(CEnemyModelManager* modelManager);
    ~CEnemy4() override;

protected:
    void InitStatus() override;
    void InitModelType() override;
};