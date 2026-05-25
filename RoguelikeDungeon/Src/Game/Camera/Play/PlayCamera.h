#pragma once
#include<DxLib.h>

class PlayerCamera
{
private:
	VECTOR m_camPos;//カメラの位置
	VECTOR m_targetPos;//注視点
	VECTOR m_upVec;//高さ
	int m_Camera_Distance;
	bool m_isZoomIn;
public:
	//コンストラクタ・デストラクタ
	PlayerCamera();
	~PlayerCamera();

	//初期化
	void Init();
	void Init(VECTOR camPos, VECTOR targetPos, VECTOR upVec);

	//制御
	void Step(VECTOR focus, float rotY,float spd, bool zoomin);

	//更新
	void UpDate();

	//NearFarのセット
	void SetNearFar(float n, float f);

	//Get関連
	VECTOR GetCamPos() { return m_camPos; }
	VECTOR GetUpVec() { return m_upVec; }
	VECTOR GetTPos() { return  m_targetPos; }
};