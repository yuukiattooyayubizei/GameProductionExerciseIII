#include"ObjectManager.h"
#include<iostream>
#include"../Scene/Play/PlayScene.h"
#include"Enemy/Enemy1/Enemy1.h"
#include"Enemy/Enemy2/Enemy2.h"
#include"Enemy/Enemy3/Enemy3.h"
#include"Enemy/Enemy4/Enemy4.h"
#include<algorithm>
#include"../../Lib/Input/Input.h"
#include"Player/Player.h"
#include "../UI/Log.h"

<<<<<<< HEAD
void CObjectManager::Init() {
    m_ObjectStore.Init();
=======
void CObjectManager::DeleteDeadObject()
{
    auto newEnd = std::remove_if(GetObjects().begin(), GetObjects().end(),[this](CObject* object){
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
                    m_Player->AddExp(exp);
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

    //ClearEnemy();
    GetObjects().erase(newEnd, GetObjects().end());


}

std::vector<CObject*> CObjectManager::FindObjectsInSameRoom(Int2 pos,CMap& map){

    std::vector<CObject*> result;

    int roomNum = map.GetRoomNum(pos);

    if (roomNum == -1)
    {
        return result;
    }

    for (CObject* object : GetObjects())
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

//指定された座標から指定された方向を見るときに映るオブジェクトを返す
//isClosestObjectがtrueなら、一番近い一つだけ返す
std::vector<CObject*> CObjectManager::FindObjectsInSeeDirection(Int2 pos, DIRECTION dir, CMap& map, bool isClosestObject) {
    Int2 nextpos = pos;
    Int2 movepos = DirectionToInt2(dir);
    std::vector<CObject*> result;

    for (int i = 0;i < map.GetFieldOfVision(pos, dir);i++) {
        //次のマスを見る
        nextpos = AddInt2(nextpos, movepos);
        //次のマスにオブジェクトがいるか見る
        CObject* target = FindObjectAt(nextpos);

        //なにかいたら、そいつを返り値に加える
        if (target != nullptr)
        {
            result.push_back(target);

            //1体見つけて終わりなら、ここで終わる
            if (isClosestObject == true)return result;
        }
    }
    return result;
}

void CObjectManager::Init() {
    for (CObject* obj : GetObjects())
    {
        if (obj != nullptr)
        {
            obj->Init();
        }
    }
>>>>>>> e9bdf0b032d5c04ebb51621f16223fb007233716

    // 念のため前回の残りを消す
    ClearAll();
    //m_PlayerTurn = true;
    m_EnemySpawnWait = ENEMY_SPAWN_WAIT;
    m_Player = {};
    m_PlayMode = MODE_PLAY;
}

void CObjectManager::Load() {
<<<<<<< HEAD
    m_ObjectStore.Load();
=======
    for (CObject* obj : GetObjects())
    {
        if (obj != nullptr)
        {
            obj->Load();
        }
    }
>>>>>>> e9bdf0b032d5c04ebb51621f16223fb007233716
}

PlayerAction CObjectManager::PlayerStep(int floor) {
    CMap* Map = CMap::GetInstance();
    CLog* Log = CLog::GetInstance();

        //アイテム選択に移行
    if (IsInputTrg(KEY_K))
    {
        m_PlayMode = MODE_ITEM_MENU;
        Map->SetSelectItemIndex();
        Map->SetItemPage();
        int InventorySize = m_Player->GetInventorySize();
        Map->UpDateItemMenu(InventorySize);
        return ACTION_ITEM_MENU;
    }
        //足踏みする(なにもしない)
    if (IsInputTrg(KEY_F))
    {
        return ACTION_END;
    }

    //Zキーでランダム敵召喚
    if (IsInputTrg(KEY_Z))
        CreateEnemy(floor);
    //Xキーでランダムアイテム入手
    if (IsInputTrg(KEY_X))
    {
        int i = GetRand(ITEM_NUM);
        Item item = {};
        switch (i)
        {
        case 0:
            item.m_Type = ITEM_1;
            m_Player->AddItem(item);
            break;
        case 1:
            item.m_Type = ITEM_2;
            m_Player->AddItem(item);
            break;
        case 2:
            item.m_Type = ITEM_3;
            m_Player->AddItem(item);
            break;
        case 3:
            item.m_Type = ITEM_4;
            m_Player->AddItem(item);
            break;
        default:
            break;
        }
    }

    //オブジェクトが動けるマスを探す
    CanMove C = GetCanMove(m_Player->GetPos());

    //プレイヤーだけ動かす
    if (m_Player->GetKind() == KIND_PLAYER)
    {
        m_Player->Step(C, m_Player->GetPos());

        //プレイヤーが移動していたら
        if (m_Player->GetIsMove() == true)
        {
            Int2 move = DirectionToInt2(m_Player->GetDirection());
            Int2 NextPos = AddInt2(m_Player->GetPos(), move);
            //移動する方向にオブジェクトがいないかチェック
            int ObjectNum = CollisionObject(NextPos);
            TILE NextTile = Map->GetTile(NextPos);
            if (ObjectNum == -1)
            {
                if (NextTile == TILE_ROOM || NextTile == TILE_CORRIDOR || NextTile == TILE_CORRIDOR_ADJACENT_ROOM)
                {
                    //何もいないなら
                    //プレイヤーを移動させる
                    m_Player->AddPos(move);
                    m_Player->SetMove(false);
                }
            }
            else
            {
                // 何かがいるなら代わりにそいつに攻撃
                CObject* target = FindObjectAt(NextPos);

                m_CombatResolver.PlayerAttack(*m_Player,*target);

            }

            //移動先のアイテムを検索
            Item item = {};
            item.m_Type = Map->IsItemExist(m_Player->GetPos());

            //アイテムがあったら
            if (item.m_Type != ITEM_NON)
            {
                //そのアイテムをインベントリに入れる
                if (m_Player->AddItem(item))
                    //入れたアイテムを消す
                    Map->EraseItem(m_Player->GetPos());
                else {
                    std::string rog = "インベントリがまんたん";
                    Log->AddLog(rog);
                }
            }
            return ACTION_END;
        }

    }


    //死んでる敵の消去
    DeleteDeadObject();

   
    return ACTION_NON;
}

void CObjectManager::EnemyStep(int floor) {
    CMap* Map = CMap::GetInstance();
    CLog* Log = CLog::GetInstance();

    // プレイヤー行動の後
    for (CObject* object : GetObjects())
    {
        if (object == nullptr)
        {
            continue;
        }

        if (!object->GetActive())
        {
            continue;
        }

        // プレイヤー以外を動かす
        if (object->GetKind() != KIND_PLAYER)
        {
            // オブジェクトが動けるマスを探す
            CanMove C = GetCanMoveEnemy(object->GetPos());

            object->Step(C, m_Player->GetPos());

            // 敵が移動していたら
            if (object->GetDirection() != DIRECTION_NON)
            {
                Int2 move = DirectionToInt2(object->GetDirection());
                Int2 NextPos = AddInt2(object->GetPos(), move);

                // 移動先にオブジェクトがいるかチェック
                CObject* target = FindObjectAt(NextPos);

                TILE NextTile = Map->GetTile(NextPos);

                //移動先にオブジェクトがいない時
                if (target == nullptr)
                {
                    if (NextTile == TILE_ROOM || NextTile == TILE_CORRIDOR || NextTile == TILE_CORRIDOR_ADJACENT_ROOM)
                    {
                        // 何もいないなら移動
                        object->AddPos(move);
                    }
                }
                else
                {
                    // プレイヤーがいるなら攻撃
                    if (target->GetKind() == KIND_PLAYER)
                    {
                        int damage = object->GetAtk();

                        m_CombatResolver.EnemyAttack(*m_Player, damage);
                    }
                }
            }
        }
    }

    //敵を出す処理
    //敵を出すまでのカウントを下げる
    m_EnemySpawnWait--;
    if (m_EnemySpawnWait <= 0)
    {
        //0になったら敵を出してカウントをリセット
        m_EnemySpawnWait = ENEMY_SPAWN_WAIT;
        CreateEnemy(floor);
    }
    
    return;
}

void CObjectManager::Draw() {
<<<<<<< HEAD
    m_ObjectStore.Draw();
=======
    for (CObject* obj : GetObjects())
    {
        if (obj != nullptr)
        {
            obj->Draw();
        }
    }
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
        Int2 pos = Map->GetNotHerePlayerRoomPos(m_Player->GetPos());

        if (!CollisionAll(pos))
        {
            return pos;
        }
    }
    return { -1, -1 };
}

CEnemy* CObjectManager::CreateRandomEnemy(int floor) {

    int enemyType = GetRand(1);
    if (floor <= 3)
        ;
    else  if (floor <= 7)
        enemyType += 1;
    else
        enemyType += 2;

    switch (enemyType) {
    case 0:     return new CEnemy1(&m_EnemyModelManager);
    case 1:     return new CEnemy2(&m_EnemyModelManager);
    case 2:     return new CEnemy3(&m_EnemyModelManager);
    case 3:     return new CEnemy4(&m_EnemyModelManager);
    default:    return new CEnemy1(&m_EnemyModelManager);
    }
}

void CObjectManager::CreateEnemy(int floor, int CreateNum)
{
    for (int i = 0; i < CreateNum; i++)
    {
        Int2 pos = FindSpawnPos();

        CEnemy* enemy = CreateRandomEnemy(floor);

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

void CObjectManager::CreatePlayer() {

    m_Player = nullptr;

    m_Player = new CPlayer();
    m_Player->Init();

    m_Player->Load();

    AddObject(m_Player);
}

void CObjectManager::CreatePlayerPos() {
    Int2 v = FindSpawnPos();
    m_Player->SetPos(v);
>>>>>>> e9bdf0b032d5c04ebb51621f16223fb007233716
}