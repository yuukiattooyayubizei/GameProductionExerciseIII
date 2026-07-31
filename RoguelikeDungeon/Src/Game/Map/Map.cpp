#include"Map.h"

CMap* CMap::m_Instance = NULL;

CMap* CMap::GetInstance() {
	//Ç‹Çæê∂ê¨Ç≥ÇÍÇƒÇ»Ç¢Ç»ÇÁ
	if (m_Instance == NULL)
	{
		m_Instance = new CMap();
	}

	return m_Instance;
}

void CMap::DeleteInstance() {
	//Ç‹Çæê∂ê¨Ç≥ÇÍÇƒÇ»Ç¢Ç»ÇÁ
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