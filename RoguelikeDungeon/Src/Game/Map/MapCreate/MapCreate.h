#pragma once
#include<vector>
#include"../MapCommon.h"
#include"../../Common.h"
#include"../MapData/MapData.h"
#include"../Room/Room.h"

//廊下を作成するときに使う、部屋と部屋が行き来できるか調べるためのクラス
class UnionFind
{
private:
	std::vector<int> m_Parent;
public:
	UnionFind(int size)
	{
		//要素数を部屋数と同じにする
		m_Parent.resize(size);

		//自分自身を親にする
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
		//これを、自分自身が親のものが出るまで繰り返す
		//さらに、その自分自身が親のものが出た時にそれを保存して、次回以降の探索を短縮
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

class CMapCreate {
private:
	// 階段を作成
	void CreateStairs(CMapData& mapData);

	// 掘る
	void DigCorridor(CMapData& mapData, Int2 a, Int2 b);

	// 部屋を作成
	bool CreateRoom(CMapData& mapData, int CreateNum);

	// 部屋のサイズ決定
	CRoom RoomSizeDecision();

	// 廊下の生成
	bool CreateCorridor(CMapData& mapData);
public:
	//階層を生成
	void CreateFloor(CMapData& mapData);
};