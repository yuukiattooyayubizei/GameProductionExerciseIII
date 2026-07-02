#pragma once
#include "../Enemy/Enemy.h"
#include"CObjectStore.h"
#include"../Player/Player.h"

class CObjectSpawner {
public:
	void CreatePlayer(CObjectStore& objectStore, CPlayer* player);

	void CreatePlayerPos(CObjectStore& objectStore, CPlayer* player);

	//敵の種類を決定
	CEnemy* CreateRandomEnemy(int floor, CEnemyModelManager& enemyModelManager);

	//敵生成
	void CreateEnemy(int floor, CObjectStore& objectStore, CEnemyModelManager& enemyModelManager, CPlayer* player, int CreateNum = 1);

	//オブジェクト、アイテム、敵など全てと被っているかどうか
	bool CollisionAll(Int2 pos, CObjectStore& objectStore);

	//オブジェクトがスポーンする位置を探す
	Int2 FindSpawnPos(CObjectStore& objectStore, CPlayer* player);
};