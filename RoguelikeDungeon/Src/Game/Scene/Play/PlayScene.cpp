#include"PlayScene.h"
#include"math.h"
#include <iostream>
#include <vector>
#include<algorithm>
#include<list>
#include "../../Common.h"
#include "../../../Lib/Input/PadInput.h"
#include "../../../Lib/Sound/sound.h"
#include "../../../Lib/Input/input.h"

using namespace std;

//コンストラクタ
CPlayScene::CPlayScene()
{
	m_tagPlayScene = PLAY_SCENE_INIT;

	m_EnemySpwanWait = 0;
	m_PlayMode = MODE_PLAY;
	m_Floor = 1;
	m_PlayerTurn = true;
	Init();
}

//デストラクタ
CPlayScene::~CPlayScene()
{
	Exit();
}


void CPlayScene::Init()
{
	CMap* Map = CMap::GetInstance();

	Map->Init();

	// 念のため前回の残りを消す
	m_ObjectManager.ClearAll();

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
}

void CPlayScene::Load()
{
	m_ObjectManager.Load();

	CMap* Map = CMap::GetInstance();
	Map->Load();

	//プレイヤーを作成
	m_ObjectManager.CreatePlayer();

	CreateFloor();
}

void CPlayScene::CreateFloor() {
	CMap* Map = CMap::GetInstance();
	//プレイヤー以外のオブジェクトを削除
	m_ObjectManager.ClearEnemy();
	Map->CreateFloor();
	//敵を作成
	m_ObjectManager.CreateEnemy(STRAT_ENEMY_NUM);

	//プレイヤーを作成
	m_ObjectManager.CreatePlayerPos();
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
	plPos.x = -(float)m_ObjectManager.GetPlayer()->GetPos().x * 100.0f;
	plPos.y = 0;
	plPos.z = (float)m_ObjectManager.GetPlayer()->GetPos().y * 100.0f;
	m_CameraManager.Step(plPos, 0, 0, 0);



	m_CameraManager.UpDate();

		if (m_PlayMode == MODE_PLAY)
		{
			return StepPlay();
		}
		else if (m_PlayMode == MODE_ITEM_MENU)
		{
			int i =  Map->StepItemMenu(m_ObjectManager.GetPlayer()->GetInventorySize());
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
	const auto& inventory = m_ObjectManager.GetPlayer()->GetInventory();

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
		*m_ObjectManager.GetPlayer(),
		*map,
		m_ObjectManager
	};

	if (!item.Use(context))
	{
		return false;
	}

	m_ObjectManager.GetPlayer()->EraseItem(index);

	return true;
}

int CPlayScene::StepPlay() {
	CMap* Map = CMap::GetInstance();
	if (m_ObjectManager.Step() == 1) {
		m_PlayMode = MODE_ITEM_MENU;
		Map->SetSelectItemIndex();
		Map->SetItemPage();
		int InventorySize = m_ObjectManager.GetPlayer()->GetInventorySize();
		Map->UpDateItemMenu(InventorySize);
		return 0;
	}

	if (CheckHitKey(KEY_INPUT_L))
		return 1;

	//プレイヤーが5階で階段に乗ったら終了
	if (CollsionInt2(m_ObjectManager.GetPlayer()->GetPos(), Map->GetStairsPos()) == true)
	{
		if (m_Floor >= 5)
			return 1;
		m_Floor++;
		CreateFloor();
	}
		

	//プレイヤーが死んだら終了
	if (m_ObjectManager.GetPlayer()->GetHP() <= 0)
		return 1;

	return 0;
}


void CPlayScene::Draw()
{
	CMap* Map = CMap::GetInstance();
	//プレイヤーがnullなら呼ばない
	if (m_ObjectManager.GetPlayer() == nullptr)return;

	Map->Draw(m_ObjectManager.GetPlayer()->GetPos());


	m_ObjectManager.Draw();

	//描画処理

	DrawFormatString(32, 160, GetColor(255, 255, 255), "%d 階", m_Floor);

	DrawFormatString(32, 704, GetColor(255, 255, 255), "プレイシーン Fで足踏み、Kでアイテムメニュー");

	m_CameraManager.Draw();

	if (m_PlayMode == MODE_ITEM_MENU)
	{
		Map->DrawItemMenu(m_ObjectManager.GetPlayer()->GetInventory());
	}
}