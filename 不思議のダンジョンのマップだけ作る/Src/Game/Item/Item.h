#pragma once
#include "../Common.h"

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
private:
    FieldItem m_FieldItem;
public:
    void SetPos(int x, int y) { m_FieldItem.pos.x = x;m_FieldItem.pos.y = y; }
    Int2 GetPos() { return m_FieldItem.pos; }
};