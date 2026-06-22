#pragma once
#include"../Common.h"

static constexpr int MAP_X = 50;			//マップの最大横幅
static constexpr int MAP_Y = 30;			//マップの最大縦幅
static constexpr int MAP_SIZE_MIN = 5;		//部屋の最小の大きさ
static constexpr int MAP_SIZE_MAX = 9;		//部屋の最大の大きさ
static constexpr int ROOM_MARGIN = 3;		//部屋どうしの最低距離
static constexpr int DRAW_LENGTH_X = 12;	//描写するマスの幅
static constexpr int DRAW_LENGTH_Y = 9;		
static constexpr int ITEM_PER_PAGE = 10;	//インベントリの１ページに表示される数
static constexpr int ROOM_COUNT_MIN = 2;

struct RoomEdge
{
	int roomA;		//部屋AのID
	int roomB;		//部屋BのID
	int distance;	//2部屋間の距離
};

struct CorridorInfo
{
	Int2 StratPos;
	Int2 MovePos;
};