#pragma once
#include "../common.h"

static constexpr int MAP_X = 50;			//マップの最大横幅
static constexpr int MAP_Y = 30;			//マップの最大縦幅
static constexpr int MAP_SIZE_MIN = 5;		//部屋の最小の大きさ
static constexpr int MAP_SIZE_MAX = 9;		//部屋の最大の大きさ
static constexpr int ROOM_MARGIN = 3;		//部屋どうしの最低距離

static constexpr int DRAW_LENGTH_X = 12;					//描写するマスの幅
static constexpr int DRAW_LENGTH_Y = 9;

//インベントリの１ページい表示される数
static const int ITEM_PER_PAGE = 10;



class UnionFind
{
private:
	std::vector<int> m_Parent;

public:
	UnionFind(int size)
	{
		m_Parent.resize(size);

		for (int i = 0; i < size; i++)
		{
			m_Parent[i] = i;
		}
	}

	int Find(int x)
	{
		if (m_Parent[x] == x)
			return x;

		m_Parent[x] = Find(m_Parent[x]);
		return m_Parent[x];
	}

	bool Same(int a, int b)
	{
		return Find(a) == Find(b);
	}

	void Unite(int a, int b)
	{
		int rootA = Find(a);
		int rootB = Find(b);

		if (rootA == rootB)
			return;

		m_Parent[rootB] = rootA;
	}
};

struct SpecifiedRoomInformation {
	int m_CloseRoomID;			//どの部屋が一番近いか
	DIRECTION m_Direction;		//一番近い部屋の方角	
	int m_DistanceX;			//中心までのX座標の距離
	int m_DistanceY;			//中心までのY座標の距離
};

struct RoomEdge
{
	int roomA;
	int roomB;
	int distance;
};

struct CorridorInfo
{
	Int2 StratPos;
	Int2 MovePos;
};