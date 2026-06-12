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

	// 階段の座標を返す
	Int2 GetStairsPos() const { return m_MapData.GetStairsPos(); }
	void SetStairsPos(Int2 pos) { m_MapData.SetStairsPos(pos); }

	void SetSelectItemIndex(int i = 0) { m_SelectItemIndex = i; }

	void SetItemPage(int i = 0) { m_ItemPage = i; }


	//----------------------------------------------
	// 判定系
	//----------------------------------------------

	// 座標がマップ外かチェック
	// trueならマップ外
	bool InvestigationMapOutside(Int2 i);

	//----------------------------------------------
	// 部屋・階段生成系
	//----------------------------------------------

	//----------------------------------------------
	// アイテム関連
	//----------------------------------------------

	// 床落ちアイテムを作成
	// xとyを入力したらその座標に、
	// しなかったら置けるランダムなマスから選択
	void CreateItem(int CreateNum, int x = -1, int y = -1);

	// アイテムどうしが同じ座標にあるかチェック
	bool CollisionItemToItem(CFieldItem& item);

	//----------------------------------------------
	// 廊下関連
	//----------------------------------------------
};