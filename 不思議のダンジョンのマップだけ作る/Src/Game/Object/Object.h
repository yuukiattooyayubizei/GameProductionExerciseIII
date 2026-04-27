#pragma once
#include <DxLib.h>
#include"../common.h"

enum ObjectKind {
    KIND_NON,
    KIND_PLAYER,
};



class CObject {
protected:
    ObjectKind m_Kind;
    VECTOR m_Pos;
    DIRECTION m_Direction;
public:
    CObject();
    virtual ~CObject() {}

    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Step(CanMove canmove) = 0;
    virtual void Draw() = 0;
    virtual void Exit() = 0;

    VECTOR GetPos() const { return m_Pos; }
    ObjectKind GetKind() const { return m_Kind; }
    DIRECTION GetDirection() const { return m_Direction; }


    void SetPos(VECTOR pos) { m_Pos = pos; }
};