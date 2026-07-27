#include"ResultScene.h"
#include"../../../Lib/Sound/Sound.h"
#include"../../../Lib/Input/Input.h"
#include "../../Common.h"

//コンストラクタ
CResultScene::CResultScene()
{
	m_tagResultScene = RESULT_SCENE_INIT;
	m_Resulthndl = -1;
}

//デストラクタ
CResultScene::~CResultScene()
{
	Exit();
}

void CResultScene::Init()
{
	m_tagResultScene = RESULT_SCENE_INIT;
}

void CResultScene::Exit()
{
	if (m_Resulthndl != -1)
	{
		DeleteGraph(m_Resulthndl);
		m_Resulthndl = -1;
	}
}

void CResultScene::Load()
{
	if (m_Resulthndl == -1)
		m_Resulthndl = LoadGraph("Data/Image/Result/Result.png");

}

int CResultScene::Loop()
{

	int m_ret = 0;

	//状態遷移に応じて挙動を変更
	switch (m_tagResultScene)
	{
	case CResultScene::RESULT_SCENE_INIT:
		//初期化
		Init();
		m_tagResultScene = RESULT_SCENE_LOAD;
		break;
	case CResultScene::RESULT_SCENE_LOAD:
		//ロード
		Load();
		m_tagResultScene = RESULT_SCENE_LOOP;
		//BGMを鳴らす
		RequestSound(BGMID_RESULT, DX_PLAYTYPE_BACK);
		break;
	case CResultScene::RESULT_SCENE_LOOP:
		//処理
		if (Step() == 1)m_tagResultScene = RESULT_SCENE_END;
		break;
	case CResultScene::RESULT_SCENE_END:
		//破棄
		StopAllSound();
		Exit();
		m_tagResultScene = RESULT_SCENE_INIT;
		m_ret = 1;
		break;
	default:
		break;
	}

	return m_ret;
}

int CResultScene::Step()
{

	if(CheckHitKey(KEY_INPUT_K))
		return 1;

	return 0;
}

void CResultScene::Draw()
{
	DrawRotaGraph(WINDOW_SIZE_HALF_X,WINDOW_SIZE_HALF_Y ,1.0f,0.0f, m_Resulthndl, TRUE);
	DrawFormatString(32, 96, GetColor(255, 255, 255), "Kキーでタイトル");
}