#include"TitleScene.h"
#include"../../../Lib/Sound/Sound.h"
#include"../../../Lib/Input/Input.h"

//コンストラクタ
CTitleScene::CTitleScene()
{
	m_tagTitleScene = TITLE_SCENE_INIT;
	m_Titlehndl = -1;
}

//デストラクタ
CTitleScene::~CTitleScene()
{
	Exit();
}

void CTitleScene::Init()
{
	m_tagTitleScene = TITLE_SCENE_INIT;
}

void CTitleScene::Exit()
{
	if (m_Titlehndl != -1)
	{
		DeleteGraph(m_Titlehndl);
		m_Titlehndl = -1;
	}
}

void CTitleScene::Load()
{
	if(m_Titlehndl == -1)
		m_Titlehndl = LoadGraph("../Data/Image/Title/Title.JPG");
}

int CTitleScene::Loop()
{
	int m_ret = 0;

	//状態遷移に応じて挙動を変更
	switch (m_tagTitleScene)
	{
	case CTitleScene::TITLE_SCENE_INIT:
		//初期化
		Init();
		m_tagTitleScene = TITLE_SCENE_LOAD;
		break;
	case CTitleScene::TITLE_SCENE_LOAD:
		//ロード
		Load();
		m_tagTitleScene = TITLE_SCENE_LOOP;
		//BGMを鳴らす
		RequestSound(BGMID_TITLE, DX_PLAYTYPE_BACK);
		break;
	case CTitleScene::TITLE_SCENE_LOOP:
		//処理
		if (Step() == 1)m_tagTitleScene = TITLE_SCENE_END;
		break;
	case CTitleScene::TITLE_SCENE_END:
		//破棄
		StopAllSound();
		Exit();
		m_tagTitleScene = TITLE_SCENE_INIT;
		m_ret = 1;
		break;
	default:
		break;
	}

	return m_ret;
}

int CTitleScene::Step()
{
	if(CheckHitKey(KEY_INPUT_J))
		return 1;

	return 0;
}

void CTitleScene::Draw()
{
	//描画処理

	DrawFormatString(32, 96, GetColor(255, 255, 255), "Jキーでスタート");
}