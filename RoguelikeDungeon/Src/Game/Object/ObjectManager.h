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

    void DeleteDeadObject();
    void ClearEnemy();
    void ClearAll();
};