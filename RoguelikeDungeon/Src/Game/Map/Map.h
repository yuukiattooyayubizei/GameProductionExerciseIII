#pragma once
#include <vector>
#include<DxLib.h>
#include "../common.h"
#include "../Item/FieldItem/FieldItem.h"
#include"Room/Room.h"
#include "MapCommon.h"
#include "MapData/MapData.h"
#include "FieldItemManager/FieldItemManager.h"
#include "MapCreate/MapCreate.h"
#include "ItemMenu/ItemMenu.h"
#include "MapDraw/MapDraw.h"


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


	void CreateItem(int CreateNum, int x = -1, int y = -1) { m_FieldItemManager.CreateItem(m_MapData,CreateNum, x, y); }
	// 廊下とかぶっているかを判定
	bool CollisionStairs(Int2 i) { return m_MapData.CollisionStairs(i); }
	ITEM_TYPE IsItemExist(Int2 i) { return m_FieldItemManager.IsItemExist(i); }
	// 指定した座標のアイテムを消去
	void EraseItem(Int2 pos) { m_FieldItemManager.EraseItem(pos); }
	// アイテムが同じ座標にあるかチェック
	bool CollisionItem(Int2 i) { return m_FieldItemManager.CollisionItem(i); }
	// 部屋の中のランダムな座標を取得
	Int2 GetRoomPos() { return m_MapData.GetRoomPos(); }
	void DrawItemMenu(const std::vector<Item>& Inventory) { m_ItemMenu.DrawItemMenu(Inventory); }

	int StepItemMenu(int itemCount) { return m_ItemMenu.StepItemMenu(itemCount); }
	void UpDateItemMenu(int itemCount) { m_ItemMenu.UpdatePage(itemCount); }
	// 座標がマップ外かチェック
	// trueならマップ外
	bool InvestigationMapOutside(Int2 i) { return m_MapData.InvestigationMapOutside(i); }

	// アイテムどうしが同じ座標にあるかチェック
	bool CollisionItemToItem(CFieldItem& item) { return m_FieldItemManager.CollisionItemToItem(m_MapData, item); }

	CMap() { Init(); }



	//----------------------------------------------
	// 基本処理
	//----------------------------------------------

	void Init();

	void Load();

	// 描画
	void Draw(Int2 playerPos);

	void Exit();

	// 全部消す
	void DeleteAll();

	//階層を生成
	void CreateFloor();

	//----------------------------------------------
	// 取得系
	//----------------------------------------------

	// 階段の座標を返す
	Int2 GetStairsPos() const { return m_MapData.GetStairsPos(); }
	TILE GetTile(Int2 pos) { return m_MapData.GetTile(pos); }
	int GetRoomNum(Int2 pos) { return m_MapData.GetRoomNum(pos); }
	CRoom GetStartRoom() { return m_MapData.GetStartRoom(); }
	void SetSelectItemIndex(int i = 0) { m_ItemMenu.SetSelectItemIndex(i); }
	void SetItemPage(int i = 0) { m_ItemMenu.SetItemPage(i); }
	void SetStairsPos(Int2 pos) { m_MapData.SetStairsPos(pos); }

	//----------------------------------------------
	// 判定系
	//----------------------------------------------

	//----------------------------------------------
	// 部屋・階段生成系
	//----------------------------------------------

	//----------------------------------------------
	// アイテム関連
	//----------------------------------------------


	//----------------------------------------------
	// 廊下関連
	//----------------------------------------------
};