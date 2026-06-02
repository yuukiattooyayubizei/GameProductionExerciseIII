#pragma once
#include "../Common.h"
#include"../Object/Object.h"
#include <vector>


// アイテム1個分の情報
struct Item
{
    ITEM_TYPE type;

};

// マップ上に落ちているアイテム
struct FieldItem
{
    Item item;   // アイテム本体
    Int2 pos;    // マップ上の位置
};

class CFieldItem {
protected:
    FieldItem m_FieldItem;

public:
    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Step() = 0;
    virtual void Draw() = 0;
    virtual void Exit() = 0;
public:
    void SetPos(Int2 pos) { m_FieldItem.pos = pos; }
    Int2 GetPos() { return m_FieldItem.pos; }

    virtual bool Use(Int2* plPos, std::vector<CObject*>& targets) = 0;
};

