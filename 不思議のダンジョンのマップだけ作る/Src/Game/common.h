#pragma once

enum DIRECTION {
	DIRECTION_NON,
	DIRECTION_UP,	//上
	DIRECTION_LEFT,	//左
	DIRECTION_DOWN,	//下
	DIRECTION_RIGHT	//右
};

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

struct Int2 {
	int x, y;
};

static Int2 AddInt2(Int2 i1, Int2 i2)
{
	Int2 Ret;
	Ret.x = i1.x + i2.x;
	Ret.y = i1.y + i2.y;
	return Ret;
}

struct Float2 {
	float x, y;
};

//bool型のtrueとfalseを逆転させる
static bool ReverseBool(bool is) {
	if (is == true)
		return false;
	else
		true;
}