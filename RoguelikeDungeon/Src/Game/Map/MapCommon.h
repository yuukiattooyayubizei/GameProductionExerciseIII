#pragma once
#include"../Common.h"

static constexpr int MAP_X = 50;			//マップの最大横幅
static constexpr int MAP_Y = 30;			//マップの最大縦幅
static constexpr int MAP_SIZE_MIN = 5;		//部屋の最小の大きさ
static constexpr int MAP_SIZE_MAX = 9;		//部屋の最大の大きさ
static constexpr int ROOM_MARGIN = 3;		//部屋どうしの最低距離
static constexpr int DRAW_LENGTH_X = 12;	//描写するマスの幅
static constexpr int DRAW_LENGTH_Y = 9;		
static const int ITEM_PER_PAGE = 10;		//インベントリの１ページに表示される数
static constexpr int ROOM_COUNT_MIN = 2;

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

	//親を探す
	int Find(int x)
	{
		//親が自分自身なら自分自身を返す
		if (m_Parent[x] == x)
			return x;

		//親が自分自身じゃないなら、その親の親を探す
		m_Parent[x] = Find(m_Parent[x]);
		return m_Parent[x];
	}

	//2つが同じグループかどうかを返す
	bool Same(int a, int b)
	{
		if (Find(a) == Find(b))return true;
		return false;
	}

	//2つを同じグループにする
	//Aが親としてつなぐ
	void Unite(int a, int b)
	{
		int rootA = Find(a);
		int rootB = Find(b);

		//既に同じグループなら(親が同じなら)つながない
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
	int roomA;		//部屋AのID
	int roomB;		//部屋BのID
	int distance;	//2部屋間の距離
};

struct CorridorInfo
{
	Int2 StratPos;
	Int2 MovePos;
};