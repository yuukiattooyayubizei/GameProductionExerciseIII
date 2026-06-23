#pragma once
#include"../Common.h"
#include"ItemUseContext.h"
#include<string>

// ƒAƒCƒeƒ€1ŒÂ•ª‚Ìî•ñ
struct Item
{
    ITEM_TYPE m_Type;
    std::string m_Name;

    bool Use(ItemUseContext& context) const;
};

