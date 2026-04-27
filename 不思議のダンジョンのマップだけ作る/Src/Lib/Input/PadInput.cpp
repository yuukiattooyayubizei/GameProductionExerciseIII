#include "DxLib.h"
#include "PadInput.h"


XINPUT_STATE PadInput::input[USE_PAD_NUM];

bool PadInput::currentButtonBuf[USE_PAD_NUM][BUTTON_BUF_LEN];

bool PadInput::preButtonBuf[USE_PAD_NUM][BUTTON_BUF_LEN];

bool PadInput::currentLStickXBuf[USE_PAD_NUM];

bool PadInput::preLStickXBuf[USE_PAD_NUM];

bool PadInput::currentLStickYBuf[USE_PAD_NUM];

bool PadInput::preLStickYBuf[USE_PAD_NUM];



//初期化
void PadInput::Init() {

	for (int p_index = 0; p_index < USE_PAD_NUM; p_index++) {
		for (int index = 0; index < BUTTON_BUF_LEN; index++) {
			currentButtonBuf[p_index][index] = 0;
			preButtonBuf[p_index][index] = 0;
		}
	}
	SetJoypadDeadZone(DX_INPUT_PAD1, 0.1);
	SetJoypadDeadZone(DX_INPUT_PAD2, 0.1);
	SetJoypadDeadZone(DX_INPUT_PAD3, 0.1);
}


//前フレと現フレを記録(シーン管理の前に呼ぶ必要あり)
void PadInput::Step() {

	//前フレに現フレを入れる
	for (int p_index = 0; p_index < USE_PAD_NUM; p_index++) {
		for (int index = 0; index < BUTTON_BUF_LEN; index++) {
			preButtonBuf[p_index][index] = currentButtonBuf[p_index][index];
		}
		preLStickXBuf[p_index] = currentLStickXBuf[p_index];
		preLStickYBuf[p_index] = currentLStickYBuf[p_index];
	}

	// 入力状態を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &input[0]);
	GetJoypadXInputState(DX_INPUT_PAD2, &input[1]);
	GetJoypadXInputState(DX_INPUT_PAD3, &input[2]);


	//ボタン入力のフレームを更新
	for (int p_index = 0; p_index < USE_PAD_NUM; p_index++) {
		for (int index = 0; index < BUTTON_BUF_LEN; index++) {
			if (input[p_index].Buttons[index] > 0) {
				currentButtonBuf[p_index][index] = 1;
			}
			else {
				currentButtonBuf[p_index][index] = 0;
			}
		}

		//スティックの入力更新
		if (GetLX(p_index) != 0) {
			currentLStickXBuf[p_index] = 1;
		}
		else {
			currentLStickXBuf[p_index] = 0;
		}

		if (GetLY(p_index) != 0) {
			currentLStickYBuf[p_index] = 1;
		}
		else {
			currentLStickYBuf[p_index] = 0;
		}
	}
}


//今押された
//引数１、コントローラーのナンバー０～２　引数２、判定するボタン(ボタンの種類はPadInput.hに)
bool PadInput::Push(int pad_no, int button_index) {
	
	//前フレで押されていないかつ、現フレで押されている
	if (preButtonBuf[pad_no][button_index] == 0 && currentButtonBuf[pad_no][button_index] == 1) {
		return true;
	}
	else {
		//押されていないので false
		return false;
	}
}


//押されている
//引数１、コントローラーのナンバー０～２　引数２、判定するボタン(ボタンの種類はPadInput.hに)
bool PadInput::Keep(int pad_no, int button_index) {
	
	//現フレで押されている
	if (currentButtonBuf[pad_no][button_index] == 1) {
		return true;
	}
	else {
		return false;
	}
}


//離された
//引数１、コントローラーのナンバー０～２　引数２、判定するボタン(ボタンの種類はPadInput.hに)
bool PadInput::Release(int pad_no, int button_index) {
	
	//前フレで押されているかつ、現フレで押されていない。
	if (preButtonBuf[pad_no][button_index] == 1 && currentButtonBuf[pad_no][button_index] == 0) {
		return true;
	}
	else {
		return false;
	}
}


//前フレーム、現フレーム押されていない
//引数１、コントローラーのナンバー０～２　引数２、判定するボタン(ボタンの種類はPadInput.hに)
bool PadInput::Release2(int pad_no, int button_index) {

	//前フレで押されていないかつ、現フレで押されていない。
	if (preButtonBuf[pad_no][button_index] == 0 && currentButtonBuf[pad_no][button_index] == 0) {
		return true;
	}
	else {
		return false;
	}
}

