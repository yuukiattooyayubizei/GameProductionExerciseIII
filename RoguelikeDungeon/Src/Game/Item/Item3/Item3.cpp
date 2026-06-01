#include "Item3.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Object.h"
#include <iostream>

CItem3::CItem3()
{
    Init();
}

CItem3::~CItem3()
{
}

void CItem3::InitItemType()
{
    m_ItemType = ITEM_3;
}

const char* CItem3::GetName() const
{
    return "アイテム3";
}

bool CItem3::Use(CPlayer* player, const std::vector<CObject*>& targets)
{
    if (player == nullptr)
        return false;

    player->AddAtk(5);
    std::cout << "攻撃力5アップ" << std::endl;

    return true;
}
