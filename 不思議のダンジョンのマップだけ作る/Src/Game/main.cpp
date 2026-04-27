#include"iostream"
#include "map/map.h"
#include<DxLib.h>

using namespace std;

int main(void)
{
	ChangeWindowMode(TRUE);			// ウィンドウモードにするかどうか
	SetGraphMode(960, 720, 32);		// ウィンドウの解像度を設定する

	// ＤＸライブラリ初期化処理　基本的にはこれより後でDxLib関数を使う
	if (DxLib_Init() == -1) return -1;

	CMap m_Map;

	//3個から5個の部屋を作成
	for (int i = 0;i < GetRand(2) + 3;i++)
	{
		if (m_Map.CreateRoom() == false)
			break;
	}

	m_Map.CreateCorridor();

	m_Map.CreateStairs();

	m_Map.Draw();

	//終了処理
	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;
}