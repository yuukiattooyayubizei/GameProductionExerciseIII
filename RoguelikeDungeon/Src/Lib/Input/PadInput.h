#pragma once
#include "DxLib.h"

constexpr int USE_PAD_NUM = 3;		//使用するコントローラーの数

constexpr int BUTTON_BUF_LEN = 16;	//コントローラーボタン数


/*
XInputボタン入力定義
	XINPUT_BUTTON_DPAD_UP			// デジタル方向ボタン上
	XINPUT_BUTTON_DPAD_DOWN			// デジタル方向ボタン下
	XINPUT_BUTTON_DPAD_LEFT			// デジタル方向ボタン左
	XINPUT_BUTTON_DPAD_RIGHT		// デジタル方向ボタン右
	XINPUT_BUTTON_START				// STARTボタン
	XINPUT_BUTTON_BACK				// BACKボタン
	XINPUT_BUTTON_LEFT_THUMB		// 左スティック押し込み
	XINPUT_BUTTON_RIGHT_THUMB		// 右スティック押し込み
	XINPUT_BUTTON_LEFT_SHOULDER		// LBボタン
	XINPUT_BUTTON_RIGHT_SHOULDER	// RBボタン
	XINPUT_BUTTON_A					// Aボタン
	XINPUT_BUTTON_B					// Bボタン
	XINPUT_BUTTON_X					// Xボタン
	XINPUT_BUTTON_Y					// Yボタン

	"int button_index"に上の名前を入れれば判定が取れます。
	脳内コンパイルではそのはずです。多分。
*/




//コントローラーからの入力（Xinput）
class PadInput{
public:
	
	static XINPUT_STATE  input[USE_PAD_NUM];						//入力情報

	static bool currentButtonBuf[USE_PAD_NUM][BUTTON_BUF_LEN];		//現在のボタン入力状態	

	static bool preButtonBuf[USE_PAD_NUM][BUTTON_BUF_LEN];			//１フレ前のボタン入力状態

	static bool currentLStickXBuf[USE_PAD_NUM];						//Lスティック現在のX

	static bool preLStickXBuf[USE_PAD_NUM];							//Lスティック1フレ前のX
	
	static bool currentLStickYBuf[USE_PAD_NUM];						//Lスティック現在のY

	static bool preLStickYBuf[USE_PAD_NUM];							//Lスティック1フレ前のY


	//インプット初期化
	static void Init();

	//インプット通常処理
	static void Step();

	//今押された
	//引数１、コントローラーのナンバー０～２　引数２、判定するボタン(ボタンの種類はPadInput.hに)
	static bool Push(int pad_no,int button_index);

	//押されている
//引数１、コントローラーのナンバー０～２　引数２、判定するボタン(ボタンの種類はPadInput.hに)
	static bool Keep(int pad_no, int button_index);

	//離された
//引数１、コントローラーのナンバー０～２　引数２、判定するボタン(ボタンの種類はPadInput.hに)
	static bool Release(int pad_no, int button_index);

	//ずっと押されていない
//引数１、コントローラーのナンバー０～２　引数２、判定するボタン(ボタンの種類はPadInput.hに)
	static bool Release2(int pad_no, int button_index);

	//LスティックのX値を得る
	//引数、コントローラーのナンバー０～２
	static int GetLX(int pad_no) { 
		
		if(abs(input[pad_no].ThumbLX) < 2000) {
			return 0;
		}
		return input[pad_no].ThumbLX;
	}

	//Lスティックが押された瞬間のX値を得る
	//引数、コントローラーのナンバー０～２
	static int GetPushLX(int pad_no) {
		if (currentLStickXBuf[pad_no] == 1 && preLStickXBuf[pad_no] == 0) {
			return input[pad_no].ThumbLX;
		}
		return 0;
	}

	//LスティックのY値を得る
	//引数、コントローラーのナンバー０～２
	static int GetLY(int pad_no) {
	
		if (abs(input[pad_no].ThumbLY) < 2000) {
			return 0;
		}
		return input[pad_no].ThumbLY;
	}

	//Lスティックが押された瞬間のY値を得る
	//引数、コントローラーのナンバー０～２
	static int GetPushLY(int pad_no) {
		if (currentLStickYBuf[pad_no] == 1 && preLStickYBuf[pad_no] == 0) {
			return input[pad_no].ThumbLY;
		}
		return 0;
	}

	//RスティックのX値を得る
	//引数、コントローラーのナンバー０～２
	static int GetRX(int pad_no) {
		
		if (abs(input[pad_no].ThumbRX) < 2000) {
			return 0;
		}
		return input[pad_no].ThumbRX;
	}

	//RスティックのY値を得る
	//引数、コントローラーのナンバー０～２
	static int GetRY(int pad_no) {
		
		if (abs(input[pad_no].ThumbRY) < 2000) {
			return 0;
		}
		return input[pad_no].ThumbRY;
	}
};