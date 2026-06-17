#include"UI.h"
#include <DxLib.h>
#include "Log.h"

void CUI::Init() {
	for (int i = 0;i < LOG_LINE_NUM;i++) {
		m_Log[i] = "";
	}
}

void CUI::Draw(int hp,int mhp) {
	//ÉçÉOâÊñ ÇÃòg
	CLog::GetInstance()->Draw();

	//HPÉQÅ[ÉWçÏê¨
	float GaugeSize = DRAW_GAUGE_SIZE_X * (static_cast<float>(hp) / mhp);
	DrawBox(DRAW_HP_X, DRAW_BOX_HP_START_Y, DRAW_HP_END_X, DRAW_BOX_HP_END_Y, GetColor(255, 255, 255), TRUE);
	DrawBox(DRAW_HP_X, DRAW_BOX_HP_START_Y, DRAW_HP_X + GaugeSize, DRAW_BOX_HP_END_Y, GetColor(255, 0, 0), TRUE);

	DrawFormatString(DRAW_HP_X, DRAW_STRING_HP_Y, GetColor(255, 255, 255), "HP = %d / %d", hp , mhp);
}