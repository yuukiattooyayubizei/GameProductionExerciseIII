#pragma once
#include<DxLib.h>
#include"../Scene.h"
#include "../../Object/Player/Player.h"
#include <vector>
#include "../../map/map.h"

//部屋の最小値
static const int ROOM_MIN = 5;
//部屋の最大値
static const int ROOM_MAX = 7;

class CPlayScene 
{
private:
	//プレイの状態遷移
	enum tagPlayScene
	{
		PLAY_SCENE_INIT,
		PLAY_SCENE_LOAD,
		PLAY_SCENE_LOOP,
		PLAY_SCENE_END,

		PLAY_SCENE_NUM,
	};
	tagPlayScene m_tagPlayScene;//状態遷移

	std::vector<CObject*> m_Object;
	CPlayer* m_Player;
	CMap m_Map;

	bool m_PlayerTurn;

public:
	//コンストラクタ・デストラクタ
	CPlayScene();
	~CPlayScene();

	//メイン処理
	int Loop();
	//描画処理
	void Draw();

private:
	//初期化
	void Init(void);

	//破棄
	void Exit();

	//ロード
	void Load();

	//制御
	int Step();
};