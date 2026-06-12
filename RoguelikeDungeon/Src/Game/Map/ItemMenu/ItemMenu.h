#pragma once
#include <vector>
#include "../MapCommon.h"
#include "../../Item/Item.h"


class CItemMenu {
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