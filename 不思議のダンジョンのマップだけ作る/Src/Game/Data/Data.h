#pragma once
#include <DxLib.h>
#include "../Common.h"

class CData {
private:
	static CData* m_Instance;
public:
	
public:
	CData();
	~CData();
	void Init();
	void Load();
public:
	static CData* GetInstance();

	static void DeleteInstance();

private:

};