#pragma once
#include"../Enemy.h"
#include"../../Player/Player.h"

class CEnemy1 : public CEnemy {
public:
    CEnemy1(CEnemyModelManager* modelManager);
    ~CEnemy1() override;
protected:
    void InitStatus() override;
    void InitModelType() override;

    void Attackeffect(CPlayer& player) override;
};