#include"Map.h"

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
	m_FieldItemManager.Init();
	m_ItemMenu.Init();
}

void CMap::Load() {
	m_FieldItemManager.Load();
	m_MapDraw.Load();
}

void CMap::Exit() {
	m_MapData.Exit();
	m_MapDraw.Exit();
	m_FieldItemManager.Exit();
}

void CMap::Draw(Int2 playerPos) {
	m_MapDraw.Draw(m_MapData, playerPos);
	m_FieldItemManager.Draw();					
	m_MapData.GetRoomLinks();
}

void CMap::DeleteAll() {
	Exit();
	Init();
}

void CMap::CreateFloor() {
	m_MapCreate.CreateFloor(m_MapData);
	m_FieldItemManager.EraseAllItem();
	CreateItem(STRAT_ITEM_NUM);
}

bool CMap::IsVisibleFrom(const Int2 PPos, const  Int2 EPos){
	// 隣接している場合は廊下や部屋に関係なく見える
	if (IsAdjacentInt2(PPos, EPos))
	{
		return true;
	}

	//プレイヤーがどの部屋にいるか取得
	const int PID = GetRoomID(PPos);

	// 見る側が廊下にいる場合、隣接マス以外は見えない
	if (PID == -1)
	{
		return false;
	}

	//敵がどの部屋にいるか取得
	const int EID = GetRoomID(EPos);

	if (PID == EID)return true;

	return false;
}