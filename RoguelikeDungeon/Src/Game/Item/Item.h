#pragma once
#include "../Common.h"
#include"../Object/Object.h"
#include <vector>
#include "ItemModelManager.h"
#include"ItemUseContext.h"


// ƒAƒCƒeƒ€1ŒÂ•ª‚Ìî•ñ
struct Item
{
    ITEM_TYPE type;

    bool Use(ItemUseContext& context) const;
};

