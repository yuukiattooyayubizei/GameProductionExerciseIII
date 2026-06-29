#pragma once
#include<vector>
#include"../../Item/Item.h"

class CItemMenu {
private:
	static constexpr int ITEM_MENU_FRAME_START_X = 80;
	static constexpr int ITEM_MENU_FRAME_START_Y = 80;
	static constexpr int ITEM_MENU_FRAME_END_X = 500;
	static constexpr int ITEM_MENU_FRAME_END_Y = 500;
	static constexpr int ITEM_MENU_DESCRIPTION_FRAME_START_X = 510;
	static constexpr int ITEM_MENU_DESCRIPTION_FRAME_START_Y = 180;
	static constexpr int ITEM_MENU_DESCRIPTION_FRAME_END_X = 810;
	static constexpr int ITEM_MENU_DESCRIPTION_FRAME_END_Y = 320;
	static constexpr int ITEM_MENU_DRAW_START_X = 100;
	static constexpr int ITEM_NAME_DRAW_START_X = 130;
	static constexpr int ITEM_OPERATION_DRAW_STRAT_Y = 460;
	static constexpr int ITEM_PAGE_DRAW_START_Y = 410;
	static constexpr int ITEM_ITEM_DRAW_START_Y = 100;
	static constexpr int ITEM_NAME_DRAW_START_Y = 140;
	static constexpr int ITEM_NAME_DRAW_INTERVAL_Y = 24;
	static constexpr int ITEM_DESCRIPTION_DRAW_START_X = 530;
	static constexpr int ITEM_DESCRIPTION_DRAW_START_Y = 200;
private:
	//アイテム選択時どれを選んでいるか
	int m_SelectItemIndex;
	//アイテム選択時のぺージ
	int m_ItemPage;

	//ページ数の最大を決める
	int m_MaxPage;
	//ページの一番上のアイテム番号
	int m_PageStart;
	//ページの一番下のアイテム番号
	int m_PageEnd;
public:
	void Init();

	void UpdatePage(int itemCount);

	void DrawItemMenu(const std::vector<Item>& Inventory);

	int StepItemMenu(int itemCount);

	void SetSelectItemIndex(int i) { m_SelectItemIndex = i; }
	void SetItemPage(int i) { m_ItemPage = i; }
};