#include"Item.h"
#include"../Object/Player/Player.h"
#include"../Object/ObjectManager/ObjectManager.h"
#include <iostream>
#include <vector>
#include "../UI/Log.h"

bool Item::Use(ItemUseContext& context) const
{

    //使用されるアイテムによって挙動を変える
    switch (m_Type)
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
    {
        std::vector<CObject*> targets =
            context.objectManager.FindObjectsInSeeDirection(context.player.GetPos(), context.player.GetDirection(), context.map, false);

        for (CObject* object : targets)
        {
            if (object == nullptr)
            {
                continue;
            }

            object->AddDamage(20);


        }

        std::cout << "視界内の敵に20ダメージ" << std::endl;

        context.objectManager.DeleteDeadObject();
        break;
    }

    case ITEM_4:
    {
        std::vector<CObject*> targets =
            context.objectManager.FindObjectsInSameRoom(context.player.GetPos(),context.map);

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