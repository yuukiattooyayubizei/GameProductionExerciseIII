#pragma once
#include"../Object.h"
#include <DxLib.h>
#include "../../Common.h"


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
