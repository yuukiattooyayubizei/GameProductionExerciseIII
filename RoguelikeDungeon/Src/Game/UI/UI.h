#pragma once
#include <iostream>
#include <string>

class CUI {
private:
	static constexpr int LOG_LINE_NUM = 5;
	static constexpr int DRAW_HP_X = 448;
	static constexpr float DRAW_GAUGE_SIZE_X = 480.0f;
	static constexpr int DRAW_HP_END_X = 928;
	static constexpr int DRAW_STRING_HP_Y = 32;
	static constexpr int DRAW_BOX_HP_START_Y = 80;
	static constexpr int DRAW_BOX_HP_END_Y = 112;
private:
	std::string m_Log[LOG_LINE_NUM];
public:

	void Init();
	void Draw(int hp, int mhp);
};
