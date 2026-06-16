#pragma once
#include<DxLib.h>
#include"Play/PlayCamera.h"
#include"Debug/DebugCamera.h"
#include"Edit/EditCamera.h"

static constexpr int CAMERA_DISTANCE_MAX = 150;					//カメラの距離
static constexpr int CAMERA_DISTANCE_MIN = 100;					//カメラの距離

static constexpr int CAMERA_HEIGHT = 350;						//カメラの高さ
static constexpr int CAMERA_TARGET_POS_Y = 250;					//カメラの注視点の高さ

class CCameraManager
{
public:
	//どのカメラを使うかのID
	enum tagCAMERA_ID
	{
		CAMERA_IS_PLAY,
		CAMERA_IS_DEBUG,
		CAMERA_IS_EDIT,
		CAMERA_IS_DIRECTION,

		CAMERA_ID_NUM,
	};

	tagCAMERA_ID m_CPLayCamID;//カメラのID
	PlayerCamera m_CPlayCam;//通常のカメラ
	DebugCamera m_CDebugCam;//デバッグ用のカメラ
	EditCamera m_CEditCam;//デバッグ用のカメラ

	//コンストラクタ
	CCameraManager();

	//初期化
	void Init();

	//カメラの制御
	void Step(VECTOR vForcos, float Rot, float spd, bool drift);

	//制御したカメラの反映
	void UpDate();

	//情報の表示
	void Draw();

	//破棄
	void Exit();

	//使うカメラの変更
	void ChengeCamera(tagCAMERA_ID ID) { m_CPLayCamID = ID; }

	//セット関連
	void SetNearFar(float n, float f);

	//ゲット関連
	tagCAMERA_ID GetCameraID() { return m_CPLayCamID; }
};