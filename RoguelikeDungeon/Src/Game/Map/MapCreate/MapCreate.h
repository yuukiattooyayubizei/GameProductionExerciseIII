#pragma once
#include<vector>
#include"../../Common.h"
#include"../MapData/MapData.h"
#include"../Room/Room.h"

struct RoomEdge
{
	int roomA;		//部屋AのID
	int roomB;		//部屋BのID
	int distance;	//2部屋間の距離
};

//廊下を作成するときに使う、部屋と部屋が行き来できるか調べるためのクラス
class UnionFind
{
private:
	std::vector<int> m_Parent;
public:
	UnionFind(int size);

	//親を探す
	int Find(int x);

	//2つが同じグループかどうかを返す
	bool Same(int a, int b);

	//2つを同じグループにする
	//Aが親としてつなぐ
	void Unite(int a, int b);
};

class CMapCreate {
private:
	// 階段を作成
	void CreateStairs(CMapData& mapData);

	// 掘る
	void DigCorridor(CMapData& mapData,const std::vector<Int2>& route);

	// 部屋を作成
	bool CreateRoom(CMapData& mapData, int CreateNum);

	// 部屋のサイズ決定
	CRoom RoomSizeDecision();

	// 廊下の生成
	bool CreateCorridor(CMapData& mapData);

	//廊下のルートの直線を作成
	//route:作成したrouteを入れる
	//includeStart:移動前のマスを含むか
	void AddLineToRoute(std::vector<Int2>& route, Int2 start, Int2 end, bool includeStart);

	//廊下のルートを作成
	//gateA:スタートの座標
	//gateB:ゴールの座標
	//bend:曲がり角の座標
	std::vector<Int2> CreateCorridorRoute(Int2 gateA, Int2 gateB, Int2 bend);

	bool IsRoomLikeTile(TILE tile);

	bool IsCorridorTile(TILE tile);

	bool FindCorridorGates(CMapData& mapData,const std::vector<Int2>& fullRoute,Int2& gateA,Int2& gateB,std::vector<Int2>& gateRoute);
public:
	//階層を生成
	void CreateFloor(CMapData& mapData);
};