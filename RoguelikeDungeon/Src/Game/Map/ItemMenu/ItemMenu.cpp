#include"ItemMenu.h"
#include"../MapCommon.h"
#include"../../../Lib/Input/Input.h"

static constexpr int ITEM_MENU_FRAME_START_X = 80;
static constexpr int ITEM_MENU_FRAME_START_Y = 80;
static constexpr int ITEM_MENU_FRAME_END_X = 500;
static constexpr int ITEM_MENU_FRAME_END_Y = 500;
static constexpr int ITEM_MENU_DRAW_START_X = 100;
static constexpr int ITEM_NAME_DRAW_START_X = 130;
static constexpr int ITEM_OPERATION_DRAW_STRAT_Y = 460;
static constexpr int ITEM_PAGE_DRAW_START_Y = 410;
static constexpr int ITEM_ITEM_DRAW_START_Y = 100;
static constexpr int ITEM_NAME_DRAW_START_Y = 140;
static constexpr int ITEM_NAME_DRAW_INTERVAL_Y = 24;

void CItemMenu::Init() {
	//アイテムの状況を初期化
	UpdatePage(0);
}

void CItemMenu::UpdatePage(int itemCount) {

	//アイテムが0だった場合処理が変わる
	if (itemCount <= 0) {
		m_MaxPage = 0;
		m_ItemPage = 0;
		m_PageStart = 0;
		m_PageEnd = 0;
		m_SelectItemIndex = 0;
		return;
	}

	//ページ数の最大を決める
	m_MaxPage = (itemCount + ITEM_PER_PAGE - 1) / ITEM_PER_PAGE;

	//ItemPageが最低値より低いか最大値より大きいなら直す
	if (m_ItemPage < 0)
	{
		m_ItemPage = 0;
	}

	if (m_ItemPage >= m_MaxPage)
	{
		m_ItemPage = m_MaxPage - 1;
	}

	//ページの一番上のアイテム番号
	m_PageStart = m_ItemPage * ITEM_PER_PAGE;
	//ページの一番下のアイテム番号
	m_PageEnd = m_PageStart + ITEM_PER_PAGE;

	//アイテムの最大数がページの一番下のアイテム番号より低いなら
	if (m_PageEnd > itemCount)
	{
		//一番下のアイテム番号をアイテムの最大数にする
		m_PageEnd = itemCount;
	}

	//SelectItemIndexが最低値より低いか最大値より大きいなら直す
	if (m_SelectItemIndex < m_PageStart)
	{
		m_SelectItemIndex = m_PageStart;
	}

	if (m_SelectItemIndex >= m_PageEnd)
	{
		m_SelectItemIndex = m_PageEnd - 1;
	}
}

int CItemMenu::StepItemMenu(int itemCount)
{
	//SPACEでキャンセル
	if (IsInputTrg(KEY_SPACE))
	{
		return 1;
	}

	if (itemCount <= 0)
	{
		m_SelectItemIndex = 0;
		m_ItemPage = 0;
		return 0;
	}

	//Aで前のページへ
	if (IsInputTrg(KEY_A))
	{
		m_ItemPage--;

		if (m_ItemPage < 0)
		{
			m_ItemPage = m_MaxPage - 1;
		}

		m_SelectItemIndex = m_ItemPage * ITEM_PER_PAGE;
	}

	//Dで次のページへ
	if (IsInputTrg(KEY_D))
	{
		m_ItemPage++;

		if (m_ItemPage >= m_MaxPage)
		{
			m_ItemPage = 0;
		}

		m_SelectItemIndex = m_ItemPage * ITEM_PER_PAGE;
	}

	//Wで上へ
	if (IsInputTrg(KEY_W))
	{
		m_SelectItemIndex--;

		if (m_SelectItemIndex < m_PageStart)
		{
			m_SelectItemIndex = m_PageEnd - 1;
		}
	}

	//Sで下へ
	if (IsInputTrg(KEY_S))
	{
		m_SelectItemIndex++;

		if (m_SelectItemIndex >= m_PageEnd)
		{
			m_SelectItemIndex = m_PageStart;
		}
	}

	//Kで使用
	if (IsInputTrg(KEY_K))
	{
		//2以上ならアイテムを使用したことになる
		return m_SelectItemIndex + 2;
	}

	return 0;
}

void CItemMenu::DrawItemMenu(const std::vector<Item>& Inventory)
{
	//アイテム画面の枠
	DrawBox(ITEM_MENU_FRAME_START_X, ITEM_MENU_FRAME_START_Y, ITEM_MENU_FRAME_END_X, ITEM_MENU_FRAME_END_Y, GetColor(0, 0, 0), TRUE);
	DrawBox(ITEM_MENU_FRAME_START_X, ITEM_MENU_FRAME_START_Y, ITEM_MENU_FRAME_END_X, ITEM_MENU_FRAME_END_Y, GetColor(255, 255, 255), FALSE);

	DrawFormatString(ITEM_MENU_DRAW_START_X, ITEM_ITEM_DRAW_START_Y, GetColor(255, 255, 255), "ITEM");

	const auto& inventory = Inventory;
	int itemCount = static_cast<int>(inventory.size());

	//アイテムを持っていない時
	if (itemCount <= 0)
	{
		DrawFormatString(ITEM_MENU_DRAW_START_X, ITEM_NAME_DRAW_START_Y, GetColor(255, 255, 255), "アイテムを持っていません");
		DrawFormatString(ITEM_MENU_DRAW_START_X, ITEM_OPERATION_DRAW_STRAT_Y, GetColor(255, 255, 255), "SPACE: 戻る");
		return;
	}

	for (int i = m_PageStart; i < m_PageEnd; i++)
	{
		int drawIndex = i - m_PageStart;
		int y = ITEM_NAME_DRAW_START_Y + drawIndex * ITEM_NAME_DRAW_INTERVAL_Y;

		if (i == m_SelectItemIndex)
		{
			DrawFormatString(ITEM_MENU_DRAW_START_X, y, GetColor(255, 255, 0), ">");
		}

		const char* name = "不明なアイテム";

		switch (inventory[i].type)
		{
		case ITEM_1:
			name = "アイテム1";
			break;
		case ITEM_2:
			name = "アイテム2";
			break;
		case ITEM_3:
			name = "アイテム3";
			break;
		case ITEM_4:
			name = "アイテム4";
			break;
		default:
			break;
		}

		DrawFormatString(ITEM_NAME_DRAW_START_X, y, GetColor(255, 255, 255), "%s", name);
	}

	DrawFormatString(ITEM_MENU_DRAW_START_X, ITEM_PAGE_DRAW_START_Y, GetColor(255, 255, 255), "Page %d / %d", m_ItemPage + 1, m_MaxPage);

	DrawFormatString(ITEM_MENU_DRAW_START_X, ITEM_OPERATION_DRAW_STRAT_Y, GetColor(255, 255, 255), "W/S: 選択  A/D: ページ変更  K: 使用  SPACE: 戻る");
}