#include"ObjectSearch.h"

std::vector<CObject*> CObjectSearch::FindObjectsInSameRoom(Int2 pos, CMap& map, const CObjectStore& objectStore) {

    std::vector<CObject*> result;

    int roomNum = map.GetRoomID(pos);

    if (roomNum == -1)
    {
        return result;
    }

    for (CObject* object : objectStore.GetObjects())
    {
        if (object == nullptr)
        {
            continue;
        }

        if (object->GetKind() == KIND_PLAYER)
        {
            continue;
        }

        if (map.GetRoomID(object->GetPos()) == roomNum)
        {
            result.push_back(object);
        }
    }

    return result;
}

//指定された座標から指定された方向を見るときに映るオブジェクトを返す
//isClosestObjectがtrueなら、一番近い一つだけ返す
std::vector<CObject*> CObjectSearch::FindObjectsInSeeDirection(Int2 pos, DIRECTION dir, CMap& map, bool isClosestObject, const CObjectStore& objectStore) {
    Int2 nextpos = pos;
    Int2 movepos = DirectionToInt2(dir);
    std::vector<CObject*> result;

    for (int i = 0;i < map.GetFieldOfVision(pos, dir);i++) {
        //次のマスを見る
        nextpos = AddInt2(nextpos, movepos);
        //次のマスにオブジェクトがいるか見る
        CObject* target = objectStore.FindObjectAt(nextpos);

        //なにかいたら、そいつを返り値に加える
        if (target != nullptr)
        {
            result.push_back(target);

            //1体見つけて終わりなら、ここで終わる
            if (isClosestObject == true)return result;
        }
    }
    return result;
}

//プレイヤーと同じ部屋にいるObjectを返す
std::vector<CObject*> CObjectSearch::FindLiveTogetherObject(Int2 i, const CObjectStore& objectStore) {
    CMap* Map = CMap::GetInstance();
    std::vector<CObject*> res;

    //プレイヤーの部屋番号を取得
    int PlayerRoomNum = Map->GetRoomID(i);
    //-1(部屋にいない)の場合終了
    if (PlayerRoomNum == -1)return {};


    for (CObject* object : objectStore.GetObjects()) {
        if (object->GetKind() == KIND_PLAYER) {
            continue;
        }

        if (Map->GetRoomID(object->GetPos()) == PlayerRoomNum) {
            res.push_back(object);
        }
    }

    return res;
}