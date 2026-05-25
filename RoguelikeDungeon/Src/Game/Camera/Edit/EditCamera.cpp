#include<DxLib.h>
#include"Editcamera.h"
#include<math.h>
#include "../../Common.h"
#include "../../../Lib/Input/input.h"

#define ZERO (VGet(0.0f,0.0f,0.0f))
static const float ROT_SPEED = 0.05f;
static const float MOVE_SPEED = 10.0f;

EditCamera::EditCamera()
{
	//初期化
	Init();

	LoadMapChip();
}

EditCamera::~EditCamera()
{

}

void EditCamera::Init()
{
	//とりあえず全部0を入れる
	memset(&m_camPos, 0, sizeof(VECTOR));
	memset(&m_targetPos, 0, sizeof(VECTOR));
	memset(&m_upVec, 0, sizeof(VECTOR));

	m_upVec.x = 0;
	m_upVec.y = 1.0f;
	m_upVec.z = 1.0f;

	m_TarGetPosX = m_TarGetPosZ = 0;


}

void EditCamera::Init(VECTOR camPos, VECTOR targetPos, VECTOR upVec)
{
	//初期化
	m_camPos = camPos;
	m_targetPos = targetPos;
	m_upVec = upVec;

}




void EditCamera::Step(VECTOR focus, float rota)
{

	m_camPos.x = m_TarGetPosX * 200 + 100;
	m_camPos.y = 1000;
	m_camPos.z = m_TarGetPosZ * 200 + 100;


	m_targetPos.x = m_TarGetPosX * 200 + 101;
	m_targetPos.y = 0;
	m_targetPos.z = m_TarGetPosZ * 200 + 101;



	if (IsInputTrg(KEY_W))
	{
		if(m_TarGetPosZ < MAP_Y)
		m_TarGetPosZ++;
	}
	if (IsInputTrg(KEY_S))
	{
		if (m_TarGetPosZ > 0)
		m_TarGetPosZ--;
	}
	if (IsInputTrg(KEY_A))
	{
		if (m_TarGetPosX > 0)
		m_TarGetPosX--;
	}
	if (IsInputTrg(KEY_D))
	{
		if (m_TarGetPosX < MAP_X)
		m_TarGetPosX++;
	}

}

void EditCamera::UpDate()
{
	//カメラのアップデート
	SetCameraPositionAndTargetAndUpVec(m_camPos, m_targetPos, m_upVec);
}

void EditCamera::SetNearFar(float n, float f)
{
	SetCameraNearFar(n, f);
}

void EditCamera::LoadMapChip() {
	//CSVファイルからデータを読み込み
	FILE* fp;

	//ファイルを開く　失敗したら即終了
	if (fopen_s(&fp, "Data/model/Field/Course.csv", "r") != 0)return;

	//実際のデータを所得
	//縦分
	for (int Z = 0; Z < MAP_Y; Z++)
	{
		//横分
		for (int X = 0; X < MAP_X; X++)
		{
			//データ一つ分取得
			fscanf_s(fp, "%d", &m_MapData[Z][X]);
			//カンマor改行を飛ばす
			fgetc(fp);
		}
	}
	//開いたファイルを閉じる
	fclose(fp);
}