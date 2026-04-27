#pragma once
#include<DxLib.h>
#include"../Scene.h"
#include "../../Object/Player/Player.h"
#include <vector>
#include "../../map/map.h"

//当たり判定を行うフィールドのマップチップの長さ
static const int HIT_FIELD_MAP_CHIP_SIZE = 3;

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