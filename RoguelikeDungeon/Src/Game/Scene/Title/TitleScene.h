#pragma once
#include<DxLib.h>
#include"../Scene.h"

class CTitleScene 
{
private:
	//プレイの状態遷移
	enum tagTitleScene
	{
		TITLE_SCENE_INIT,
		TITLE_SCENE_LOAD,
		TITLE_SCENE_LOOP,
		TITLE_SCENE_END,

		TITLE_SCENE_NUM,
	};
	tagTitleScene m_tagTitleScene;//状態遷移

	int m_Titlehndl;

public:
	//コンストラクタ・デストラクタ
	CTitleScene();
	~CTitleScene();

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