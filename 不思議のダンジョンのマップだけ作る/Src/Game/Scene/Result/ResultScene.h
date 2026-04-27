#pragma once
#include<DxLib.h>
#include"../Scene.h"

class CResultScene 
{
private:
	//プレイの状態遷移
	enum tagResultScene
	{
		RESULT_SCENE_INIT,
		RESULT_SCENE_LOAD,
		RESULT_SCENE_LOOP,
		RESULT_SCENE_END,

		RESULT_SCENE_NUM,
	};
	tagResultScene m_tagResultScene;//状態遷移


public:
	//コンストラクタ・デストラクタ
	CResultScene();
	~CResultScene();

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