#pragma once
#include "../common.h"

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
