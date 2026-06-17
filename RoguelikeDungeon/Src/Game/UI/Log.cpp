#include"Log.h"
#include <iostream>
#include<DxLib.h>

CLog* CLog::m_Instance = NULL;

CLog* CLog::GetInstance() {
	//まだ生成されてないなら
	if (m_Instance == NULL)
	{
		m_Instance = new CLog();
	}

	return m_Instance;
}

void CLog::DeleteInstance() {
	//まだ生成されてないなら
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}

void CLog::Init() {
	for (int i = 0;i < LOG_LINE_NUM;i++) {
		m_Log[i] = "";
	}
}

void CLog::Draw() {
	DrawBox(0, 0, 960, 160, GetColor(0, 0, 0), TRUE);
	DrawBox(0, 0, 960, 160, GetColor(255, 255, 255), FALSE);
	for (int i = 0;i < LOG_LINE_NUM;i++) {
		DrawString(DRAW_X, DRAW_Y + i * DRAW_DISTANCE_Y, m_Log[i].c_str(), GetColor(255, 255, 255));
	}
}

//ログを追加
void CLog::AddLog(const std::string& log) {
	//ログを一つづつずらす
	for (int i = LOG_LINE_NUM - 1;i > 0;i--) {
		m_Log[i] = m_Log[i - 1];
	}
	//ログを書き込む
	m_Log[0] = log;

	//ついでにデバッグ用に出力
	std::cout << log << std::endl;
}