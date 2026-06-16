#pragma once
#include<vector>
#include"Object.h"
#include"../Map/Map.h"
#include"Enemy/EnemyModelManager.h"
#include"Enemy/Enemy.h"

class CObjectManager
{
private:
    std::vector<CObject*> m_Object;
    CPlayer* m_Player;
    CEnemyModelManager m_EnemyModelManager;

    bool m_PlayerTurn;
    int m_EnemySpwanWait;
    PlayMode m_PlayMode;
public:
    void AddObject(CObject* object)
    {
        if (object != nullptr)
        {
            m_Object.push_back(object);
        }
    }

    std::vector<CObject*>& GetObjects()
    {
        return m_Object;
    }

    const std::vector<CObject*>& GetObjects() const
    {
        return m_Object;
    }

    void Init();
    void Load();
    int Step();
    void Draw();

    void CreatePlayer();
    void CreatePlayerPos();

    std::vector<CObject*> FindObjectsInSameRoom(Int2 pos, CMap& map);
    CObject* FindObjectAt(Int2 pos);
    ObjectKind GetKind(int id)const;

    //死んでいる敵の消去
    void DeleteDeadObject();
    void ClearEnemy();
    void ClearAll();

    //既に生成されているオブジェクトと座標が被っているかどうか
    int CollisionObject(const Int2& pos) const;

    //オブジェクト、アイテム、敵など全てと被っているかどうか
    bool CollisionAll(Int2 pos);

    //オブジェクトがスポーンする位置を探す
    Int2 FindSpawnPos();

    //敵の種類を決定
    CEnemy* CreateRandomEnemy();

    //敵生成
    void CreateEnemy(int CreateNum = 1);

    //もらった座標と同じ部屋にいるObjectを返す
    std::vector<CObject*> FindLiveTogetherObject(Int2 i);

    CanMove GetCanMove(Int2 pos);

    //敵の移動先決定
    CanMove GetCanMoveEnemy(Int2 pos);

    //移動先にObjectがいるかどうか
    ObjectKind GetAheadMoveObject(Int2 pos, DIRECTION dir);

    CPlayer* GetPlayer() { return m_Player; }

};