#pragma once
#include <vector>
#include "../../Object.h"
#include "../../Player/Player.h"

class CObjectStore {
private:
	std::vector<CObject*> m_Object;
public:
    void AddObject(CObject* object);

    std::vector<CObject*>& GetObjects() { return m_Object; }
    const std::vector<CObject*>& GetObjects() const { return m_Object; }


    CObject* FindObjectAt(Int2 pos) const;
    ObjectKind GetKind(int id) const;
    int CollisionObject(const Int2& pos) const;

    //éÄÇÒÇ≈Ç¢ÇÈìGÇÃè¡ãé
    void DeleteDeadObject(CObjectStore& objectStore, CPlayer* player);

    void Init();
    void Load();
    void Draw();

    void ClearEnemy();
    void ClearAll();
};