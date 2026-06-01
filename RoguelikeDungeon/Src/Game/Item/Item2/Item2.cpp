#include "Item2.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Object.h"
#include <iostream>

CItem2::CItem2()
{
    Init();
}

CItem2::~CItem2()
{
}

void CItem2::InitItemType()
{
    m_ItemType = ITEM_2;
}

const char* CItem2::GetName() const
{
    return "アイテム2";
}

bool CItem2::Use(CPlayer* player, const std::vector<CObject*>& targets)
{
    if (player == nullptr)
        return false;

    player->AddMaxHP(5);
    std::cout << "最大HP5アップ" << std::endl;

    return true;
}
