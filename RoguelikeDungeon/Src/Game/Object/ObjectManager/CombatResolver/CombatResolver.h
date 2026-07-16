#pragma once
#include "../../Player/Player.h"

class CCombatResolver {
public:
	void PlayerAttack(CPlayer& player, CObject& target);

	void EnemyAttack(CPlayer& player, int damage);
};