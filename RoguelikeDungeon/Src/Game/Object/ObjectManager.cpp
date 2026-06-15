#include"ObjectManager.h"
#include <iostream>
#include "../Scene/Play/PlayScene.h"
#include"Enemy/Enemy1/Enemy1.h"
#include"Enemy/Enemy2/Enemy2.h"
#include"Enemy/Enemy3/Enemy3.h"
#include"Enemy/Enemy4/Enemy4.h"
#include <algorithm>

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
                object->Exit();
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

int CObjectManager::CollisionObject(const Int2& pos) const
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

bool CObjectManager::CollisionAll(Int2 pos)
{
    CMap* Map = CMap::GetInstance();
    if (CollisionObject(pos) != -1)return true;
    if (Map->CollisionItem(pos) == true)return true;
    if (Map->CollisionStairs(pos) == true)return true;

    return false;
}

Int2 CObjectManager::FindSpawnPos()
{
    CMap* Map = CMap::GetInstance();
    for (int i = 0; i < RETRY_MAX; ++i)
    {
        Int2 pos = Map->GetRoomPos();

        if (!CollisionAll(pos))
        {
            return pos;
        }
    }
    return { -1, -1 };
}

CEnemy* CObjectManager::CreateRandomEnemy() {
    int enemyType = GetRand(3);

    switch (enemyType) {
    case 0:
        return new CEnemy1(&m_EnemyModelManager);

    case 1:
        return new CEnemy2(&m_EnemyModelManager);
    case 2:
        return new CEnemy3(&m_EnemyModelManager);

    case 3:
        return new CEnemy4(&m_EnemyModelManager);

    default:
        return new CEnemy1(&m_EnemyModelManager);
    }
}

void CObjectManager::CreateEnemy(int CreateNum)
{
    for (int i = 0; i < CreateNum; i++)
    {
        Int2 pos = FindSpawnPos();

        CEnemy* enemy = CreateRandomEnemy();

        enemy->Init();
        enemy->SetPos(pos);

        std::cout << pos.x << "," << pos.y << "に敵を生成" << std::endl;

        AddObject(enemy);
    }
}

//プレイヤーと同じ部屋にいるObjectを返す
std::vector<CObject*> CObjectManager::FindLiveTogetherObject(Int2 i) {
    CMap* Map = CMap::GetInstance();
    std::vector<CObject*> res;

    //プレイヤーの部屋番号を取得
    int PlayerRoomNum = Map->GetRoomNum(i);
    //-1(部屋にいない)の場合終了
    if (PlayerRoomNum == -1)return {};


    for (CObject* object : GetObjects()) {
        if (object->GetKind() == KIND_PLAYER) {
            continue;
        }

        if (Map->GetRoomNum(object->GetPos()) == PlayerRoomNum) {
            res.push_back(object);
        }
    }

    return res;
}

CanMove CObjectManager::GetCanMove(Int2 pos)
{
    CMap* Map = CMap::GetInstance();
    Int2 v = pos;
    CanMove C;
    Int2 NextPos{};
    NextPos.x = static_cast<int>(v.x);
    NextPos.y = static_cast<int>(v.y);

    //一旦全部trueに
    C.Down = true, C.Up = true, C.Left = true, C.Right = true;
    //マス目の端だとマスの外側の方向には行けない
    if (NextPos.x <= 0)
        C.Left = false;
    if (NextPos.x >= MAP_X - 1)
        C.Right = false;
    if (NextPos.y <= 0)
        C.Up = false;
    if (NextPos.y >= MAP_Y - 1)
        C.Down = false;



    //上下左右のマスを見て通れるマスでなければ行けない
    //すでにfalseなら見る必要がない
    TILE t = {};
    if (C.Left == true)
    {
        NextPos.x--;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Left = false;
        NextPos.x++;
    }
    if (C.Right == true)
    {
        NextPos.x++;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Right = false;
        NextPos.x--;
    }
    if (C.Up == true)
    {
        NextPos.y--;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Up = false;
        NextPos.y++;
    }
    if (C.Down == true)
    {
        NextPos.y++;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Down = false;
        NextPos.y--;
    }

    return C;
}

CanMove CObjectManager::GetCanMoveEnemy(Int2 pos)
{
    CMap* Map = CMap::GetInstance();
    Int2 v = pos;
    CanMove C;
    Int2 NextPos{};
    NextPos.x = static_cast<int>(v.x);
    NextPos.y = static_cast<int>(v.y);

    //一旦全部trueに
    C.Down = true, C.Up = true, C.Left = true, C.Right = true;
    //マス目の端だとマスの外側の方向には行けない
    if (NextPos.x <= 0)
        C.Left = false;
    if (NextPos.x >= MAP_X - 1)
        C.Right = false;
    if (NextPos.y <= 0)
        C.Up = false;
    if (NextPos.y >= MAP_Y - 1)
        C.Down = false;



    //上下左右のマスを見て通れるマスでなければ行けない
    //すでにfalseなら見る必要がない
    TILE t = {};
    if (C.Left == true)
    {
        NextPos.x--;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Left = false;


        NextPos.x++;
        if (GetAheadMoveObject(NextPos, DIRECTION_LEFT) == KIND_ENEMY)
            C.Left = false;
    }
    if (C.Right == true)
    {
        NextPos.x++;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Right = false;
        NextPos.x--;
        if (GetAheadMoveObject(NextPos, DIRECTION_RIGHT) == KIND_ENEMY)
            C.Right = false;
    }
    if (C.Up == true)
    {
        NextPos.y--;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Up = false;
        NextPos.y++;
        if (GetAheadMoveObject(NextPos, DIRECTION_UP) == KIND_ENEMY)
            C.Up = false;
    }
    if (C.Down == true)
    {
        NextPos.y++;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Down = false;
        NextPos.y--;
        if (GetAheadMoveObject(NextPos, DIRECTION_DOWN) == KIND_ENEMY)
            C.Down = false;
    }

    return C;
}

ObjectKind CObjectManager::GetAheadMoveObject(Int2 pos, DIRECTION dir)
{
    ObjectKind ret = KIND_NON;
    Int2 p = pos;
    int id = -1;

    switch (dir)
    {
    case DIRECTION_UP:
        p.y--;
        break;
    case DIRECTION_DOWN:
        p.y++;
        break;
    case DIRECTION_LEFT:
        p.x--;
        break;
    case DIRECTION_RIGHT:
        p.x++;
        break;
    default:
        return KIND_NON;
    }

    id = CollisionObject(p);

    if (id != -1)
    {
        ret = GetKind(id);
    }

    return ret;
}

ObjectKind CObjectManager::GetKind(int id)const {
    //オブジェクトの範囲外ならNONを返す
    if (id < 0 || id >= static_cast<int>(m_Object.size())) return KIND_NON;
    if (m_Object[id] == nullptr) return KIND_NON;
    
    return m_Object[id]->GetKind();
}