#pragma once
#include"../Object.h"
#include <DxLib.h>
#include "../../Common.h"
#include "../../Item/Item.h"
#include <vector>

static constexpr int HP_MAX = 15;               //初期の体力最大値
static constexpr int INVENTORY_MAX = 20;        //所持できるアイテム数の限界

class CPlayer : public CObject {
private:
    std::vector<Item> m_Inventory;
public:
    ~CPlayer()override;

    void Init()override;
    void Load()override;
    void Step(CanMove canmove)override;
    void Draw()override;
    void Exit()override;
public:
    //アイテム追加
    bool AddItem(const Item& item);

    //インベントリが埋まっているかそうか
    bool IsInventoryFull() const;

    //インベントリの閲覧
    const std::vector<Item>& GetInventory() const { return m_Inventory; }

    //デバッグ用の表示
    void DrawInventoryDebug();
};
