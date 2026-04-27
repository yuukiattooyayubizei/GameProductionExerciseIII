#pragma once
#include"../Object.h"

class CTrap : public CObject {
public:
    virtual ~CTrap() {}

    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Step() = 0;
    virtual void Draw() = 0;
    virtual void Exit() = 0;
};