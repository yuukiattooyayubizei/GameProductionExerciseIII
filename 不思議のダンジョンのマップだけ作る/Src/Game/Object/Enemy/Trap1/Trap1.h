#pragma once
#include"../Trap.h"
#include <DxLib.h>
#include "../../../Data/Data.h"

class CTrap1 : public CTrap {
public:
    CTrap1();
    ~CTrap1()override;

    void Init()override;
    void Load()override;
    void Step()override;
    void Draw()override;
    void Exit()override;
private:
    TrapData m_TrapData;
    int m_hndl;
    int m_Size;                 //ã©‚Ì‘å‚«‚³
public:
    VECTOR GetPos() { return m_Pos; }
    int GetSize() { return m_TrapData.m_Size; }
    int Getm_FallingSpd() { m_TrapData.m_FallingSpd; }
};