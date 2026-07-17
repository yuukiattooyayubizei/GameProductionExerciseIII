#pragma once

#include "../../Common.h"
#include <vector>

struct RoomLink
{
	int m_RoomA;//部屋AのID
	int m_RoomB;//部屋BのID

	Int2 m_CenterA;//部屋Aの中心点
	Int2 m_CenterB;//部屋Bの中心点

	Int2 m_GateA;//廊下に隣接する部屋Aの座標
	Int2 m_GateB;//廊下に隣接する部屋Bの座標

	Int2 m_Bend;//廊下の曲がる位置

	// 推奨
	// AからBまでの廊下経路を順番に保存
	std::vector<Int2> m_Route;
};