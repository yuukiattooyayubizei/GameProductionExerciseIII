#include<DxLib.h>
#include"PlayCamera.h"
#include<math.h>
#include "../../Common.h"
#include "../CameraManager.h"

PlayerCamera::PlayerCamera()
{
	//初期化
	Init();
}

PlayerCamera::~PlayerCamera()
{

}

void PlayerCamera::Init()
{
	//とりあえず全部0を入れる
	memset(&m_camPos, 0, sizeof(VECTOR));
	memset(&m_targetPos, 0, sizeof(VECTOR));
	memset(&m_upVec, 0, sizeof(VECTOR));

	m_upVec.y = 1.0f;
	m_Camera_Distance = CAMERA_DISTANCE_MAX;
	m_isZoomIn = false;
}

void PlayerCamera::Init(VECTOR camPos, VECTOR targetPos, VECTOR upVec)
{
	//初期化
	m_camPos = camPos;
	m_targetPos = targetPos;
	m_upVec = upVec;
	m_Camera_Distance = CAMERA_DISTANCE_MAX;
	m_isZoomIn = false;
}



void PlayerCamera::Step(VECTOR focus, float rotY, float spd,bool zoomin)
{

		// focus はプレイヤーの3D座標
		// XZ平面がマップ、Yが高さ

		m_targetPos = focus;
		m_targetPos.y = 0.0f;

		m_camPos.x = focus.x;
		m_camPos.y = 600.0f;
		m_camPos.z = focus.z - 600.0f;

		m_upVec = VGet(0.0f, 1.0f, 0.0f);
	


	// //プレイヤーが原点にいて、Y軸回転が0度であることを前提に考える
	////プレイヤーの回転角度に合わせて上記方向ベクトルを回転させる
	//VECTOR v;
	////v.x = cosf(rotY + DX_PI_F) * m_Camera_Distance;
	////v.y = sinf(rotY + DX_PI_F) * m_Camera_Distance;
	////v.z = CAMERA_HEIGHT;
	//v.x = 0;
	//v.y = 0;
	//v.z = CAMERA_HEIGHT;
	//
	////プレイヤーの位置から計算結果の距離を移動させたらカメラの視点になる
	//m_camPos = VAdd(focus, v);

	//// 注視点はプレイヤーの位置を利用する
	//m_targetPos = focus;
	//m_targetPos.y += CAMERA_TARGET_POS_Y;
}

void PlayerCamera::UpDate()
{
	//カメラのアップデート
	SetCameraPositionAndTargetAndUpVec(m_camPos, m_targetPos, m_upVec);
}

void PlayerCamera::SetNearFar(float n, float f)
{
	//ニアファーをセット
	SetCameraNearFar(n, f);
}