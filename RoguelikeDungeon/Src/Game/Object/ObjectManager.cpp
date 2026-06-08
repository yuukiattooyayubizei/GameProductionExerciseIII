#include"ObjectManager.h"

void CObjectManager::DeleteDeadObject()
{
    auto newEnd = std::remove_if(
        m_Object.begin(),
        m_Object.end(),
        [](CObject* object)
        {
            // プレイヤーはここでは削除しない
            if (object->GetKind() == KIND_PLAYER)
            {
                return false;
            }
            if (!object->GetActive())
            {
                delete object;
                return true;
            }


            return false;
        }
    );

    m_Object.erase(newEnd, m_Object.end());
}

std::vector<CObject*> CObjectManager::FindObjectsInSameRoom(Int2 pos,CMap& map){

    std::vector<CObject*> result;

    int roomNum = map.GetRoomNum(pos);

    if (roomNum == -1)
    {
        return result;
    }

    for (CObject* object : m_Object)
    {
        if (object == nullptr)
        {
            continue;
        }

        if (object->GetKind() == KIND_PLAYER)
        {
            continue;
        }

        if (map.GetRoomNum(object->GetPos()) == roomNum)
        {
            result.push_back(object);
        }
    }

    return result;
}

void CObjectManager::ClearEnemy() {
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

CObject* CObjectManager::FindObjectAt(Int2 pos)
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

void CObjectManager::ClearAll()
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

void CObjectManager::Init() {
    for (CObject* obj : m_Object)
    {
        if (obj != nullptr)
        {
            obj->Init();
        }
    }
}

void CObjectManager::Load() {
    for (CObject* obj : m_Object)
    {
        if (obj != nullptr)
        {
            obj->Load();
        }
    }
}

void CObjectManager::Step(CanMove cmove, Int2 pos) {
    for (CObject* obj : m_Object)
    {
        if (obj != nullptr)
        {
            obj->Step(cmove, pos);
        }
    }
}

void CObjectManager::Draw() {
    for (CObject* obj : m_Object)
    {
        if (obj != nullptr)
        {
            obj->Draw();
        }
    }
}