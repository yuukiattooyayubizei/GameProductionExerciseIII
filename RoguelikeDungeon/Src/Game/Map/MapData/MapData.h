#pragma once
#include"../Room/Room.h"
#include<vector>
#include"../MapCommon.h"
#include "../RoomLink/RoomLink.h"

class CMapData {
private:
	// 階段の座標
	Int2 m_StairsPos = {};
	// マップチップ
	TILE m_Map[MAP_Y][MAP_X] = {};

	// 部屋
	std::vector<CRoom> m_Room;

	//部屋どうしのつながり
	std::vector<RoomLink> m_RoomLinks;

	std::vector<std::vector<int>> m_RoomGraph;

public:
	void Init();
	void Draw();
	void Exit();
private:
	std::vector<CRoom> GetAllRoom() { return m_Room; }

	//現在位置に廊下があるか調べる
	bool IsPositionOnRoomLink(const RoomLink& link, const Int2& position) const;
public:
	//指定座標がプレイヤーから見えるか判定
	bool IsVisibleFrom(const Int2 PPos, const  Int2 EPos);

	void AddLineToRoute(std::vector<Int2>& route,Int2 start,Int2 end,bool includeStart);

	void SetTile(Int2 pos, TILE tile) { m_Map[pos.y][pos.x] = tile; }
	void SetStairsPos(Int2 pos) { m_StairsPos = pos; }
	void AddRoomLink(const RoomLink& link){m_RoomLinks.push_back(link);}

	const std::vector<RoomLink>& GetRoomLinks() const{return m_RoomLinks;}
	Int2 GetStairsPos() const { return m_StairsPos; }
	CRoom GetRoom(int i) { return m_Room[i]; }
	const std::vector<std::vector<int>>& GetRoomGraph() const{return m_RoomGraph;}

	//const std::vector<std::vector<int>>& graphはそれぞれの部屋がどの部屋と直通しているか
	//graphがgraph[0] = { 1, 3 };graph[1] = { 0,2 };graph[2] = { 1 };graph[3] = { 0 };
	//の場合0と1、0と3、1と2が繋がっている
	std::vector<int> CalcRoomDistance(int startRoom) const;

	// 座標に部屋があるかを調べる
	// 返り値は部屋の番号
	// 部屋がない場合は -1 を返す
	int GetRoomID(Int2 i);

	// 座標から特定の向きを見たらどこまで見れるかを返す
	int GetFieldOfVision(Int2 i, DIRECTION dir);

	CanMove GetCanMove(Int2 pos);

	//指定された座標のタイルを取得
	TILE GetTile(Int2 i);

	CRoom GetStartRoom();

	//部屋の数を返す
	int GetRoomNum() { return (int)m_Room.size(); }

	// 部屋の中のランダムな座標を取得
	Int2 GetRoomPos();

	// 部屋の保存
	void RoomSave(const CRoom& room);

	// 座標がマップ外かチェック
	// trueならマップ外
	bool InvestigationMapOutside(Int2 i);

	// プレイヤーがいない部屋の中からランダムな座標を取得
	Int2 GetNotHerePlayerRoomPos(Int2 PlPos);

	// 廊下とかぶっているかを判定
	bool CollisionStairs(Int2 i);

	// 部屋どうしがかぶっているかを判定
	bool CollisionRoomToRoom(const CRoom& room);

	//指定されたマスに隣接しているマスに指定されたタイルがあるかどうか調べる
	bool IsAdjacentTile(Int2 pos,TILE tile);


	// 保存されたRoomLinkから部屋グラフを作る
	void CreateRoomGraph();

	std::vector<int> FindRoomRoute(int startRoom,int goalRoom) const;

	const RoomLink* FindRoomLink(int currentRoomID, int nextRoomID);

	Int2 GetStartGate(const RoomLink& link, int currentRoomID) ;

	//現在位置の廊下を調べる
	const RoomLink* FindRoomLinkByPosition(const Int2& position) const;
};
