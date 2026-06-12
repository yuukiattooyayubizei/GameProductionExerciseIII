#pragma once
#include <vector>
#include "../MapCommon.h"
#include "../../common.h"
#include "../MapData/MapData.h"
#include"../Room/Room.h"


class CMapCreate {
public:
	// ŠK’i‚ğì¬
	void CreateStairs(CMapData& mapData);

	// Œ@‚é
	void DigCorridor(CMapData& mapData, Int2 a, Int2 b);

	// •”‰®‚ğì¬
	bool CreateRoom(CMapData& mapData, int CreateNum);

	// •”‰®‚ÌƒTƒCƒYŒˆ’è
	CRoom RoomSizeDecision();

	//ŠK‘w‚ğ¶¬
	void CreateFloor(CMapData& mapData);

	// ˜L‰º‚Ì¶¬
	bool CreateCorridor(CMapData& mapData);
};