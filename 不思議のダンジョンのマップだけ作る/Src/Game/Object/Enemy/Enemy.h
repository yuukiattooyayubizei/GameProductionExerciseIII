#pragma once
#include"../Object.h"
#include <DxLib.h>
#include "../../Common.h"
#include "../../Item/Item.h"
#include <vector>

class CEnemy : public CObject {
private:

public:
    CEnemy();
    ~CEnemy()override;

    void Init()override;
    void Load()override;
    void Step(CanMove canmove)override;
    void Draw()override;
    void Exit()override;
public:

};
