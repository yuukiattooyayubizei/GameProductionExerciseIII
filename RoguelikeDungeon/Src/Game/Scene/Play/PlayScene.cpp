#include"PlayScene.h"
#include"math.h"
#include <iostream>
#include <vector>
#include<algorithm>
#include<list>
#include "../../Common.h"
#include "../../../Lib/Input/PadInput.h"
#include "../../../Lib/Sound/sound.h"
#include "../../Data/Data.h"
#include "../../../Lib/Input/input.h"

using namespace std;

//コンストラクタ
CPlayScene::CPlayScene()
{
	m_tagPlayScene = PLAY_SCENE_INIT;


	m_PlayerTurn = true;
}

//デストラクタ
CPlayScene::~CPlayScene()
{
	Exit();
}


void CPlayScene::Init()
{
	CData* Data = CData::GetInstance();
	CMap* Map = CMap::GetInstance();

	Data->Init();

	// 念のため前回の残りを消す
	m_ObjectManager.ClearAll();
	m_Player = nullptr;

	m_Player = new CPlayer();
	m_Player->Init();

	m_ObjectManager.AddObject(m_Player);

	m_EnemySpwanWait = 30;
	m_PlayMode = MODE_PLAY;
	m_Floor = 1;

	m_PlayerTurn = true;

	m_CameraManager.Init();
	m_CameraManager.SetNearFar(CAMERA_NEAR, CAMERA_FAR);
}


void CPlayScene::Exit()
{
	CMap* Map = CMap::GetInstance();

	m_ObjectManager.ClearAll();

	Map->DeleteAll();
	m_CameraManager.Exit();

	m_Player = nullptr;
}


void CPlayScene::CreatePlayer() {
	Int2 v = m_ObjectManager.FindSpawnPos();
	m_Player->SetPos(v);
}

void CPlayScene::Load()
{
	CMap* Map = CMap::GetInstance();
	CData* Data = CData::GetInstance();


	m_ObjectManager.Load();

	CreateFloor();



}

void CPlayScene::CreateFloor() {
	CMap* Map = CMap::GetInstance();
	CData* Data = CData::GetInstance();

	//プレイヤー以外のオブジェクトを削除
	m_ObjectManager.ClearEnemy();

	Data->Load();


	Map->CreateFloor();

	//プレイヤーを作成
	CreatePlayer();
	//敵を作成
	m_ObjectManager.CreateEnemy(STRAT_ENEMY_NUM);
}

int CPlayScene::Loop()
{
	
	int m_ret = 0;

	//状態遷移に応じて挙動を変更
	switch (m_tagPlayScene)
	{
	case CPlayScene::PLAY_SCENE_INIT:
		//初期化
		Init();
		m_tagPlayScene = PLAY_SCENE_LOAD;
		break;
	case CPlayScene::PLAY_SCENE_LOAD:
		//ロード
		Load();
		m_tagPlayScene = PLAY_SCENE_LOOP;
		//BGMを鳴らす
		RequestSound(BGMID_GAME, DX_PLAYTYPE_BACK);
		break;
	case CPlayScene::PLAY_SCENE_LOOP:
		//処理
		if (Step() != 0)m_tagPlayScene = PLAY_SCENE_END;
		break;
	case CPlayScene::PLAY_SCENE_END:
		//破棄
		Exit();
		StopAllSound();
		m_tagPlayScene = PLAY_SCENE_INIT;
		m_ret = 1;
		break;
	default:
		break;
	}

	return m_ret;
}


int CPlayScene::Step()
{
	CMap* Map = CMap::GetInstance();
	VECTOR plPos{};
	plPos.x = -m_Player->GetPos().x * 100;
	plPos.y = 0;
	plPos.z = m_Player->GetPos().y * 100;
	m_CameraManager.Step(plPos, 0, 0, 0);



	m_CameraManager.UpDate();

		if (m_PlayMode == MODE_PLAY)
		{
			return StepPlay();
		}
		else if (m_PlayMode == MODE_ITEM_MENU)
		{
			int i =  Map->StepItemMenu(m_Player->GetInventorySize());
			if(i == 1)
				m_PlayMode = MODE_PLAY;
			if(i >= 2)
			{
				if (UseItem(i - 2))
				{
					m_PlayMode = MODE_PLAY;
					m_PlayerTurn = false;
				}
			}
		}

	return 0;
}


bool CPlayScene::UseItem(int index)
{
	const auto& inventory = m_Player->GetInventory();

	if (index < 0 || index >= static_cast<int>(inventory.size()))
	{
		return false;
	}

	CMap* map = CMap::GetInstance();
	if (map == nullptr)
	{
		return false;
	}

	Item item = inventory[index];

	ItemUseContext context
	{
		*m_Player,
		*map,
		m_ObjectManager
	};

	if (!item.Use(context))
	{
		return false;
	}

	m_Player->EraseItem(index);

	return true;
}

