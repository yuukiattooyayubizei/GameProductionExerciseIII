#pragma once
#include<DxLib.h>
#include"../../Common.h"
#include"../../Map/Map.h"

class EditCamera
{
private:
	int m_TarGetPosX;
	int m_TarGetPosZ;

	int m_MapData[MAP_X][MAP_Y];//マップチップのデータ

	VECTOR m_camPos;//カメラの位置
	VECTOR m_targetPos;//注視点
	VECTOR m_upVec;//高さ
public:
	//コンストラクタ、デストラクタ
	EditCamera();
	~EditCamera();

	//初期化
	void Init();
	void Init(VECTOR camPos, VECTOR targetPos, VECTOR upVec);

	void LoadMapChip();

	//毎フレーム動かす処理
	void Step(VECTOR focus, float rotY);
	void UpDate();

	//ゲット関連
	//Get関連
	VECTOR GetCamPos() { return m_camPos; }
	VECTOR GetUpVec() { return m_upVec; }
	VECTOR GetTPos() { return  m_targetPos; }

	//セット関連
	void SetNearFar(float n, float f);
};