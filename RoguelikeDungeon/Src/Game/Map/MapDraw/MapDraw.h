#pragma once
#include "../MapData/MapData.h"
#include <DxLib.h>

class CMapDraw {
private:
	int m_Corridorhndl = -1;
	int m_Roomhndl = -1;
	int m_Wallhndl = -1;
	int m_Stairshndl = -1;
public:
	void Load();
	void Draw(CMapData& mapData,Int2 playerpos);
	void Exit();
};