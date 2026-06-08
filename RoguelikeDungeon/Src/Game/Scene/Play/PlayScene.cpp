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
#include "../../Object/Enemy/Enemy.h"
#include "../../Object/Enemy/Enemy1/Enemy1.h"
#include "../../Object/Enemy/Enemy2/Enemy2.h"
#include "../../Object/Enemy/Enemy3/Enemy3.h"
#include "../../Object/Enemy/Enemy4/Enemy4.h"
#include "../../../Lib/Input/input.h"


using namespace std;

Int2 CPlayScene::FindSpawnPos()
{
	CMap* Map = CMap::GetInstance();
	for (int i = 0; i < RETRY_MAX; ++i)
	{
		Int2 pos = Map->GetRoomPos();

		if (!CollsionAll(pos))
		{
			return pos;
		}
	}
	return { -1, -1 };
}

int CPlayScene::CollsionObject(const Int2& pos) const
{
	int ret = 0;

	for (CObject* obj : m_ObjectManager.GetObjects())
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

CanMove CPlayScene::GetCanMove(Int2 pos)
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

CanMove CPlayScene::GetCanMoveEnemy(Int2 pos)
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
		if (GetAheadMoveObject(NextPos,DIRECTION_LEFT) == KIND_ENEMY)
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

bool CPlayScene::CollsionAll(Int2 pos) 
{
	CMap* Map = CMap::GetInstance();
	if (CollsionObject(pos) != -1)return true;
	if (Map->CollisionItem(pos) == true)return true;
	if (Map->CollisionStairs(pos) == true)return true;

	return false;
}

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

	m_SelectItemIndex = 0;
	m_ItemPage = 0;

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

CEnemy* CPlayScene::CreateRandomEnemy() {
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


void CPlayScene::CreateEnemy(int CreateNum)
{
	for (int i = 0; i < CreateNum; i++)
	{
		Int2 pos = FindSpawnPos();

		CEnemy* enemy = CreateRandomEnemy();

		enemy->Init();
		enemy->SetPos(pos);

		std::cout << pos.x << "," << pos.y << "に敵を生成" << std::endl;

		m_ObjectManager.AddObject(enemy);
	}
}

void CPlayScene::CreatePlayer() {
	Int2 v = FindSpawnPos();
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

	//マップを消去
	Map->Init();
	//プレイヤー以外のオブジェクトを削除
	m_ObjectManager.ClearEnemy();

	Data->Load();

	Map->Load();

	//3個から5個の部屋を作成
	if (Map->CreateRoom(GetRand(ROOM_MAX - ROOM_MIN) + 3) == false)return;
	Map->CreateCorridor();
	Map->CreateStairs();
	Map->CreateItem(STRAT_ITEM_NUM);

	//プレイヤーを作成
	CreatePlayer();
	//敵を作成
	CreateEnemy(STRAT_ENEMY_NUM);
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

ObjectKind CPlayScene::GetAheadMoveObject(Int2 pos, DIRECTION dir)
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

	id = CollsionObject(p);

	if (id != -1)
	{
		ret = m_ObjectManager.GetKind(id);
	}

	return ret;
}

int CPlayScene::Step()
{
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
			return StepItemMenu();
		}

	return 0;
}

int CPlayScene::StepItemMenu()
{
	int itemCount = m_Player->GetInventorySize();

	if (IsInputTrg(KEY_J) || IsInputTrg(KEY_K))
	{
		m_PlayMode = MODE_PLAY;
		return 0;
	}

	if (itemCount <= 0)
	{
		m_SelectItemIndex = 0;
		m_ItemPage = 0;
		return 0;
	}

	int maxPage = (itemCount + ITEM_PER_PAGE - 1) / ITEM_PER_PAGE;

	// Aで前のページへ
	if (IsInputTrg(KEY_A))
	{
		m_ItemPage--;

		if (m_ItemPage < 0)
		{
			m_ItemPage = maxPage - 1;
		}

		m_SelectItemIndex = m_ItemPage * ITEM_PER_PAGE;
	}

	// Dで次のページへ
	if (IsInputTrg(KEY_D))
	{
		m_ItemPage++;

		if (m_ItemPage >= maxPage)
		{
			m_ItemPage = 0;
		}

		m_SelectItemIndex = m_ItemPage * ITEM_PER_PAGE;
	}

	int pageStart = m_ItemPage * ITEM_PER_PAGE;
	int pageEnd = pageStart + ITEM_PER_PAGE;

	if (pageEnd > itemCount)
	{
		pageEnd = itemCount;
	}

	// Wで上へ
	if (IsInputTrg(KEY_W))
	{
		m_SelectItemIndex--;

		if (m_SelectItemIndex < pageStart)
		{
			m_SelectItemIndex = pageEnd - 1;
		}
	}

	// Sで下へ
	if (IsInputTrg(KEY_S))
	{
		m_SelectItemIndex++;

		if (m_SelectItemIndex >= pageEnd)
		{
			m_SelectItemIndex = pageStart;
		}
	}

	// SPACEで使用
	if (IsInputTrg(KEY_SPACE))
	{
		if (UseItem(m_SelectItemIndex))
		{
			m_PlayMode = MODE_PLAY;
			m_PlayerTurn = false;
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
			m_SelectItemIndex = 0;
			m_ItemPage = 0;
			return 0;
		}
		//足踏みする(なにもしない)
		if (IsInputTrg(KEY_F))
		{
			m_PlayerTurn = false;
		}
		//デバッグ用
		if (IsInputTrg(KEY_Z))CreateEnemy();
		
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
				int ObjectNum = CollsionObject(NextPos);
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
		DeleteDeadObject();

		
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
				CanMove C = GetCanMoveEnemy(object->GetPos());

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
			CreateEnemy();
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

void CPlayScene::DeleteDeadObject() {
	//死んでいる敵を消去
	m_ObjectManager.DeleteDeadObject();
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
		DrawItemMenu();
	}
}

void CPlayScene::DrawItemMenu()
{
	DrawBox(80, 80, 500, 500, GetColor(0, 0, 0), TRUE);
	DrawBox(80, 80, 500, 500, GetColor(255, 255, 255), FALSE);

	DrawFormatString(100, 100, GetColor(255, 255, 255), "ITEM");

	const auto& inventory = m_Player->GetInventory();
	int itemCount = static_cast<int>(inventory.size());

	if (itemCount <= 0)
	{
		DrawFormatString(100, 140, GetColor(255, 255, 255), "アイテムを持っていません");
		DrawFormatString(100, 460, GetColor(255, 255, 255), "J/K: 戻る");
		return;
	}

	int maxPage = (itemCount + ITEM_PER_PAGE - 1) / ITEM_PER_PAGE;

	int pageStart = m_ItemPage * ITEM_PER_PAGE;
	int pageEnd = pageStart + ITEM_PER_PAGE;

	if (pageEnd > itemCount)
	{
		pageEnd = itemCount;
	}

	for (int i = pageStart; i < pageEnd; i++)
	{
		int drawIndex = i - pageStart;
		int y = 140 + drawIndex * 24;

		if (i == m_SelectItemIndex)
		{
			DrawFormatString(100, y, GetColor(255, 255, 0), ">");
		}

		const char* name = "不明なアイテム";

		switch (inventory[i].type)
		{
		case ITEM_1:
			name = "アイテム1";
			break;
		case ITEM_2:
			name = "アイテム2";
			break;
		case ITEM_3:
			name = "アイテム3";
			break;
		case ITEM_4:
			name = "アイテム4";
			break;
		default:
			break;
		}

		DrawFormatString(130, y, GetColor(255, 255, 255), "%s", name);
	}

	DrawFormatString(100,410,GetColor(255, 255, 255),"Page %d / %d",m_ItemPage + 1,maxPage);

	DrawFormatString(100,460,GetColor(255, 255, 255),"W/S: 選択  A/D: ページ変更  SPACE: 使用  J/K: 戻る");
}

//プレイヤーと同じ部屋にいるObjectを返す
std::vector<CObject*> CPlayScene::FindPlayerLivingTogetherObject(Int2 i) {
	CMap* Map = CMap::GetInstance();
	std::vector<CObject*> res;

	//プレイヤーの部屋番号を取得
	int PlayerRoomNum = Map->GetRoomNum(i);
	//-1(部屋にいない)の場合終了
	if (PlayerRoomNum == -1)return {};


	for (CObject* object : m_ObjectManager.GetObjects()) {
		if (object->GetKind() == KIND_PLAYER) {
			continue;
		}

		if (Map->GetRoomNum(object->GetPos()) == PlayerRoomNum) {
			res.push_back(object);
		}
	}

	return res;
}