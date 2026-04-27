#include"Trap1.h"
#include "../../../Data/Data.h"

static constexpr char TRAP_ANIM_HNDL[128] = "../Data/Image/BBass.PNG";

CTrap1::CTrap1() {
	CData* Data = CData::GetInstance();
	m_TrapData = Data->GetTrapData();

	m_Pos.y = GetRand(720);
	m_Pos.x = 1080;
	m_Pos.z = 0;
	m_Kind = KIND_TRAP1;



		m_hndl = -1;
	m_Size = 30;
	Load();
}

CTrap1::~CTrap1() {

}

void CTrap1::Init() {

}

void CTrap1::Load() {


		
		m_hndl = LoadGraph(TRAP_ANIM_HNDL);




	
}

void CTrap1::Step() {

	CData* Data = CData::GetInstance();
	m_Pos.x -= m_TrapData.m_FallingSpd * Data->GetSpd();


}

void CTrap1::Draw() {
	DrawBox(m_Pos.x + m_Size /2, m_Pos.y + m_Size / 2, m_Pos.x - m_Size / 2, m_Pos.y - m_Size / 2, GetColor(255, 0, 0), TRUE);
	DrawRotaGraph(m_Pos.x, m_Pos.y, 1.0f, 0.0f, m_hndl, TRUE);
}

void CTrap1::Exit() {

}