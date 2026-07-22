#pragma once
#include<vector>
#include"../Common.h"
#include"../Item/FieldItem/FieldItem.h"
#include"Room/Room.h"
#include"MapCommon.h"
#include"MapData/MapData.h"
#include"FieldItemManager/FieldItemManager.h"
#include"MapCreate/MapCreate.h"
#include"ItemMenu/ItemMenu.h"
#include"MapDraw/MapDraw.h"

class CMap {
private:
	static CMap* m_Instance;
public:
	static CMap* GetInstance();
	static void DeleteInstance();
private:
	CMapData m_MapData;
	CMapDraw m_MapDraw;
	CMapCreate m_MapCreate;
	CItemMenu m_ItemMenu;
	CFieldItemManager m_FieldItemManager;
public:
	CMap() { Init(); }

	//----------------------------------------------
	// 基本処理
	//----------------------------------------------

	void Init();
	void Load();
	void Draw(Int2 playerPos);
	void Exit();

	// 全部消す
	void DeleteAll();
	//階層を生成
	void CreateFloor();

	//----------------------------------------------
	// 取得系
	//----------------------------------------------

	Int2 GetStairsPos() const { return m_MapData.GetStairsPos(); }									//階段の座標を返す
	TILE GetTile(Int2 pos) { return m_MapData.GetTile(pos); }										//指定された座標のタイルの種類を返す
	int GetRoomID(Int2 pos) { return m_MapData.GetRoomID(pos); }									//座標の部屋のIDを返す
	Int2 GetNotHerePlayerRoomPos(Int2 PlPos) { return m_MapData.GetNotHerePlayerRoomPos(PlPos); }	//部屋の中のプレイヤーがいないランダムな座標を取得
	int GetRoomNum() { return m_MapData.GetRoomNum(); }												//部屋の数を取得

	void SetSelectItemIndex(int i = 0) { m_ItemMenu.SetSelectItemIndex(i); }
	void SetItemPage(int i = 0) { m_ItemMenu.SetItemPage(i); }
	int GetFieldOfVision(Int2 i, DIRECTION dir) { return m_MapData.GetFieldOfVision(i,dir); }
	std::vector<int> CalcRoomDistance(int startRoom) const{return m_MapData.CalcRoomDistance(startRoom);}
	std::vector<int> FindRoomRoute(int startRoom,int goalRoom) const{return m_MapData.FindRoomRoute(startRoom,goalRoom);}
	const RoomLink* FindRoomLink(int currentRoomID, int nextRoomID) { return m_MapData.FindRoomLink(currentRoomID,nextRoomID); }
	const RoomLink* FindRoomLinkByPosition(const Int2& position) const { return m_MapData.FindRoomLinkByPosition(position); }

	//----------------------------------------------
	// 判定系
	//----------------------------------------------

	// 廊下とかぶっているかを判定
	bool CollisionStairs(Int2 i) { return m_MapData.CollisionStairs(i); }

	//----------------------------------------------
	// アイテム関連
	//----------------------------------------------

	void CreateItem(int CreateNum, int x = -1, int y = -1) { m_FieldItemManager.CreateItem(m_MapData, CreateNum, x, y); }
	// 指定した座標のアイテムを消去
	void EraseItem(Int2 pos) { m_FieldItemManager.EraseItem(pos); }
	// アイテムが同じ座標にあるかチェック
	bool CollisionItem(Int2 i) { return m_FieldItemManager.CollisionItem(i); }
	void DrawItemMenu(const std::vector<Item>& Inventory) { m_ItemMenu.DrawItemMenu(Inventory); }
	int StepItemMenu(int itemCount) { return m_ItemMenu.StepItemMenu(itemCount); }
	void UpDateItemMenu(int itemCount) { m_ItemMenu.UpdatePage(itemCount); }
	ITEM_TYPE IsItemExist(Int2 i) { return m_FieldItemManager.IsItemExist(i); }
};