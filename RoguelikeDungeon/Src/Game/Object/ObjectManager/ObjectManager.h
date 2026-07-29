#pragma once
#include<vector>
#include"../Object.h"
#include"../../Map/Map.h"
#include"../Enemy/EnemyModelManager.h"
#include"../Enemy/Enemy.h"
#include "MoveRule/MoveRule.h"
#include "CombatResolver/CombatResolver.h"
#include "ObjectSearch/ObjectSearch.h"
#include "ObjectSpawner/ObjectSpawner.h"
#include "ObjectStore/ObjectStore.h"

class CObjectManager
{
private:
    static constexpr int ENEMY_SPAWN_WAIT = 30;
private:
    CPlayer* m_Player = {};
    CEnemyModelManager m_EnemyModelManager;

    CMoveRule m_MoveRule;
    CCombatResolver m_CombatResolver;
    CObjectStore m_ObjectStore;
    CObjectSearch m_ObjectSearch;
    CObjectSpawner m_ObjectSpawner;

    int m_EnemySpawnWait = ENEMY_SPAWN_WAIT;

    PlayMode m_PlayMode = MODE_PLAY;
private:
    void AddObject(CObject* object){m_ObjectStore.AddObject(object);}

    CObject* FindObjectAt(Int2 pos) { return m_ObjectStore.FindObjectAt(pos); };
    ObjectKind GetKind(int id)const { return m_ObjectStore.GetKind(id); };

    //既に生成されているオブジェクトと座標が被っているかどうか
    int CollisionObject(const Int2& pos) const { return m_ObjectStore.CollisionObject(pos); };

    //オブジェクト、アイテム、敵など全てと被っているかどうか
    bool CollisionAll(Int2 pos) { return m_ObjectSpawner.CollisionAll(pos, m_ObjectStore); }

    //オブジェクトがスポーンする位置を探す
    Int2 FindSpawnPos() { return m_ObjectSpawner.FindSpawnPos(m_ObjectStore, m_Player); }

    //敵の種類を決定
    CEnemy* CreateRandomEnemy(int floor) {return m_ObjectSpawner.CreateRandomEnemy(floor, m_EnemyModelManager);}

    //もらった座標と同じ部屋にいるObjectを返す
    std::vector<CObject*> FindLiveTogetherObject(Int2 i) { return m_ObjectSearch.FindLiveTogetherObject(i,m_ObjectStore); }

    //移動できる方向を返す
    CanMove GetCanMove(Int2 pos) { return m_MoveRule.GetCanMove(pos); }

    //敵の移動先決定
    CanMove GetCanMoveEnemy(Int2 pos) { return m_MoveRule.GetCanMoveEnemy(pos, m_ObjectStore); }

    //移動先にObjectがいるかどうか
    ObjectKind GetAheadMoveObject(Int2 pos, DIRECTION dir) { return m_MoveRule.GetAheadMoveObject(pos,dir, m_ObjectStore); }
public:
    const std::vector<CObject*>& GetObjects() const {return m_ObjectStore.GetObjects();}
    std::vector<CObject*>& GetObjects(){return m_ObjectStore.GetObjects();}

    void Init();
    void Load();
    //int Step(int floor);
    PlayerAction PlayerStep(int floor);
    void EnemyStep(int floor);
    void Draw();

    void CreatePlayer() { m_ObjectSpawner.CreatePlayer(m_ObjectStore, m_Player); }
    void CreatePlayerPos(){ m_ObjectSpawner.CreatePlayerPos(m_ObjectStore, m_Player); }

    //指定された座標と同じ部屋にいるオブジェクトを返す
    std::vector<CObject*> FindObjectsInSameRoom(Int2 pos, CMap& map) { return m_ObjectSearch.FindObjectsInSameRoom(pos,map,m_ObjectStore); }
    //指定された座標から指定された方向を見るときに映るオブジェクトを返す
    //isClosestObjectがtrueなら、一番近い一つだけ返す
    std::vector<CObject*> FindObjectsInSeeDirection(Int2 pos, DIRECTION dir, CMap& map,bool isClosestObject) 
    { return m_ObjectSearch.FindObjectsInSeeDirection(pos, dir,map, isClosestObject, m_ObjectStore); }

    //死んでいる敵の消去
    void DeleteDeadObject() { m_ObjectStore.DeleteDeadObject(m_ObjectStore,m_Player); }
    void ClearEnemy() { m_ObjectStore.ClearEnemy(); }
    void ClearAll() { m_ObjectStore.ClearAll(); }

    //敵生成
    void CreateEnemy(int floor, int CreateNum = 1) { return m_ObjectSpawner.CreateEnemy(floor, m_ObjectStore, m_EnemyModelManager, m_Player,CreateNum); }
    CPlayer* GetPlayer() { return m_Player; }

};