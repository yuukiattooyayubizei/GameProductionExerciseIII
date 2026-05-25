#pragma once

#include<DxLib.h>
#include "Play/PlayScene.h"
#include "Title/TitleScene.h"
#include "Result/ResultScene.h"

class SceneManager
{
private:
	//状態遷移
	enum tagScene
	{
		SCENE_TITLE,
		SCENE_PLAY,
		SCENE_RESULT,

		SCENE_NUM,
	};
	CPlayScene CPlayScene;//プレイ時のシーン
	CTitleScene CTitleScene;//タイトル時のシーン
	CResultScene CResultScene;//リザルト時のシーン
	tagScene m_TagScene;//状態遷移

public:
	//コンストラクタ・デストラクタ
	SceneManager();
	~SceneManager();

	//メイン処理
	int Loop();
	//描画処理
	void Draw();

	//ロード
	void Load();

	//破棄
	void Exit();
};