#include"ObjectManager.h"
#include<iostream>
#include"../../Scene/Play/PlayScene.h"
#include"../Enemy/Enemy1/Enemy1.h"
#include"../Enemy/Enemy2/Enemy2.h"
#include"../Enemy/Enemy3/Enemy3.h"
#include"../Enemy/Enemy4/Enemy4.h"
#include<algorithm>
#include"../../../Lib/Input/Input.h"
#include"../Player/Player.h"
#include "../../UI/Log.h"

void CObjectManager::Init() {
    for (CObject* obj : GetObjects())
    {
        if (obj != nullptr)
        {
            obj->Init();
        }
    }

    // 念のため前回の残りを消す
    ClearAll();
    //m_PlayerTurn = true;
    m_EnemySpawnWait = ENEMY_SPAWN_WAIT;
    m_Player = {};
    m_PlayMode = MODE_PLAY;
}

void CObjectManager::Load() {
    m_ObjectStore.Load();
}

PlayerAction CObjectManager::PlayerStep(int floor) {
    CMap* Map = CMap::GetInstance();
    CLog* Log = CLog::GetInstance();
    CInput* Input = CInput::GetInstance();
        //アイテム選択に移行
    if (Input->IsInputTrg(KEY_K))
    {
        m_PlayMode = MODE_ITEM_MENU;
        Map->SetSelectItemIndex();
        Map->SetItemPage();
        int InventorySize = m_Player->GetInventorySize();
        Map->UpDateItemMenu(InventorySize);
        return ACTION_ITEM_MENU;
    }
        //足踏みする(なにもしない)
    if (Input->IsInputTrg(KEY_F))
    {
        return ACTION_END;
    }

    //Zキーでランダム敵召喚
    if (Input->IsInputTrg(KEY_Z))
        CreateEnemy(floor);
    //Xキーでランダムアイテム入手
    if (Input->IsInputTrg(KEY_X))
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

    //全ての敵の行動したかをリセット
    for (CObject* object : GetObjects())
    {
        if (object->GetKind() == KIND_ENEMY)
        {
            object->SetIsActed(false);
        }
    }

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
            //既に行動しているなら動かさない
            if (object->GetIsActed())
            {
                continue;
            }

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
                    //その方向に移動可能なマスなら進む
                    if (NextTile == TILE_ROOM|| NextTile == TILE_CORRIDOR || NextTile == TILE_CORRIDOR_ADJACENT_ROOM)
                    {
                        object->AddPos(move);
                        object->SetIsActed(true);
                    }
                }
                else
                {
                    // プレイヤーがいる場合は攻撃
                    if (target->GetKind() == KIND_PLAYER)
                    {
                        CEnemy* enemy = static_cast<CEnemy*>(object);

                        m_CombatResolver.EnemyAttack(*m_Player, *enemy);
                    
                        object->SetIsActed(true);
                    }
                    // 敵がいる場合はこの敵と進むマスにいる敵の座標を交換
                    else if (target->GetKind() == KIND_ENEMY)
                    {
                        //// 移動先に敵がいるため、このターンは動かない
                        //object->SetDirection(DIRECTION_NON);

                        const Int2 currentPos = object->GetPos();
                        const Int2 targetPos = target->GetPos();

                        object->SetPos(targetPos);
                        target->SetPos(currentPos);

                        //交代した2体とも行動済みにする
                        object->SetIsActed(true);
                        target->SetIsActed(true);
                    }
                }
            }
            //移動できたいとき
            else
            {
                object->SetPos(object->GetPos());
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

    //プレイヤーかマップのどちらかがなかったらやめる
    if (m_Player == nullptr)return;

    CMap* Map = CMap::GetInstance();
    if (Map == nullptr)return;

    //プレイヤーの座標を取得
    Int2 PPos = m_Player->GetPos();


    for (CObject* obj : GetObjects()) {
        if (obj->GetKind() == KIND_PLAYER) {
            obj->Draw();
            continue;
        }

        if (Map->IsVisibleFrom(PPos, obj->GetPos()))
            obj->Draw();
    }
}