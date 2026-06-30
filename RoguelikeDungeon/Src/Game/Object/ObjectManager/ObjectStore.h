#pragma once
#include <vector>
#include "../Object.h"

class CObjectStore {
private:
	std::vector<CObject*> m_Object;
public:
    void Add(CObject* object);
    const std::vector<CObject*>& GetObjects() const { return m_Object; }
    std::vector<CObject*>& GetObjects(){return m_Object;}

    CObject* FindObjectAt(Int2 pos) const;
    ObjectKind GetKind(int id) const;
    int CollisionObject(const Int2& pos) const;

    void Load();
    void Draw();

    void ClearEnemy();
    void ClearAll();
};