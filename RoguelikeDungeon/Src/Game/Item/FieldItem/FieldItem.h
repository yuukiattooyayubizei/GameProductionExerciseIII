#pragma once
#include "../../common.h"
#include "../Item.h"

// マップ上に落ちているアイテム
struct FieldItem
{
    Item item;   // アイテム本体
    Int2 pos;    // マップ上の位置
};


class CFieldItem
{
private:
    ITEM_TYPE m_Type;
    Int2 m_Pos;

public:
    CFieldItem(ITEM_TYPE type, Int2 pos)
        : m_Type(type), m_Pos(pos)
    {
    }

    ITEM_TYPE GetType() const { return m_Type; }
    Int2 GetPos() const { return m_Pos; }

    void Draw(const CItemModelManager& modelManager) const
    {
        int hndl = modelManager.GetModel(static_cast<int>(m_Type));
        if (hndl == -1) return;

        MV1SetPosition(
            hndl,
            VGet(-m_Pos.x * TILE_SIZE, 151, m_Pos.y * TILE_SIZE)
        );

        MV1DrawModel(hndl);
    }
};