#pragma once
#include "../../../Common.h"
#include "../ObjectStore/ObjectStore.h"

class CMoveRule {
public:
	CanMove GetCanMove(Int2 pos);

	//“G‚ÌˆÚ“®æŒˆ’è
	CanMove GetCanMoveEnemy(Int2 pos, CObjectStore& objectStore);

	//ˆÚ“®æ‚ÉObject‚ª‚¢‚é‚©‚Ç‚¤‚©
	ObjectKind GetAheadMoveObject(Int2 pos, DIRECTION dir, CObjectStore& objectStore);
};