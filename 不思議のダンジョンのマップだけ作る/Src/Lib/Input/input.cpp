#include "input.h"
#include<DxLib.h>

#define SLOW_COOLTIME (5)
#define FAST_COOLTIME (12)

//キー入力情報に必要なデータをまとめた構造体
typedef struct {
	unsigned int m_nowKey; //現在のボタン情報
	unsigned int m_PrevKey;//1フレーム前のボタン情報
}INPUT_DATA;

//グローバル変数で生成
static INPUT_DATA g_inputData;
static int SlowShotCoolTime = SLOW_COOLTIME;
static int FastShotCoolTime = FAST_COOLTIME;

//-------------------------------------
//キー入力初期化
//-------------------------------------
void InitInput()
{
	g_inputData.m_nowKey = g_inputData.m_PrevKey = 0;
}


//-------------------------------------
//キー入力情報更新
//-------------------------------------
void UpdateInput()
{
	//前回の入力情報を最新に更新
	g_inputData.m_PrevKey = g_inputData.m_nowKey;
	//今回の入力情報を一旦0にする
	g_inputData.m_nowKey = 0;

	//上を押した
	if (CheckHitKey(KEY_INPUT_UP))
		g_inputData.m_nowKey |= KEY_UP;
	//下を押した
	if (CheckHitKey(KEY_INPUT_DOWN))
		g_inputData.m_nowKey |= KEY_DOWN;
	//右を押した
	if (CheckHitKey(KEY_INPUT_RIGHT))
		g_inputData.m_nowKey |= KEY_RIGHT;
	//左を押した
	if (CheckHitKey(KEY_INPUT_LEFT))
		g_inputData.m_nowKey |= KEY_LEFT;
	//決定を押した
	if (CheckHitKey(KEY_INPUT_SPACE) )
		g_inputData.m_nowKey |= KEY_SPACE;
	//戻るを押した
	if (CheckHitKey(KEY_INPUT_W) )
		g_inputData.m_nowKey |= KEY_W;
	//戻るを押した
	if (CheckHitKey(KEY_INPUT_A))
		g_inputData.m_nowKey |= KEY_A;
	//戻るを押した
	if (CheckHitKey(KEY_INPUT_S))
		g_inputData.m_nowKey |= KEY_S;
	//戻るを押した
	if (CheckHitKey(KEY_INPUT_D))
		g_inputData.m_nowKey |= KEY_D;
	//戻るを押した
	if (CheckHitKey(KEY_INPUT_J))
		g_inputData.m_nowKey |= KEY_J;


	if (CheckHitKey(KEY_INPUT_K))
		g_inputData.m_nowKey |= KEY_K;
}

//キー入力判定(通常判定)
bool IsInputRep(unsigned int key)
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
bool IsInputTrg(unsigned int key)
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

