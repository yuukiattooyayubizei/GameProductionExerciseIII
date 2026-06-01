#include "Item4.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Object.h"
#include <iostream>

CItem4::CItem4()
{
    Init();
}

CItem4::~CItem4()
{
}

void CItem4::InitItemType()
{
    m_ItemType = ITEM_4;
}

const char* CItem4::GetName() const
{
    return "アイテム4";
}

bool CItem4::Use(CPlayer* player, const std::vector<CObject*>& targets)
{
    if (player == nullptr)
        return false;

    for (CObject* object : targets) {
        object->AddDamage(5);
    }
    std::cout << "敵全体に5ダメージ" << std::endl;

    return true;
}
