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
}

void PlayerCamera::Init(VECTOR camPos, VECTOR targetPos, VECTOR upVec)
{
	//初期化
	m_camPos = camPos;
	m_targetPos = targetPos;
	m_upVec = upVec;
}



void PlayerCamera::Step(VECTOR focus, float rotY, float spd, bool zoomin)
{
	m_targetPos = focus;
	m_targetPos.y = 0.0f;

	m_camPos = VGet(focus.x, 1400.0f, focus.z);

	m_upVec = VGet(0.0f, 0.0f, -1.0f);
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