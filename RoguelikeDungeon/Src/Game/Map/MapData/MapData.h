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
private:
	std::vector<CRoom> GetAllRoom() { return m_Room; }
public:
	void SetTile(Int2 pos, TILE tile) { m_Map[pos.y][pos.x] = tile; }
	void SetStairsPos(Int2 pos) { m_StairsPos = pos; }

	Int2 GetStairsPos() const { return m_StairsPos; }
	CRoom GetRoom(int i) { return m_Room[i]; }

	// 座標に部屋があるかを調べる
	// 返り値は部屋の番号
	// 部屋がない場合は -1 を返す
	int GetRoomNum(Int2 i);

	// 座標から特定の向きを見たらどこまで見れるかを返す
	int GetFieldOfVision(Int2 i, DIRECTION dir);

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
};
