#include"playScene.h"
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

using namespace std;

Int2 CPlayScene::FindSpawnPos()
{
	for (int i = 0; i < RETRY_MAX; ++i)
	{
		Int2 pos = m_Map.GetRoomPos();

		if (!CollsionAll(pos))
		{
			return pos;
		}
	}
	return { -1, -1 };
}

bool CPlayScene::CollsionObject(Int2& pos) const
{
	for (CObject* obj : m_Object)
	{
		if (obj->GetPos().x == pos.x && obj->GetPos().y == pos.y)
		{
			return true;
		}
	}

	return false;
}

CanMove CPlayScene::GetCanMove(Int2 pos)
{
	Int2 v = pos;
	CanMove C;

	int x = static_cast<int>(v.x);
	int y = static_cast<int>(v.y);
	//一旦全部trueに
	C.Down = true, C.Up = true, C.Left = true, C.Right = true;
	//マス目の端だとマスの外側の方向には行けない
	if (x <= 0)
		C.Left = false;
	if (x >= MAP_X - 1)
		C.Right = false;
	if (y <= 0)
		C.Up = false;
	if (y >= MAP_Y - 1)
		C.Down = false;



	//上下左右のマスを見て通れるマスでなければ行けない
	//すでにfalseなら見る必要がない
	TILE t = {};
	if (C.Left == true)
	{
		t = m_Map.GetTile(x - 1, y);
		if (t == TILE_WALL)
			C.Left = false;
	}
	if (C.Right == true)
	{
		t = m_Map.GetTile(x + 1, y);
		if (t == TILE_WALL)
			C.Right = false;
	}
	if (C.Up == true)
	{
		t = m_Map.GetTile(x, y - 1);
		if (t == TILE_WALL)
			C.Up = false;
	}
	if (C.Down == true)
	{
		t = m_Map.GetTile(x, y + 1);
		if (t == TILE_WALL)
			C.Down = false;
	}

	return C;
}

bool CPlayScene::CollsionAll(Int2 pos) 
{
	if (CollsionObject(pos) == true)return true;
	if (m_Map.CollisionItem(pos) == true)return true;
	if (m_Map.CollisionStairs(pos) == true)return true;

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
	Data->Init();

	m_Player = new CPlayer();
	m_Object.push_back(m_Player);



	m_Player->Init();

}

void CPlayScene::Exit()
{

	for (auto obj : m_Object) {
		obj->Exit();
		delete obj;
	}
	m_Object.clear();
	m_Map.DeleteAll();

	m_Player = nullptr;
}

void CPlayScene::CreateEnemy(int CreateNum){
	for (int i = 0; i < CreateNum; i++)
	{
		Int2 pos = FindSpawnPos();

		CEnemy* enemy = new CEnemy();
		enemy->SetPos(pos);

		std::cout << pos.x << "," << pos.y << "に敵を生成" << std::endl;
		m_Object.push_back(enemy);
	}
}

void CPlayScene::CreatePlayer() {
	Int2 v = FindSpawnPos();
	m_Player->SetPos(v);
}

void CPlayScene::Load()
{
	CData* Data = CData::GetInstance();

	Data->Load();

	//3個から5個の部屋を作成
	if (m_Map.CreateRoom(GetRand(ROOM_MAX - ROOM_MIN) + 3) == false)return;
	m_Map.CreateCorridor();
	m_Map.CreateStairs();
	m_Map.CreateItem(5);

	CreatePlayer();

	//敵を作成
	CreateEnemy(5);

	for_each(m_Object.begin(), m_Object.end(), [](CObject* object) {object->Load(); });
	//m_Map.Draw(m_Player->GetPos().x, m_Player->GetPos().y);
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

ObjectKind CPlayScene::GetAheadMoveObject(Int2 pos, DIRECTION dir){

	switch (dir){
	case DIRECTION_UP:
		break;
	case DIRECTION_DOWN:
		break;
	case DIRECTION_LEFT:
		break;
	case DIRECTION_RIGHT:
		break;
	default:
		break;
	}
	for_each(m_Object.begin(), m_Object.end(), [this](CObject* object) {

	});
	return KIND_PLAYER;
}

int CPlayScene::Step()
{
	CData* Data = CData::GetInstance();


	//プレイヤーの行動待ちなら
	if (m_PlayerTurn == true)
	{
		for_each(m_Object.begin(), m_Object.end(), [this](CObject* object) {
			//オブジェクトが動けるマスを探す
			CanMove C = GetCanMove(object->GetPos());
			
			//プレイヤーだけ動かす
			if (object->GetKind() == KIND_PLAYER)
			{
				object->Step(C);

				//プレイヤーが移動していたら
				if (object->GetDirection() != DIRECTION_NON)
				{
					m_PlayerTurn = false;

					//移動先のアイテムを検索
					Item item = {};
					item.type = m_Map.IsItemExist(m_Player->GetPos());

					//アイテムがあったら
					if (item.type != ITEM_NON)
					{
						//そのアイテムをインベントリに入れる
						m_Player->AddItem(item);
						//入れたアイテムを消す
						m_Map.EraseItem(m_Player->GetPos());
					}
				}
			}
		});
	}
	//プレイヤー行動の後
	else
	{
		for_each(m_Object.begin(), m_Object.end(), [this](CObject* object) {
			//オブジェクトが動けるマスを探す
			CanMove C = GetCanMove(object->GetPos());

			//プレイヤー以外を動かす
			if (object->GetKind() != KIND_PLAYER)
				object->Step(C);

			});
		m_PlayerTurn = true;
	}



	if (CheckHitKey(KEY_INPUT_L))
		return 1;
	return 0;
}

void CPlayScene::Draw()
{
	//プレイヤーがnullなら呼ばない
	if (m_Player == nullptr)return;


	CData* Data = CData::GetInstance();

	
	m_Map.Draw(m_Player->GetPos().x, m_Player->GetPos().y);

	for_each(m_Object.begin(), m_Object.end(), [](CObject* object) {object->Draw(); });

	//描画処理
	DrawFormatString(32, 704, GetColor(255, 255, 255), "プレイシーンLキーでリザルトに遷移");


}