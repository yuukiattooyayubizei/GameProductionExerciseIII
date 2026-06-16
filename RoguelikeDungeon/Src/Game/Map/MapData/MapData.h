#pragma once
#include"../Room/Room.h"
#include<vector>
#include"../MapCommon.h"

class CMapData {
private:
	// 階段の座標
	Int2 m_StairsPos = {};
	// マップチップ
	TILE m_Map[MAP_Y][MAP_X] = {};

	// 部屋
	std::vector<CRoom> m_Room;
public:
	void Init();
	void Draw();
	void Exit();
public:
	void SetTile(Int2 pos, TILE tile) { m_Map[pos.y][pos.x] = tile; }
	TILE GetTile(Int2 pos) const { return m_Map[pos.y][pos.x]; }

	Int2 GetStairsPos() const { return m_StairsPos; }
	void SetStairsPos(Int2 pos) { m_StairsPos = pos; }

	CRoom GetRoom(int i) { return m_Room[i]; }

	// 座標に部屋があるかを調べる
	// 返り値は部屋の番号
	// 部屋がない場合は -1 を返す
	int GetRoomNum(Int2 i);

	// 座標から特定の向きを見たらどこまで見れるかを返す
	int GetFieldOfVision(Int2 i, DIRECTION dir);

	TILE GetTile(Int2 i);

	CRoom GetStartRoom();

	//部屋の数を返す
	int GetRoomNum() { return (int)m_Room.size(); }

	// 部屋の中のランダムな座標を取得
	Int2 GetRoomPos();

	// 部屋の保存
	void RoomSave(const CRoom& room);

	// 部屋どうしがかぶっているかを判定
	bool CollisionRoomToRoom(const CRoom& room);

	// 掘る
	void DigCorridor(Int2 a, Int2 b);

	// 座標がマップ外かチェック
	// trueならマップ外
	bool InvestigationMapOutside(Int2 i);

	// 廊下につながる部屋のマスを決定
	CorridorInfo ConnectHallwayToRoom(const CRoom& room, SpecifiedRoomInformation close);

	// 一番近い部屋を特定
	SpecifiedRoomInformation SpecifiedRoom(const CRoom& room);

	// 廊下とかぶっているかを判定
	bool CollisionStairs(Int2 i);
};
