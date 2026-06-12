#include"Map.h"
#include"iostream"
#include <DxLib.h>
#include"cmath"
#include <algorithm>
#include <random>
#include "../../Lib/Input/input.h"

using namespace std;

static constexpr int DRAW_LENGTH_X = 12;					//描写するマスの幅
static constexpr int DRAW_LENGTH_Y = 9;

//インベントリの１ページい表示される数
static const int ITEM_PER_PAGE = 10;

enum ItemMenu {
	ITEM_MENU_NORMAL,
	ITEM_MENU_CANSEL,
	ITEM_MENU_USE,
};

CMap* CMap::m_Instance = NULL;

CMap* CMap::GetInstance() {
	//まだ生成されてないなら
	if (m_Instance == NULL)
	{
		m_Instance = new CMap();
	}

	return m_Instance;
}

void CMap::DeleteInstance() {
	//まだ生成されてないなら
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}

}

void CMap::Init() {

	

	m_MapData.Init();





	m_Corridorhndl = -1;
	m_Roomhndl = -1;
	m_Wallhndl = -1;
	m_Stairshndl = -1;
	m_SelectItemIndex = 0;
	m_ItemPage = 0;
}

void CMap::Load() {
	if (m_Corridorhndl == -1)
	{
		m_Corridorhndl = MV1LoadModel("Data/Model/Corridor.x");
		MV1SetScale(m_Corridorhndl, VGet(0.5f, 0.5f, 0.5f));
	}

	if(m_Roomhndl == -1)
	{
		m_Roomhndl = MV1LoadModel("Data/Model/Room.x");
		MV1SetScale(m_Roomhndl, VGet(0.5f, 0.5f, 0.5f));
	}

	if (m_Stairshndl == -1)
	{
		m_Stairshndl = MV1LoadModel("Data/Model/STAIRS.x");
		MV1SetScale(m_Stairshndl, VGet(0.5f, 0.5f, 0.5f));
	}

	if (m_Wallhndl == -1)
	{
		m_Wallhndl = MV1LoadModel("Data/Model/WALL.x");
		MV1SetScale(m_Wallhndl, VGet(0.5f, 0.5f, 0.5f));
	}
}

void CMap::Exit() {
	m_MapData.Exit();


	if(m_Corridorhndl != -1) {
		MV1DeleteModel(m_Corridorhndl);
		m_Corridorhndl = -1;
	}
	if (m_Roomhndl != -1) {
		MV1DeleteModel(m_Roomhndl);
		m_Roomhndl = -1;
	}
	if (m_Wallhndl != -1) {
		MV1DeleteModel(m_Wallhndl);
		m_Wallhndl = -1;
	}
	if (m_Stairshndl != -1) {
		MV1DeleteModel(m_Stairshndl);
		m_Stairshndl = -1;
	}
}


