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
	int GetRoomNum(Int2 pos) { return m_MapData.GetRoomNum(pos); }									//座標の部屋のIDを返す
	CRoom GetStartRoom() { return m_MapData.GetStartRoom(); }										//始めの部屋を返す
	Int2 GetRoomPos() { return m_MapData.GetRoomPos(); }											//部屋の中のランダムな座標を取得
	Int2 GetNotHerePlayerRoomPos(Int2 PlPos) { return m_MapData.GetNotHerePlayerRoomPos(PlPos); }	//部屋の中のランダムな座標を取得


	void SetSelectItemIndex(int i = 0) { m_ItemMenu.SetSelectItemIndex(i); }
	void SetItemPage(int i = 0) { m_ItemMenu.SetItemPage(i); }
	void SetStairsPos(Int2 pos) { m_MapData.SetStairsPos(pos); }


	//----------------------------------------------
	// 判定系
	//----------------------------------------------
	
	// 座標がマップ外かチェック
	// trueならマップ外
	bool InvestigationMapOutside(Int2 i) { return m_MapData.InvestigationMapOutside(i); }
	// アイテムどうしが同じ座標にあるかチェック
	bool CollisionItemToItem(CFieldItem& item) { return m_FieldItemManager.CollisionItemToItem(m_MapData, item); }
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