#include"sceneManager.h"
#include"math.h"
#include "../../Lib/Sound/sound.h"

//定義関連

//コンストラクタ
SceneManager::SceneManager()
{
	//初期化
	m_TagScene = SCENE_TITLE;

	InitSound();
}

//デストラクタ
SceneManager::~SceneManager()
{
	//破棄
	ExitSound();
}

int SceneManager::Loop()
{
	int m_ret = 0;

	//状態遷移によって制御先を変更
	switch (m_TagScene)
	{
		//タイトルシーンの制御
	case SceneManager::SCENE_TITLE:
		if (CTitleScene.Loop() != 0)//タイトルの制御
			m_TagScene = SCENE_PLAY;
		break;
		//プレイシーンの制御
	case SceneManager::SCENE_PLAY:
	{
		if (CPlayScene.Loop() != 0)//プレイの制御
		{
			m_TagScene = SCENE_RESULT;
		}
		

	}
	break;
	//リザルトシーンの制御
	case SceneManager::SCENE_RESULT:
		if (CResultScene.Loop() != 0)//リザルトの制御
			m_TagScene = SCENE_TITLE;
		break;
	default:
		break;
	}

	return m_ret;
}

void SceneManager::Draw()
{
	//描画
	switch (m_TagScene)
	{
		//タイトルの描写
	case SceneManager::SCENE_TITLE:
		CTitleScene.Draw();
		break;
		//プレイシーンの描写
	case SceneManager::SCENE_PLAY:

		CPlayScene.Draw();
		break;
		//リザルトの描写
	case SceneManager::SCENE_RESULT:
		CResultScene.Draw();
		break;
	default:
		break;
	}
}

//ロード
void  SceneManager::Load()
{

	LoadSound();

}

//破棄
void  SceneManager::Exit()
{

}