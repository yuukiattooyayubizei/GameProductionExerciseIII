#pragma once
#include"../Common.h"

enum ObjectKind {
    KIND_NON,
    KIND_PLAYER,
    KIND_ENEMY,

    KIND_NUM,
};

class CObject {
protected:
    ObjectKind m_Kind;
    Int2 m_Pos;
    DIRECTION m_Direction;

    int m_HP;
    int m_MaxHP;
    int m_Atk;
    bool m_IsActive = true;
public:
    CObject();
    virtual ~CObject() {}

    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Step(CanMove canmove, Int2 playerPos) = 0;
    virtual void Draw() = 0;
    virtual void Exit() = 0;

    Int2 GetPos() const { return m_Pos; }
    ObjectKind GetKind() const { return m_Kind; }
    DIRECTION GetDirection() const { return m_Direction; }
    int GetAtk() const { return m_Atk; }
    int GetHP() const { return m_HP; }
    int GetMaxHP() const { return m_MaxHP; }
    bool GetActive() const { return m_IsActive; }


    void SetPos(Int2 pos) {m_Pos.x = pos.x;m_Pos.y = pos.y;}
    void AddPos(Int2 pos) { m_Pos.x += pos.x;m_Pos.y += pos.y; }
    void AddDamage(int dam) {
        m_HP -= dam;
        if (m_HP <= 0)
            m_IsActive = false;
    }
    void AddHeal(int hp) {
        m_HP += hp;
        if (m_HP > m_MaxHP)
            m_HP = m_MaxHP;
    }
    void AddMaxHP(int mhp) { m_MaxHP += mhp; }
    void AddAtk(int atk) { m_Atk+= atk; }
    void SetActive(bool active) { m_IsActive = active; }
};