bool CMap::CollisionItemToItem(CFieldItem& item){

	// まず、現在の座標にアイテムがなければそのまま置ける
	if (m_FieldItemManager.IsItemExist(item.GetPos()) == ITEM_NON)
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
		if (m_MapData.GetTile(next) != TILE_ROOM)
		{
			continue;
		}

		//そこにもアイテムが落ちていたら置けない
		if (m_FieldItemManager.IsItemExist(next) != ITEM_NON)
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

void CMap::DrawTileCube(int mapX, int mapY, int color, float height)
{


	float x = -mapX * TILE_SIZE;
	float z = mapY * TILE_SIZE;

	VECTOR pos1 = VGet(x - 50.0f, height, z - 50.0f);
	VECTOR pos2 = VGet(x + 50.0f, height + 100.0f, z + 50.0f);

	DrawCube3D(pos1, pos2, color, color, TRUE);
}

void CMap::Draw(Int2 playerPos) {

	//プレイヤーの近くだけ描写
	for (int i = playerPos.y - DRAW_LENGTH_Y;i < playerPos.y + DRAW_LENGTH_Y;i++)
	{
		for (int k = playerPos.x - DRAW_LENGTH_X;k < playerPos.x + DRAW_LENGTH_X;k++)
		{

			int tile = TILE_WALL;
			Int2 nextPos = { k,i };

			// 配列内なら実際のマップを参照
			if (InvestigationMapOutside(nextPos) == false)
			{
				tile = m_MapData.GetTile(nextPos);
			}

			switch (tile)
			{
			case TILE_WALL:
				MV1SetPosition(m_Wallhndl, VGet(-k * TILE_SIZE, 150, i * TILE_SIZE));
				MV1DrawModel(m_Wallhndl);
				break;

			case TILE_ROOM:
				MV1SetPosition(m_Roomhndl, VGet(-k * TILE_SIZE, 150, i * TILE_SIZE));
				MV1DrawModel(m_Roomhndl);
				break;

			case TILE_CORRIDOR:
				MV1SetPosition(m_Corridorhndl, VGet(-k * TILE_SIZE, 150, i * TILE_SIZE));
				MV1DrawModel(m_Corridorhndl);
				break;

			default:
				break;
			}
		}
	}

	//階段の描画
	int centerX = 8 + 16 * GetStairsPos().x;
	int centerY = 8 + 16 * GetStairsPos().y;
	//DrawBox(centerX + 8, centerY + 8, centerX - 8, centerY - 8, GetColor(255, 0, 0), TRUE);

	float x = -GetStairsPos().x * TILE_SIZE;
	float z = GetStairsPos().y * TILE_SIZE;

	VECTOR pos1 = VGet(x - 50.0f, 150, z - 50.0f);
	VECTOR pos2 = VGet(x + 50.0f, 150 + 100.0f, z + 50.0f);
	//DrawCube3D(pos1, pos2, GetColor(0, 0, 128), GetColor(128, 128, 128), TRUE);
	MV1SetPosition(m_Stairshndl, VGet(x, 150, z));
	MV1DrawModel(m_Stairshndl);

	m_FieldItemManager.Draw();



						
}

bool CMap::InvestigationMapOutside(Int2 i) {
	if (i.y >= 0 && i.y < MAP_Y && i.x >= 0 && i.x < MAP_X) return false;

	return true;
}


void CMap::CreateItem(int CreateNum, int x, int y)
{
	m_FieldItemManager.CreateItem(CreateNum,  x,  y);
}

void CMap::DeleteAll() {
	Exit();
	Init();
}

int CMap::StepItemMenu(int itemCount)
{
	

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

	int maxPage = (itemCount + ITEM_PER_PAGE - 1) / ITEM_PER_PAGE;

	// Aで前のページへ
	if (IsInputTrg(KEY_A))
	{
		m_ItemPage--;

		if (m_ItemPage < 0)
		{
			m_ItemPage = maxPage - 1;
		}

		m_SelectItemIndex = m_ItemPage * ITEM_PER_PAGE;
	}

	// Dで次のページへ
	if (IsInputTrg(KEY_D))
	{
		m_ItemPage++;

		if (m_ItemPage >= maxPage)
		{
			m_ItemPage = 0;
		}

		m_SelectItemIndex = m_ItemPage * ITEM_PER_PAGE;
	}

	int pageStart = m_ItemPage * ITEM_PER_PAGE;
	int pageEnd = pageStart + ITEM_PER_PAGE;

	if (pageEnd > itemCount)
	{
		pageEnd = itemCount;
	}

	// Wで上へ
	if (IsInputTrg(KEY_W))
	{
		m_SelectItemIndex--;

		if (m_SelectItemIndex < pageStart)
		{
			m_SelectItemIndex = pageEnd - 1;
		}
	}

	// Sで下へ
	if (IsInputTrg(KEY_S))
	{
		m_SelectItemIndex++;

		if (m_SelectItemIndex >= pageEnd)
		{
			m_SelectItemIndex = pageStart;
		}
	}

	// Kで使用
	if (IsInputTrg(KEY_K))
	{

		return m_SelectItemIndex + 2;
	}

	return 0;
}

void CMap::DrawItemMenu(const std::vector<Item>& Inventory)
{
	DrawBox(80, 80, 500, 500, GetColor(0, 0, 0), TRUE);
	DrawBox(80, 80, 500, 500, GetColor(255, 255, 255), FALSE);

	DrawFormatString(100, 100, GetColor(255, 255, 255), "ITEM");

	const auto& inventory = Inventory;
	int itemCount = static_cast<int>(inventory.size());

	if (itemCount <= 0)
	{
		DrawFormatString(100, 140, GetColor(255, 255, 255), "アイテムを持っていません");
		DrawFormatString(100, 460, GetColor(255, 255, 255), "SPACE: 戻る");
		return;
	}

	int maxPage = (itemCount + ITEM_PER_PAGE - 1) / ITEM_PER_PAGE;

	int pageStart = m_ItemPage * ITEM_PER_PAGE;
	int pageEnd = pageStart + ITEM_PER_PAGE;

	if (pageEnd > itemCount)
	{
		pageEnd = itemCount;
	}

	for (int i = pageStart; i < pageEnd; i++)
	{
		int drawIndex = i - pageStart;
		int y = 140 + drawIndex * 24;

		if (i == m_SelectItemIndex)
		{
			DrawFormatString(100, y, GetColor(255, 255, 0), ">");
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

		DrawFormatString(130, y, GetColor(255, 255, 255), "%s", name);
	}

	DrawFormatString(100, 410, GetColor(255, 255, 255), "Page %d / %d", m_ItemPage + 1, maxPage);

	DrawFormatString(100, 460, GetColor(255, 255, 255), "W/S: 選択  A/D: ページ変更  K: 使用  SPACE: 戻る");
}

void CMap::CreateFloor() {
	

	//マップを消去
	Init();
	//プレイヤー以外のオブジェクトを削除

	Load();

	//3個から5個の部屋を作成
	if (CreateRoom(GetRand(ROOM_MAX - ROOM_MIN) + ROOM_MIN) == false)return;
	m_MapData.CreateCorridor();
	CreateStairs();
	CreateItem(STRAT_ITEM_NUM);

}