int CPlayScene::StepPlay() {
	CData* Data = CData::GetInstance();
	CMap* Map = CMap::GetInstance();


	//プレイヤーの行動待ちなら
	if (m_PlayerTurn == true)
	{
		//アイテム選択に移行
		if (IsInputTrg(KEY_K))
		{
			m_PlayMode = MODE_ITEM_MENU;
			Map->SetSelectItemIndex();
			Map->SetItemPage();
			return 0;
		}
		//足踏みする(なにもしない)
		if (IsInputTrg(KEY_F))
		{
			m_PlayerTurn = false;
		}
		//デバッグ用
		if (IsInputTrg(KEY_Z))m_ObjectManager.CreateEnemy();
		
		//オブジェクトが動けるマスを探す
		CanMove C = m_ObjectManager.GetCanMove(m_Player->GetPos());

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
				int ObjectNum = m_ObjectManager.CollsionObject(NextPos);
				TILE NextTile = Map->GetTile(NextPos);
				if (ObjectNum == -1)
				{
					/*if (NextTile == TILE_ROOM || NextTile == TILE_CORRIDOR)*/
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
					CObject* target = m_ObjectManager.FindObjectAt(NextPos);

					if (target != nullptr)
					{
						// 敵なら攻撃する
						if (target->GetKind() == KIND_ENEMY)
						{
							int damage = m_Player->GetAtk();

							target->AddDamage(damage);

							std::cout << "敵に" << damage << "ダメージを与えた" << std::endl;

							if (target->GetHP() <= 0)
							{
								std::cout << "敵撃破" << std::endl;
								target->SetActive(false);
							}

							m_ObjectManager.DeleteDeadObject();

							// 敵がいたので移動はしない
							
						}
					}
				}

				m_PlayerTurn = false;

				//移動先のアイテムを検索
				Item item = {};
				item.type = Map->IsItemExist(m_Player->GetPos());

				//アイテムがあったら
				if (item.type != ITEM_NON)
				{
					//そのアイテムをインベントリに入れる
					if (m_Player->AddItem(item))
						//入れたアイテムを消す
						Map->EraseItem(m_Player->GetPos());
					else
						std::cout << "インベントリがまんたん" << std::endl;
				}
			}

		}


		//死んでる敵の消去
		m_ObjectManager.DeleteDeadObject();
		
	}
	//プレイヤー行動の後
	else
	{

		// プレイヤー行動の後
		for (CObject* object : m_ObjectManager.GetObjects())
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
				CanMove C = m_ObjectManager.GetCanMoveEnemy(object->GetPos());

				object->Step(C, m_Player->GetPos());

				// 敵が移動していたら
				if (object->GetDirection() != DIRECTION_NON)
				{
					Int2 move = DirectionToInt2(object->GetDirection());
					Int2 NextPos = AddInt2(object->GetPos(), move);

					// 移動先にオブジェクトがいるかチェック
					CObject* target = m_ObjectManager.FindObjectAt(NextPos);

					TILE NextTile = Map->GetTile(NextPos);

					if (target == nullptr)
					{
						if (NextTile == TILE_ROOM || NextTile == TILE_CORRIDOR)
						{
							// 何もいないなら移動
							object->AddPos(move);
						}
					}
					else
					{
						// 何かがいるなら攻撃
						if (target->GetKind() == KIND_PLAYER)
						{
							int damage = object->GetAtk();

							target->AddDamage(damage);

							std::cout << "プレイヤーは" << damage << "ダメージを受けた" << std::endl;

							if (target->GetHP() <= 0)
							{
								std::cout << "撃破された" << std::endl;
								target->SetActive(false);
							}
						}
					}
				}
			}
		}

		m_PlayerTurn = true;

		//敵を出す処理
		//敵をだすまでのカウントを下げる
		m_EnemySpwanWait--;
		if (m_EnemySpwanWait <= 0)
		{
			//0になったら敵を出してカウントをリセット
			m_EnemySpwanWait = 30;
			m_ObjectManager.CreateEnemy();
		}
	}



	if (CheckHitKey(KEY_INPUT_L))
		return 1;

	//プレイヤーが5階で階段に乗ったら終了
	if (CollsionInt2(m_Player->GetPos(), Map->GetStairsPos()) == true)
	{
		if (m_Floor >= 5)
			return 1;
		m_Floor++;
		CreateFloor();
	}
		

	//プレイヤーが死んだら終了
	if (m_Player->GetHP() <= 0)
		return 1;

	return 0;
}



void CPlayScene::Draw()
{
	CMap* Map = CMap::GetInstance();
	//プレイヤーがnullなら呼ばない
	if (m_Player == nullptr)return;


	CData* Data = CData::GetInstance();

	Map->Draw(m_Player->GetPos());


	m_ObjectManager.Draw();

	//描画処理

	DrawFormatString(32, 160, GetColor(255, 255, 255), "%d 階", m_Floor);

	DrawFormatString(32, 704, GetColor(255, 255, 255), "プレイシーン Fで足踏み、Kでアイテムメニュー");

	m_CameraManager.Draw();

	if (m_PlayMode == MODE_ITEM_MENU)
	{
		Map->DrawItemMenu(m_Player->GetInventory());
	}
}