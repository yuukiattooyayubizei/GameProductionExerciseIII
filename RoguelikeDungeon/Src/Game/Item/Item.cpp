#include "Item.h"

#include "../Object/Player/Player.h"
#include "../Object/ObjectManager.h"
#include "../Map/Map.h"

#include <iostream>
#include <vector>

bool Item::Use(ItemUseContext& context) const
{
    switch (type)
    {
    case ITEM_1:
        context.player.AddHeal(15);
        std::cout << "15回復" << std::endl;
        break;

    case ITEM_2:
        context.player.AddMaxHP(5);
        std::cout << "最大HP5アップ" << std::endl;
        break;

    case ITEM_3:
        context.player.AddAtk(5);
        std::cout << "攻撃力5アップ" << std::endl;
        break;

    case ITEM_4:
    {
        std::vector<CObject*> targets =
            context.objectManager.FindObjectsInSameRoom(
                context.player.GetPos(),
                context.map
            );

        for (CObject* object : targets)
        {
            if (object == nullptr)
            {
                continue;
            }

            object->AddDamage(5);
        }

        std::cout << "敵全体に5ダメージ" << std::endl;

        context.objectManager.DeleteDeadObject();
        break;
    }

    default:
        return false;
    }

    return true;
}