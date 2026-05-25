#include"CameraManager.h"
#include<DxLib.h>

CCameraManager::CCameraManager()
{
	//カメラの初期化
	Init();
	//IDをリセット
	m_CPLayCamID = CAMERA_IS_PLAY;
}

void CCameraManager::Init()
{
	//通常のカメラの初期化
	m_CPlayCam.Init();
	//デバッグ用のカメラの初期化
	m_CDebugCam.Init();	
	//編集用カメラの初期化
	m_CEditCam.Init();
}

void CCameraManager::Step(VECTOR vForcos, float Rot, float spd,bool drift)
{
	//Vキーを押したら通常のカメラに変更
	if (CheckHitKey(KEY_INPUT_V))
	{
		m_CPLayCamID = CAMERA_IS_PLAY;
	}
	//Bキーを押したらデバッグ用のカメラに変更
	if (CheckHitKey(KEY_INPUT_B))
	{
		m_CPLayCamID = CAMERA_IS_DEBUG;
	}
	//Nキーを押したら編集用のカメラに変更
	if (CheckHitKey(KEY_INPUT_N))
	{
		m_CPLayCamID = CAMERA_IS_EDIT;
	}

	//現在使っているカメラに応じて制御するカメラを決定
	switch (m_CPLayCamID)
	{
		//プレイ用のカメラの制御
	case CCameraManager::CAMERA_IS_PLAY:
		m_CPlayCam.Step(vForcos, Rot,spd, drift);
		break;
		//デバッグ用のカメラの制御
	case CCameraManager::CAMERA_IS_DEBUG:
		m_CDebugCam.Step(vForcos, Rot);
		break;
		//編集用のカメラの制御
	case CCameraManager::CAMERA_IS_EDIT:
		m_CEditCam.Step(vForcos, Rot);
		break;
		//演出用のカメラの制御
	case CCameraManager::CAMERA_IS_DIRECTION:
		
		break;
	default:
		break;
	}
}

void CCameraManager::SetNearFar(float n, float f)
{
	//カメラのニアファーをセット
	SetCameraNearFar(n, f);
}

void CCameraManager::UpDate()
{
	//カメラの設定を保存
	switch (m_CPLayCamID)
	{
		//プレイ用のカメラの設定を保存
	case CCameraManager::CAMERA_IS_PLAY:
		m_CPlayCam.UpDate();
		break;
		//デバッグ用のカメラの設定を保存
	case CCameraManager::CAMERA_IS_DEBUG:
		m_CDebugCam.UpDate();
		break;
		//編集用カメラの設定を保存
	case CCameraManager::CAMERA_IS_EDIT:
		m_CEditCam.UpDate();
		break;
		//演出用のカメラの設定を保存
	case CCameraManager::CAMERA_IS_DIRECTION:
		
		break;
	default:
		break;
	}
}

void CCameraManager::Draw()
{
	//カメラのモードに応じて表示するデータを変更
	switch (m_CPLayCamID)
	{
		//プレイ用のカメラのデータを表示
	case CCameraManager::CAMERA_IS_PLAY:
		DrawFormatString(32, 280, GetColor(255, 0, 0), "posx = %f",m_CPlayCam.GetCamPos().x);
		DrawFormatString(32, 312, GetColor(255, 0, 0), "posy = %f", m_CPlayCam.GetCamPos().y);
		DrawFormatString(32, 344, GetColor(255, 0, 0), "posz = %f", m_CPlayCam.GetCamPos().z);
		break;
		//デバッグ用のカメラのデータを表示
	case CCameraManager::CAMERA_IS_DEBUG:

		break;
		//演出用のカメラのデータを表示
	case CCameraManager::CAMERA_IS_DIRECTION:

		break;
	default:
		break;
	}
}

void CCameraManager::Exit()
{

}