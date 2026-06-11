#pragma once
#include <vector>
#include<DxLib.h>
#include "../common.h"
#include "../Item/FieldItem/FieldItem.h"
#include"Room/Room.h"
#include "MapCommon.h"
#include "MapData/MapData.h"
#include "FieldItemManager/FieldItemManager.h"


class CMap {
private:
	static CMap* m_Instance;

public:
	static CMap* GetInstance();
	static void DeleteInstance();

private:
	//// 落ちているアイテム
	//std::vector<CFieldItem> m_Item;
	//CItemModelManager m_ItemManager;

	CMapData m_MapData;

	CFieldItemManager m_FieldItemManager;

	int m_Corridorhndl = -1;
	int m_Roomhndl = -1;
	int m_Wallhndl = -1;
	int m_Stairshndl = -1;

	//アイテム選択時どれを選んでいるか
	int m_SelectItemIndex;
	//アイテム選択時のぺージ
	int m_ItemPage;
public:
	CMap() { Init(); }

	//----------------------------------------------
	// 基本処理
	//----------------------------------------------

	void Init();

	void Load();

	void DrawTileCube(int mapX, int mapY, int color, float height);

	// 描画
	void Draw(Int2 playerPos);

	void DrawItemMenu(const std::vector<Item>& Inventory);

	int StepItemMenu(int itemCount);

	void Exit();

	// 全部消す
	void DeleteAll();

	//階層を生成
	void CreateFloor();

	//----------------------------------------------
	// 取得系
	//----------------------------------------------

	// 部屋の中のランダムな座標を取得
	Int2 GetRoomPos();

	// 階段の座標を返す
	Int2 GetStairsPos() const { return m_MapData.GetStairsPos(); }
	void SetStairsPos(Int2 pos) { m_MapData.SetStairsPos(pos); }

	TILE GetTile(Int2 i);

	CRoom GetStartRoom();

	void SetSelectItemIndex(int i = 0) { m_SelectItemIndex = i; }

	void SetItemPage(int i = 0) { m_ItemPage = i; }

	// 座標に部屋があるかを調べる
// 返り値は部屋の番号
// 部屋がない場合は -1 を返す
	int GetRoomNum(Int2 i);

	// 座標から特定の向きを見たらどこまで見れるかを返す
	int GetFieldOfVision(Int2 i, DIRECTION dir);

	//----------------------------------------------
	// 判定系
	//----------------------------------------------

	// 座標がマップ外かチェック
	// trueならマップ外
	bool InvestigationMapOutside(Int2 i);

	// 部屋どうしがかぶっているかを判定
	bool CollisionRoomToRoom(const CRoom& room);

	// 廊下とかぶっているかを判定
	bool CollisionStairs(Int2 i);

	//----------------------------------------------
	// 部屋・階段生成系
	//----------------------------------------------

	// 部屋を作成
	bool CreateRoom(int CreateNum);

	// 部屋のサイズ決定
	CRoom RoomSizeDecision();

	// 部屋の保存
	void RoomSave(const CRoom& room);

	// 階段を作成
	void CreateStairs();

	// 掘る
	void DigCorridor(Int2 a, Int2 b);

	//----------------------------------------------
	// アイテム関連
	//----------------------------------------------

	// 床落ちアイテムを作成
	// xとyを入力したらその座標に、
	// しなかったら置けるランダムなマスから選択
	void CreateItem(int CreateNum, int x = -1, int y = -1);

	// 探している座標にアイテムがあるかどうか
	ITEM_TYPE IsItemExist(Int2 i);

	// アイテムどうしが同じ座標にあるかチェック
	bool CollisionItemToItem(CFieldItem& item);

	// アイテムが同じ座標にあるかチェック
	bool CollisionItem(Int2 i);

	// 指定した座標のアイテムを消去
	void EraseItem(Int2 pos);

	//----------------------------------------------
	// 廊下関連
	//----------------------------------------------

	// 廊下の生成
	bool CreateCorridor();

	// 一番近い部屋を特定
	SpecifiedRoomInformation SpecifiedRoom(const CRoom& room);

	// 廊下につながる部屋のマスを決定
	CorridorInfo ConnectHallwayToRoom(const CRoom& room,SpecifiedRoomInformation close);
};