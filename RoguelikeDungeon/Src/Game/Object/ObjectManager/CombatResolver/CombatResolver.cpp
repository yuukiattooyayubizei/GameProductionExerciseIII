#include"CombatResolver.h"
#include "../../Object.h"
#include "../../../UI/Log.h"
#include <iostream>

void CCombatResolver::PlayerAttack(CPlayer& player, CObject& target) {
    CLog* Log = CLog::GetInstance();


    // 敵なら攻撃する
    if (target.GetKind() == KIND_ENEMY)
    {
        int damage = player.GetAtk();

        target.AddDamage(damage);
        //ダメージを与えたら長押しできるかの判定を一旦消す
        player.SetCanLongPress();

        std::string rog = "敵に" + std::to_string(damage) + "ダメージを与えた";
        Log->AddLog(rog);

        // 敵がいたので移動はしない

    }

}

void CCombatResolver::EnemyAttack(CPlayer& player, CEnemy& enemy) {
    CLog* Log = CLog::GetInstance();

    const int damage = enemy.GetAtk();

    player.AddDamage(damage);

    std::string rog = "プレイヤーは" + std::to_string(damage) + "ダメージを受けた";
    Log->AddLog(rog);
    //ダメージを受けたら長押しできるかの判定を一旦消す
    player.SetCanLongPress();

    //敵ごとの効果
    enemy.Attackeffect(player);

    if (player.GetHP() <= 0)
    {
        std::cout << "撃破された" << std::endl;
        player.SetActive(false);
    }
}