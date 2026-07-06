#pragma once
#include "../../Common.h"
#include "../../Map/Map.h"
#include "../Object.h"
#include <vector>
#include "ObjectStore.h"

class CObjectSearch {
public:
    //指定された座標と同じ部屋にいるオブジェクトを返す
    std::vector<CObject*> FindObjectsInSameRoom(Int2 pos, CMap& map, const CObjectStore& objectStore);
    //指定された座標から指定された方向を見るときに映るオブジェクトを返す
    //isClosestObjectがtrueなら、一番近い一つだけ返す
    std::vector<CObject*> FindObjectsInSeeDirection(Int2 pos, DIRECTION dir, CMap& map, bool isClosestObject, const CObjectStore& objectStore);

    //もらった座標と同じ部屋にいるObjectを返す
    std::vector<CObject*> FindLiveTogetherObject(Int2 i, const CObjectStore& objectStore);
};