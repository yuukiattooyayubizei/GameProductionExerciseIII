#pragma once
#include "../../Item/ItemModelManager.h"
#include "../../Item/FieldItem/FieldItem.h"

class CFieldItemManager {
	private:
	// 落ちているアイテム
	std::vector<CFieldItem> m_Item;
	CItemModelManager m_ItemManager;
public:
	void Init();
	void Load();
	void Draw();
	void Exit();

public:

	ITEM_TYPE IsItemExist(Int2 i);

	// アイテムが同じ座標にあるかチェック
	bool CollisionItem(Int2 i);

	// 指定した座標のアイテムを消去
	void EraseItem(Int2 pos);

	// 床落ちアイテムを作成
	// xとyを入力したらその座標に、
	// しなかったら置けるランダムなマスから選択
	void CreateItem(int CreateNum, int x = -1, int y = -1);

	// アイテムどうしが同じ座標にあるかチェック
	bool CollisionItemToItem(CFieldItem& item);
};