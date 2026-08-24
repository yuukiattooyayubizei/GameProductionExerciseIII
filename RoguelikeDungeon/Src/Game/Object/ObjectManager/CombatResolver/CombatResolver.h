#pragma once
#include "../../Player/Player.h"
#include "../../Enemy/Enemy.h"

class CCombatResolver {
public:
	void PlayerAttack(CPlayer& player, CObject& target);

	void EnemyAttack(CPlayer& player, CEnemy& enemy);
};