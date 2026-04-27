#pragma once

//今回のシューティングゲームで使うボタン
#define KEY_UP		(0b000001)//上ボタン
#define KEY_DOWN	(0b000010)//下ボタン
#define KEY_RIGHT	(0b000100)//右ボタン
#define KEY_LEFT	(0b001000)//左ボタン
#define KEY_SPACE	(0b010000)//決定ボタン
#define KEY_J		(0b100000)//戻るボタン
#define KEY_W	   (0b1000000)//戻るボタン
#define KEY_A	  (0b10000000)//戻るボタン
#define KEY_S	 (0b100000000)//戻るボタン
#define KEY_D	(0b1000000000)//戻るボタン
#define KEY_K	(0b10000000000)

//キー入力初期化
void InitInput();

//キー入力情報更新
void UpdateInput();

//キー入力判定(通常判定)
bool IsInputRep(unsigned int key);

//キー入力判定(トリガー判定)
bool IsInputTrg(unsigned int key);

