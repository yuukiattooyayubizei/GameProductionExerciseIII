#pragma once
#include<vector>
#include"Object.h"
#include"../Map/Map.h"
#include"Enemy/EnemyModelManager.h"
#include"Enemy/Enemy.h"

class CObjectManager
{
private:
    static constexpr int ENEMY_SPWAN_WAIT = 30;
private:
    std::vector<CObject*> m_Object;
    CPlayer* m_Player = {};
    CEnemyModelManager m_EnemyModelManager;

    int m_EnemySpwanWait = ENEMY_SPWAN_WAIT;
    PlayMode m_PlayMode = MODE_PLAY;
private:
    void AddObject(CObject* object)
    {
        if (object != nullptr)
        {
            m_Object.push_back(object);
        }
    }

    const std::vector<CObject*>& GetObjects() const
    {
        return m_Object;
    }

    CObject* FindObjectAt(Int2 pos);
    ObjectKind GetKind(int id)const;

    //既に生成されているオブジェクトと座標が被っているかどうか
    int CollisionObject(const Int2& pos) const;

    //オブジェクト、アイテム、敵など全てと被っているかどうか
    bool CollisionAll(Int2 pos);

    //オブジェクトがスポーンする位置を探す
    Int2 FindSpawnPos();

    //敵の種類を決定
    CEnemy* CreateRandomEnemy(int floor);

    //もらった座標と同じ部屋にいるObjectを返す
    std::vector<CObject*> FindLiveTogetherObject(Int2 i);

    CanMove GetCanMove(Int2 pos);

    //敵の移動先決定
    CanMove GetCanMoveEnemy(Int2 pos);

    //移動先にObjectがいるかどうか
    ObjectKind GetAheadMoveObject(Int2 pos, DIRECTION dir);
public:

    std::vector<CObject*>& GetObjects()
    {
        return m_Object;
    }

    void Init();
    void Load();
    int Step(int floor);
    PlayerAction PlayerStep(int floor);
    void EnemyStep(int floor);
    void Draw();

    void CreatePlayer();
    void CreatePlayerPos();

    std::vector<CObject*> FindObjectsInSameRoom(Int2 pos, CMap& map);

    //死んでいる敵の消去
    void DeleteDeadObject();
    void ClearEnemy();
    void ClearAll();

    //敵生成
    void CreateEnemy(int floor, int CreateNum = 1);

    CPlayer* GetPlayer() { return m_Player; }

};