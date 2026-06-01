#include "Item1.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Object.h"
#include <iostream>

CItem1::CItem1()
{
    Init();
}

CItem1::~CItem1()
{
}

void CItem1::InitItemType()
{
    m_ItemType = ITEM_1;
}

const char* CItem1::GetName() const
{
    return "アイテム1";
}

bool CItem1::Use(CPlayer* player, const std::vector<CObject*>& targets)
{
    if (player == nullptr)
        return false;

    player->AddHeal(15);
    std::cout << "15回復" << std::endl;

    return true;
}
