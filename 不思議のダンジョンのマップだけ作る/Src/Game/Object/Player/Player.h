#pragma once
#include"../Object.h"
#include <DxLib.h>
#include "../../Common.h"

static constexpr int HP_MAX = 15;


class CPlayer : public CObject {
private:

public:
    ~CPlayer()override;

    void Init()override;
    void Load()override;
    void Step(CanMove canmove)override;
    void Draw()override;
    void Exit()override;

public:
};
