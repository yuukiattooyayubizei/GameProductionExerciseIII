#pragma once
#include"../../Common.h"
#include"../Item.h"
#include"../ItemModelManager.h"

// マップ上に落ちているアイテム
struct FieldItem
{
    Item item;   // アイテム本体
    Int2 pos;    // マップ上の位置
};

class CFieldItem
{
private:
    FieldItem m_FItem;

public:
    CFieldItem();

    ITEM_TYPE GetType() const { return m_FItem.item.m_Type; }
    Int2 GetPos() const { return m_FItem.pos; }

    void SetType(ITEM_TYPE type) { m_FItem.item.m_Type = type; }
    void SetPos(Int2 pos) { m_FItem.pos = pos; }

    void Draw(const CItemModelManager& modelManager) const
    {
        int hndl = modelManager.GetModel(static_cast<int>(m_FItem.item.m_Type));
        if (hndl == -1) return;

        MV1SetPosition(
            hndl,
            VGet(-m_FItem.pos.x * TILE_SIZE, 151, m_FItem.pos.y * TILE_SIZE)
        );

        MV1DrawModel(hndl);
    }
};