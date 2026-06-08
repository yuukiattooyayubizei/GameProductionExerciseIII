#pragma once
#include <vector>
#include "Object.h"
#include "../Map/Map.h"

class CObjectManager
{
private:
    std::vector<CObject*> m_Object;

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

    // これは「deleteせずvectorだけ空にする」ので基本的にはあまり使わない方がいい
    void Clear()
    {
        m_Object.clear();
    }

    void Init();


    void Load();

    void Step(CanMove cmove, Int2 pos);
    
    void Draw();

    std::vector<CObject*> FindObjectsInSameRoom(Int2 pos, CMap& map);
    CObject* FindObjectAt(Int2 pos);
    ObjectKind GetKind(int id) const { return m_Object[id]->GetKind(); }

    //死んでいる敵の消去
    void DeleteDeadObject();
    void ClearEnemy();
    void ClearAll();

    //既に生成されているオブジェクトと座標が被っているかどうか
    int CollsionObject(const Int2& pos) const;

    //オブジェクト、アイテム、敵など全てと被っているかどうか
    bool CollsionAll(Int2 pos);

    //オブジェクトがスポーンする位置を探す
    Int2 FindSpawnPos();
    //敵生成
    void CreateEnemy(int CreateNum = 1);

};