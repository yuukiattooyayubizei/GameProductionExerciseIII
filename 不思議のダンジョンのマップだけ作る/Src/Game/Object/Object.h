#pragma once
#include <DxLib.h>
#include"../common.h"

enum ObjectKind {
    KIND_NON,
    KIND_PLAYER,
    KIND_ENEMY,
};



class CObject {
protected:
    ObjectKind m_Kind;
    Int2 m_Pos;
    DIRECTION m_Direction;

    int m_HP;
    int m_MaxHP;
public:
    CObject();
    virtual ~CObject() {}

    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Step(CanMove canmove) = 0;
    virtual void Draw() = 0;
    virtual void Exit() = 0;

    Int2 GetPos() const { return m_Pos; }
    ObjectKind GetKind() const { return m_Kind; }
    DIRECTION GetDirection() const { return m_Direction; }


    void SetPos(Int2 pos) {m_Pos.x = pos.x;m_Pos.y = pos.y;}
};