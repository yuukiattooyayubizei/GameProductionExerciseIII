#pragma once
#include "../../Common.h"

class CMoveRule {
public:
	CanMove GetCanMove(Int2 pos);
};