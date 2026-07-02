#include "ObjectSpawner.h"
#include "../ObjectManager.h"
#include <iostream>
#include"../Enemy/Enemy1/Enemy1.h"
#include"../Enemy/Enemy2/Enemy2.h"
#include"../Enemy/Enemy3/Enemy3.h"
#include"../Enemy/Enemy4/Enemy4.h"

void CObjectSpawner::CreatePlayer(CObjectStore& objectStore, CPlayer* player) {

    player = nullptr;

    player = new CPlayer();
    player->Init();

    player->Load();

    objectStore.AddObject(player);
}

void CObjectSpawner::CreatePlayerPos(CObjectStore& objectStore, CPlayer* player) {
    Int2 v = FindSpawnPos(objectStore,player);
    player->SetPos(v);
}

void CObjectSpawner::CreateEnemy(int floor, CObjectStore& objectStore, CEnemyModelManager& enemyModelManager, CPlayer* player, int CreateNum)
{
    for (int i = 0; i < CreateNum; i++)
    {
        Int2 pos = FindSpawnPos(objectStore, player);

        CEnemy* enemy = CreateRandomEnemy(floor, enemyModelManager);

        enemy->Init();
        enemy->SetPos(pos);

        std::cout << pos.x << "," << pos.y << "‚É“G‚ð¶¬" << std::endl;

        objectStore.AddObject(enemy);
    }
}

CEnemy* CObjectSpawner::CreateRandomEnemy(int floor, CEnemyModelManager& enemyModelManager) {

    int enemyType = GetRand(1);
    if (floor <= 3)
        ;
    else  if (floor <= 7)
        enemyType += 1;
    else
        enemyType += 2;

    switch (enemyType) {
    case 0:     return new CEnemy1(&enemyModelManager);
    case 1:     return new CEnemy2(&enemyModelManager);
    case 2:     return new CEnemy3(&enemyModelManager);
    case 3:     return new CEnemy4(&enemyModelManager);
    default:    return new CEnemy1(&enemyModelManager);
    }
}

bool CObjectSpawner::CollisionAll(Int2 pos, CObjectStore& objectStore)
{
    CMap* Map = CMap::GetInstance();
    if (objectStore.CollisionObject(pos) != -1)return true;
    if (Map->CollisionItem(pos) == true)return true;
    if (Map->CollisionStairs(pos) == true)return true;

    return false;
}

Int2 CObjectSpawner::FindSpawnPos(CObjectStore& objectStore, CPlayer* player)
{
    CMap* Map = CMap::GetInstance();
    for (int i = 0; i < RETRY_MAX; ++i)
    {
        Int2 pos = Map->GetNotHerePlayerRoomPos(player->GetPos());

        if (!CollisionAll(pos, objectStore))
        {
            return pos;
        }
    }
    return { -1, -1 };
}