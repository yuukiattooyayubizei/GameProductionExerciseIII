#pragma once
#include"../Enemy.h"
#include"../../Player/Player.h"

class CEnemy2 : public CEnemy {
public:
    CEnemy2(CEnemyModelManager* modelManager);
    ~CEnemy2() override;
protected:
    void InitStatus() override;
    void InitModelType() override;

    void Attackeffect(CPlayer& player) override;
};