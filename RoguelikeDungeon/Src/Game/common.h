#pragma once
#include"DxLib.h"

static constexpr float CAMERA_NEAR = 3.0f;						//カメラの映す距離の下限
static constexpr float CAMERA_FAR = 15000.0f;					//カメラの映す距離の上限

static constexpr float TILE_SIZE = 100.0f;						//マップチップ1枚ごとのサイズ

//部屋の最小値
static constexpr int ROOM_MIN = 5;
//部屋の最大値
static constexpr int ROOM_MAX = 7;

//最初に沸くアイテムの数
static constexpr int STRAT_ITEM_NUM = 5;
//最初に沸く敵の数
static constexpr int STRAT_ENEMY_NUM = 5;

enum PlayMode
{
	MODE_PLAY,
	MODE_ITEM_MENU,
};

static int LoadModelWithScale(const char* path, VECTOR scale = VGet(1.0f,1.0f,1.0f), VECTOR rotation = VGet(0.0f, 0.0f, 0.0f))
{
	int hndl = MV1LoadModel(path);
	if (hndl != -1)
	{
		MV1SetScale(hndl, scale);
		MV1SetRotationXYZ(hndl, rotation);
	}

	return hndl;
}

struct Int2 {
	int x, y;
};

enum DIRECTION {
	DIRECTION_NON,	//移動しない
	DIRECTION_UP,	//上
	DIRECTION_LEFT,	//左
	DIRECTION_DOWN,	//下
	DIRECTION_RIGHT	//右
};

static Int2 DirectionToInt2(DIRECTION dir) {
	if (dir == DIRECTION_UP)return { 0,-1 };
	else if (dir == DIRECTION_DOWN)return { 0,1 };
	else if (dir == DIRECTION_LEFT)return { -1,0 };
	else if (dir == DIRECTION_RIGHT)return { 1,0 };
	else return { 0,0 };
}


//その方向に動けるかどうか
typedef struct {
	bool Up;
	bool Down;
	bool Left;
	bool Right;
}CanMove;

enum TILE {
	TILE_NON,			//無(バグった時にしか出ない)
	TILE_WALL,			//壁
	TILE_ROOM,			//部屋
	TILE_CORRIDOR,		//廊下
};

enum ITEM_TYPE
{
	ITEM_NON = -1,	//アイテムがない状態

	ITEM_1,
	ITEM_2,
	ITEM_3,
	ITEM_4,

	ITEM_NUM,		//アイテムの種類数
};

static Int2 AddInt2(Int2 i1, Int2 i2)
{
	Int2 Ret;
	Ret.x = i1.x + i2.x;
	Ret.y = i1.y + i2.y;
	return Ret;
}

static Int2 SubInt2(Int2 i1, Int2 i2)
{
	Int2 Ret;
	Ret.x = i1.x - i2.x;
	Ret.y = i1.y - i2.y;
	return Ret;
}

//2つのInt2が同じかどうか
static bool CollsionInt2(Int2 i1, Int2 i2)
{
	if (i1.x == i2.x && i1.y == i2.y)
		return true;
	else
		return false;
}

struct Float2 {
	float x, y;
};

static Int2 ChangeInt2ToFloat2(Float2 f) {
	Int2 ret = {};
	ret.x = (int)f.x;
	ret.y = (int)f.y;
	return ret;
}

//bool型のtrueとfalseを逆転させる
static bool ReverseBool(bool is) {
	if (is == true)
		return false;
	else
		return true;
}

static constexpr int RETRY_MAX = 100;		//部屋作成時のリトライ回数(これを超えると部屋の置き場がないとみなす)