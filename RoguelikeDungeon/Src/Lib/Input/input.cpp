#include"Input.h"
#include<DxLib.h>

CInput* CInput::m_Instance = NULL;

//-------------------------------------
//キー入力初期化
//-------------------------------------
void CInput::InitInput()
{
	g_inputData.m_nowKey = g_inputData.m_PrevKey = 0;
}


//-------------------------------------
//キー入力情報更新
//-------------------------------------
void CInput::UpdateInput()
{
	//前回の入力情報を最新に更新
	g_inputData.m_PrevKey = g_inputData.m_nowKey;
	//今回の入力情報を一旦0にする
	g_inputData.m_nowKey = 0;


	if (CheckHitKey(KEY_INPUT_UP))
		g_inputData.m_nowKey |= KEY_UP;
	if (CheckHitKey(KEY_INPUT_DOWN))
		g_inputData.m_nowKey |= KEY_DOWN;
	if (CheckHitKey(KEY_INPUT_RIGHT))
		g_inputData.m_nowKey |= KEY_RIGHT;
	if (CheckHitKey(KEY_INPUT_LEFT))
		g_inputData.m_nowKey |= KEY_LEFT;
	if (CheckHitKey(KEY_INPUT_SPACE))
		g_inputData.m_nowKey |= KEY_SPACE;
	if (CheckHitKey(KEY_INPUT_A))
		g_inputData.m_nowKey |= KEY_A;
	if (CheckHitKey(KEY_INPUT_B))
		g_inputData.m_nowKey |= KEY_B;
	if (CheckHitKey(KEY_INPUT_C))
		g_inputData.m_nowKey |= KEY_C;
	if (CheckHitKey(KEY_INPUT_D))
		g_inputData.m_nowKey |= KEY_D;
	if (CheckHitKey(KEY_INPUT_E))
		g_inputData.m_nowKey |= KEY_E;
	if (CheckHitKey(KEY_INPUT_F))
		g_inputData.m_nowKey |= KEY_F;
	if (CheckHitKey(KEY_INPUT_G))
		g_inputData.m_nowKey |= KEY_G;
	if (CheckHitKey(KEY_INPUT_H))
		g_inputData.m_nowKey |= KEY_H;
	if (CheckHitKey(KEY_INPUT_I))
		g_inputData.m_nowKey |= KEY_I;
	if (CheckHitKey(KEY_INPUT_J))
		g_inputData.m_nowKey |= KEY_J;
	if (CheckHitKey(KEY_INPUT_K))
		g_inputData.m_nowKey |= KEY_K;
	if (CheckHitKey(KEY_INPUT_L))
		g_inputData.m_nowKey |= KEY_L;
	if (CheckHitKey(KEY_INPUT_N))
		g_inputData.m_nowKey |= KEY_N;
	if (CheckHitKey(KEY_INPUT_M))
		g_inputData.m_nowKey |= KEY_M;
	if (CheckHitKey(KEY_INPUT_O))
		g_inputData.m_nowKey |= KEY_O;
	if (CheckHitKey(KEY_INPUT_P))
		g_inputData.m_nowKey |= KEY_P;
	if (CheckHitKey(KEY_INPUT_Q))
		g_inputData.m_nowKey |= KEY_Q;
	if (CheckHitKey(KEY_INPUT_R))
		g_inputData.m_nowKey |= KEY_R;
	if (CheckHitKey(KEY_INPUT_S))
		g_inputData.m_nowKey |= KEY_S;
	if (CheckHitKey(KEY_INPUT_T))
		g_inputData.m_nowKey |= KEY_T;
	if (CheckHitKey(KEY_INPUT_U))
		g_inputData.m_nowKey |= KEY_U;
	if (CheckHitKey(KEY_INPUT_V))
		g_inputData.m_nowKey |= KEY_V;
	if (CheckHitKey(KEY_INPUT_W))
		g_inputData.m_nowKey |= KEY_W;
	if (CheckHitKey(KEY_INPUT_X))
		g_inputData.m_nowKey |= KEY_X;
	if (CheckHitKey(KEY_INPUT_Y))
		g_inputData.m_nowKey |= KEY_Y;
	if (CheckHitKey(KEY_INPUT_Z))
		g_inputData.m_nowKey |= KEY_Z;
}

//キー入力判定(通常判定)
bool CInput::IsInputRep(unsigned int key)
{
	if (g_inputData.m_nowKey)
	{
		if ((g_inputData.m_nowKey & key) != 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

//キー入力判定(トリガー判定)
bool CInput::IsInputTrg(unsigned int key)
{
	if ((g_inputData.m_nowKey & key) && !(g_inputData.m_PrevKey & key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

CInput* CInput::GetInstance() {
	//まだ生成されてないなら
	if (m_Instance == NULL)
	{
		m_Instance = new CInput();
	}

	return m_Instance;
}

void CInput::DeleteInstance() {
	//まだ生成されてないなら
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}

}