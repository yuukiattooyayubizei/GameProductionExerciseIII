#pragma once
#include"../Object.h"
#include"../../Item/Item.h"
#include<vector>

static constexpr int HP_MAX = 15;               //初期の体力最大値
static constexpr int INVENTORY_MAX = 20;        //所持できるアイテム数の限界

class CPlayer : public CObject {
private:
    std::vector<Item> m_Inventory;
    //移動するかどうか
    bool m_IsMove = true;

    //足踏みしたかどうか
    bool m_IsStomping = false;

    int m_hndl = -1;

    int m_Exp = 0;
    int m_NextNecessaryExp = 0;
    int m_Lv = 1;

    int m_MoveLongPress = 0;        //長押ししたフレーム数
    bool m_CanLongPress = true;     //長押しで反応するか

    float m_heal;                   //回復
public:
    ~CPlayer()override;

    void Init()override;
    void Load()override;
    void Step(CanMove canmove, Int2 playerPos)override;
    void Draw()override;
    void Exit()override;
private:
    //インベントリが埋まっているかそうか
    bool IsInventoryFull() const;

    //デバッグ用の表示
    void DrawInventoryDebug();
public:
    //アイテム追加
    bool AddItem(const Item& item);

    //経験値追加
    void AddExp(int expnum) { m_Exp += expnum; }

    //インベントリの閲覧
    const std::vector<Item>& GetInventory() const { return m_Inventory; }

    bool EraseItem(int index);

    int GetInventorySize() const { return static_cast<int>(m_Inventory.size()); }
    bool GetIsMove() const { return m_IsMove; }

    void SetMove(bool is) { m_IsMove = is; }
    void SetCanLongPress(bool is = false) { m_CanLongPress = is; }

    void LvUp();

    //自然回復の処理
    void Heal();
};
