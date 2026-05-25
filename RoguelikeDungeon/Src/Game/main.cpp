#include"iostream"
#include<DxLib.h>
#include "../Lib/Input/input.h"
#include "Scene/SceneManager.h"

using namespace std;

int main(void)
{
	ChangeWindowMode(TRUE);			// ウィンドウモードにするかどうか
	SetGraphMode(960, 720, 32);		// ウィンドウの解像度を設定する

	// ＤＸライブラリ初期化処理　基本的にはこれより後でDxLib関数を使う
	if (DxLib_Init() == -1) return -1;




	SceneManager m_Scene;

	m_Scene.Load();
	InitInput();

	//ゲームメインループ
	while (ProcessMessage() != -1)	// 問題が発生したら終了
	{
		// 「escキー」が押されたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;


		// 画面を一度何もない状態へ
		ClearDrawScreen();

		// ゲーム本体の命令はこれより下に書く
		UpdateInput();
		m_Scene.Loop();

		m_Scene.Draw();

		// ゲーム本体の命令はこれより上に書く

		// 作成した画面をモニターへ転送
		ScreenFlip();

	}

	//終了処理
	DxLib_End();			// ＤＸライブラリ使用の終了処理
	m_Scene.Exit();

	return 0;
}