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
	Data->Init();

	m_Player = new CPlayer();
	m_Object.push_back(m_Player);



	m_Player->Init();

}

void CPlayScene::Exit()
{
	for_each(m_Object.begin(), m_Object.end(), [](CObject* object) {object->Exit(); });

	for (auto obj : m_Object) {
		obj->Exit();
		delete obj;
	}
	m_Object.clear();
	m_Map.DeleteAll();

	m_Player = nullptr;
}

void CPlayScene::Load()
{
	CData* Data = CData::GetInstance();

	Data->Load();

	//3個から5個の部屋を作成
	for (int i = 0;i < GetRand(2) + 3;i++)
	{
		if (m_Map.CreateRoom() == false)
			break;
	}

	m_Map.CreateCorridor();

	m_Map.CreateStairs();

	for_each(m_Object.begin(), m_Object.end(), [](CObject* object) {object->Load(); });

	//VECTOR v = {0};
	//v.x = GetRand(MAP_X - 1);
	//v.y = GetRand(MAP_Y - 1);
	//TILE t = {};
	//while (t == m_Map.GetTile(v.x, v.y) == TILE_WALL || t == m_Map.GetTile(v.x, v.y) == TILE_WALL_EDGE)
	//{
	//	v.x = GetRand(MAP_X - 1);
	//	v.y = GetRand(MAP_Y - 1);

	//	if (t == m_Map.GetTile(v.x, v.y) != TILE_WALL && t == m_Map.GetTile(v.x, v.y) != TILE_WALL_EDGE)break;
	//}

	m_Map.GetStratRoom();
	VECTOR v = { 0 };
	v.x = GetRand(MAP_X - 1);
	v.y = GetRand(MAP_Y - 1);


	m_Player->SetPos(v);
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
	CData* Data = CData::GetInstance();


	//プレイヤーの行動待ちなら
	if (m_PlayerTurn == true)
	{
		for_each(m_Object.begin(), m_Object.end(), [this](CObject* object) {
			//オブジェクトが動けるマスを探す
			CanMove C;
			VECTOR v = object->GetPos();

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


				if (t == TILE_WALL || TILE_WALL_EDGE)
					C.Left = false;
			}
			if (C.Right == true)
			{
				t = m_Map.GetTile(x + 1, y);


				if (t == TILE_WALL || TILE_WALL_EDGE)
					C.Right = false;
			}
			if (C.Up == true)
			{
				t = m_Map.GetTile(x, y - 1);


				if (t == TILE_WALL || TILE_WALL_EDGE)
					C.Up = false;
			}
			if (C.Down == true)
			{
				t = m_Map.GetTile(x, y + 1);


				if (t == TILE_WALL || TILE_WALL_EDGE)
					C.Down = false;
			}


			object->Step(C);

			//プレイヤーが移動していたら
			if (object->GetKind() == KIND_PLAYER && object->GetDirection() != DIRECTION_NON)
			{
				m_PlayerTurn = false;
			}
			});
	}
	//プレイヤー行動の後
	else
	{
	m_Map.Draw(m_Player->GetPos().x, m_Player->GetPos().y);
		m_PlayerTurn = true;
	}



	if (CheckHitKey(KEY_INPUT_L))
		return 1;
	return 0;
}

void CPlayScene::Draw()
{
	CData* Data = CData::GetInstance();
	//背景の描画

	for_each(m_Object.begin(), m_Object.end(), [](CObject* object) {object->Draw(); });



	//描画処理
	DrawFormatString(32, 96, GetColor(255, 255, 255), "プレイシーンLキーでリザルトに遷移");


}