#pragma once
#include"../../Camera/CameraManager.h"
#include"../../Object/ObjectManager.h"
#include"../../UI/UI.h"

class CPlayScene 
{
private:
	static constexpr int GOAL_FLOOR = 10;
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
	CCameraManager m_CameraManager;//カメラ
	CObjectManager m_ObjectManager;
	CUI m_UI;

	bool m_PlayerTurn;
	int m_EnemySpwanWait;
	PlayMode m_PlayMode;
	//階層
	int m_Floor;
private:
	//階層を生成
	void CreateFloor();

	//アイテムの使用
	bool UseItem(int index);
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

	int StepPlay();
};