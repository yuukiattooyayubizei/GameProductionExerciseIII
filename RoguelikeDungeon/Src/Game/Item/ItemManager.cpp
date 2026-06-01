#include "ItemManager.h"
#include "Item1/Item1.h"
#include "Item2/Item2.h"
#include "Item3/Item3.h"
#include "Item4/Item4.h"
#include <DxLib.h>

CItem* CreateItem(ITEM_TYPE type)
{
    switch (type)
    {
    case ITEM_1:
        return new CItem1();
    case ITEM_2:
        return new CItem2();
    case ITEM_3:
        return new CItem3();
    case ITEM_4:
        return new CItem4();
    default:
        return nullptr;
    }
}

CItem* CreateRandomItem()
{
    ITEM_TYPE type = static_cast<ITEM_TYPE>(GetRand(ITEM_NUM - 1));
    return CreateItem(type);
}

const char* GetItemName(ITEM_TYPE type)
{
    CItem* item = CreateItem(type);

    if (item == nullptr)
        return "不明なアイテム";

    const char* name = item->GetName();
    delete item;
    return name;
}
