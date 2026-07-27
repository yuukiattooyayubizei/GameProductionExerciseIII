#include<DxLib.h>
#include "../Lib/Input/input.h"
#include "Scene/SceneManager.h"
#include "../Lib/Sound/Sound.h"

using namespace std;

int main(void)
{
	ChangeWindowMode(TRUE);			// ウィンドウモードにするかどうか
	SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, 32);		// ウィンドウの解像度を設定する

	// ＤＸライブラリ初期化処理　基本的にはこれより後でDxLib関数を使う
	if (DxLib_Init() == -1) return -1;

	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	CInput* Input = CInput::GetInstance();
	SceneManager m_Scene;

	m_Scene.Load();
	Input->InitInput();

	InitSound();
	LoadSound();

	//ゲームメインループ
	while (ProcessMessage() != -1)	// 問題が発生したら終了
	{
		// 「escキー」が押されたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;

		// 画面を一度何もない状態へ
		ClearDrawScreen();

		// ゲーム本体の命令はこれより下に書く
		Input->UpdateInput();
		m_Scene.Loop();

		m_Scene.Draw();

		// ゲーム本体の命令はこれより上に書く

		// 作成した画面をモニターへ転送
		ScreenFlip();

	}

	//終了処理
	ExitSound();
	m_Scene.Exit();
	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;
}