#include"FieldItemManager.h"
#include <algorithm>
#include "../MapCommon.h"
#include "../MapData/MapData.h"
#include <iostream>

void CFieldItemManager::Init() {
	m_Item.clear();
}

void CFieldItemManager::Load() {
	m_ItemManager.LoadModel();
}

void CFieldItemManager::Draw() {
	//アイテムの描画
	for_each(m_Item.begin(), m_Item.end(), [this](CFieldItem item) {
		item.Draw(m_ItemManager);
		});
}

void CFieldItemManager::Exit() {
	m_Item.clear();
}

ITEM_TYPE CFieldItemManager::IsItemExist(Int2 i) {
	for (const CFieldItem& fieldItem : m_Item)
	{
		if (fieldItem.GetPos().x == i.x && fieldItem.GetPos().y == i.y)
		{
			return fieldItem.GetType();
		}
	}
	return ITEM_NON;
};

bool CFieldItemManager::CollisionItem(Int2 i) {

	for (const CFieldItem& fieldItem : m_Item)
	{
		if (fieldItem.GetPos().x == i.x && fieldItem.GetPos().y == i.y)
		{
			return true;
		}
	}
	return false;
};

void CFieldItemManager::EraseItem(Int2 pos) {
	int i = 0;
	for (const CFieldItem& fieldItem : m_Item)
	{
		if (fieldItem.GetPos().x == (int)pos.x && fieldItem.GetPos().y == (int)pos.y)
		{
			m_Item.erase(m_Item.begin() + i);
		}
		i++;
	}
}

void CFieldItemManager::CreateItem(int CreateNum, int x, int y)
{
	for (int index = 0;index < CreateNum;index++)
	{
		CFieldItem item{};

		if (x == -1 && y == -1)
		{
			//ランダムな部屋マスを取得
			Int2 pos = GetRoomPos();
			//エラーの場合-1が帰ってくる
			if (pos.x == -1)
				return;

			//座標を入力
			item.SetPos(pos);
		}
		else
		{
			//座標を入力
			Int2 pos{};
			pos.x = x;
			pos.y = y;
			item.SetPos(pos);
		}

		// 置けなかった場合は追加しない
		if (CollisionStairs(item.GetPos()))
			continue;
		if (!CollisionItemToItem(item))
			continue;

		//アイテムの種類をランダムで決定
		int i = GetRand(ITEM_NUM - 1);
		item.SetType(static_cast<ITEM_TYPE>(i));

		std::cout << item.GetPos().x << "," << item.GetPos().y << "にアイテムを生成" << std::endl;
		m_Item.push_back(item);
	}
}

bool CFieldItemManager::CollisionItemToItem(CFieldItem& item) {

	// まず、現在の座標にアイテムがなければそのまま置ける
	if (IsItemExist(item.GetPos()) == ITEM_NON)
	{
		return true;
	}

	//アイテムの位置が被っていた場合1マス移動させる
	// 優先順位
	// ⑤③⑥
	// ①〇②
	// ⑦④⑧
	const Int2 movePos[] =
	{
		{-1,  0},	//左
		{ 1,  0},	//右
		{ 0, -1},	//上
		{ 0,  1},	//下
		{-1, -1},	//左上
		{ 1, -1},	//右上
		{-1,  1},	//左下
		{ 1,  1},	//右下
	};

	for (const Int2& move : movePos)
	{
		//調べる座標を決定
		int nextX = item.GetPos().x + move.x;
		int nextY = item.GetPos().y + move.y;
		Int2 next;
		next.x = nextX;
		next.y = nextY;

		//マップの範囲外は置けない
		if (nextX < 0 || nextX >= MAP_X ||
			nextY < 0 || nextY >= MAP_Y)
		{
			continue;
		}

		//部屋以外のマスには置けない
		if (GetTile(next) != TILE_ROOM)
		{
			continue;
		}

		//そこにもアイテムが落ちていたら置けない
		if (IsItemExist(next) != ITEM_NON)
		{
			continue;
		}

		//ここまで来たらそのマスには置けるので移動
		item.SetPos(next);

		return true;
	}

	//周囲8マスすべて置けなかったらfalseを返す
	return false;
}