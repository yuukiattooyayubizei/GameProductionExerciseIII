#pragma once
#include"../Common.h"
#include"ItemUseContext.h"

// ƒAƒCƒeƒ€1ŒÂ•ª‚Ìî•ñ
struct Item
{
    ITEM_TYPE type;

    bool Use(ItemUseContext& context) const;
};

