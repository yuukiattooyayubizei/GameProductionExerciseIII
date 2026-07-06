#include"ObjectStore.h"
#include <string>
#include "../../UI/Log.h"

void CObjectStore::Init() {
    for (CObject* obj : m_Object)
    {
        if (obj != nullptr)
        {
            obj->Init();
        }
    }
}

void CObjectStore::Load() {
    for (CObject* obj : m_Object)
    {
        if (obj != nullptr)
        {
            obj->Load();
        }
    }
}

void CObjectStore::Draw() {
    for (CObject* obj : m_Object)
    {
        if (obj != nullptr)
        {
            obj->Draw();
        }
    }
}

void CObjectStore::AddObject(CObject* object)
{
    if (object != nullptr)
    {
        m_Object.push_back(object);
    }
}

CObject* CObjectStore::FindObjectAt(Int2 pos)const
{
    for (CObject* object : m_Object)
    {
        if (object == nullptr)
        {
            continue;
        }

        if (!object->GetActive())
        {
            continue;
        }

        if (object->GetPos().x == pos.x &&
            object->GetPos().y == pos.y)
        {
            return object;
        }
    }
    return nullptr;
}

ObjectKind CObjectStore::GetKind(int id)const {
    //オブジェクトの範囲外ならNONを返す
    if (id < 0 || id >= static_cast<int>(m_Object.size())) return KIND_NON;
    if (m_Object[id] == nullptr) return KIND_NON;

    return m_Object[id]->GetKind();
}

int CObjectStore::CollisionObject(const Int2& pos) const
{
    int ret = 0;

    for (CObject* obj : m_Object)
    {
        if (obj == nullptr)
        {
            ret++;
            continue;
        }

        if (!obj->GetActive())
        {
            ret++;
            continue;
        }

        if (obj->GetPos().x == pos.x && obj->GetPos().y == pos.y)
        {
            return ret;
        }

        ret++;
    }

    return -1;
}

void CObjectStore::ClearEnemy() {
    //プレイヤー以外のオブジェクトを削除
    auto newEnd = std::remove_if(
        m_Object.begin(),
        m_Object.end(),
        [](CObject* object)
        {
            if (object == nullptr)
            {
                return true;
            }

            if (object->GetKind() != KIND_PLAYER)
            {
                object->Exit();
                delete object;
                return true;
            }

            return false;
        }
    );

    m_Object.erase(newEnd, m_Object.end());
}

void CObjectStore::ClearAll()
{
    for (CObject* object : m_Object)
    {
        if (object == nullptr)
        {
            continue;
        }
        object->Exit();
        delete object;
    }
    m_Object.clear();
}

void CObjectStore::DeleteDeadObject(CObjectStore& objectStore, CPlayer* player)
{
    auto newEnd = std::remove_if(objectStore.GetObjects().begin(), objectStore.GetObjects().end(), [player](CObject* object) {
        CLog* Log = CLog::GetInstance();
        // プレイヤーはここでは削除しない
        if (object->GetKind() == KIND_PLAYER)
        {
            return false;
        }
        if (!object->GetActive())
        {
            if (object->GetHP() <= 0)
            {
                std::string rog = "敵撃破";
                Log->AddLog(rog);
                object->SetActive(false);
                //経験値の付与
                int exp = object->GetAddExp();
                player->AddExp(exp);
                rog = std::to_string(exp) + "の経験値を獲得";
                Log->AddLog(rog);
            }


            object->Exit();
            delete object;
            return true;
        }
        return false;
        }
    );

    objectStore.GetObjects().erase(newEnd, objectStore.GetObjects().end());
}