#pragma once

class CScene
{
private:
	//プレイの状態遷移
	enum tagScene
	{
		SCENE_INIT,
		SCENE_LOAD,
		SCENE_LOOP,
		SCENE_END,

		SCENE_NUM,
	};
	tagScene m_tagScene;//状態遷移

public:
	//コンストラクタ・デストラクタ
	CScene();
	virtual ~CScene() = 0;

	//メイン処理
	virtual int Loop() = 0;
	//描画処理
	virtual void Draw() = 0;

private:
	//初期化
	virtual void Init(void) = 0;

	//破棄
	virtual void Exit() = 0;

	//ロード
	virtual void Load() = 0;

	//制御
	virtual int Step() = 0;
};