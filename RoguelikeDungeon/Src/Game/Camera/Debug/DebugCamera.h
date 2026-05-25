#pragma once
#include<DxLib.h>

class DebugCamera
{
private:
	VECTOR m_pos;//カメラの位置
	VECTOR m_rot;//カメラの角度
public:
	//コンストラクタ、デストラクタ
	DebugCamera();
	~DebugCamera();

	//初期化
	void Init();
	void Init(VECTOR camPos, VECTOR targetPos, VECTOR upVec);

	//毎フレーム動かす処理
	void Step(VECTOR focus, float rotY);
	void UpDate();

	//ゲット関連
	VECTOR GetCamPos() { return m_pos; }
	VECTOR GetCamRot() { return m_rot; }

	//セット関連
	void SetNearFar(float n, float f);
	void SetCamPos(VECTOR pos) { m_pos = pos; }
};