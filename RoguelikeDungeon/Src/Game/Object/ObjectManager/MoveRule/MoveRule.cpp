#include"MoveRule.h"
#include "../../../Map/Map.h"
#include "../../Object.h"

CanMove CMoveRule::GetCanMove(Int2 pos)
{
    CMap* Map = CMap::GetInstance();
    Int2 v = pos;
    CanMove C;
    Int2 NextPos{};
    NextPos.x = static_cast<int>(v.x);
    NextPos.y = static_cast<int>(v.y);

    //一旦全部trueに
    C.Down = true, C.Up = true, C.Left = true, C.Right = true;
    //マス目の端だとマスの外側の方向には行けない
    if (NextPos.x <= 0)
        C.Left = false;
    if (NextPos.x >= MAP_X - 1)
        C.Right = false;
    if (NextPos.y <= 0)
        C.Up = false;
    if (NextPos.y >= MAP_Y - 1)
        C.Down = false;

    //上下左右のマスを見て通れるマスでなければ行けない
    //すでにfalseなら見る必要がない
    TILE t = {};
    if (C.Left == true)
    {
        NextPos.x--;
        t = Map->GetTile(NextPos);

        if (t == TILE_WALL)
            C.Left = false;
        NextPos.x++;
    }
    if (C.Right == true)
    {
        NextPos.x++;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Right = false;
        NextPos.x--;
    }
    if (C.Up == true)
    {
        NextPos.y--;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Up = false;
        NextPos.y++;
    }
    if (C.Down == true)
    {
        NextPos.y++;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Down = false;
        NextPos.y--;
    }

    return C;
}

CanMove CMoveRule::GetCanMoveEnemy(Int2 pos, CObjectStore& objectStore)
{
    CMap* Map = CMap::GetInstance();
    Int2 v = pos;
    CanMove C;
    Int2 NextPos{};
    NextPos.x = static_cast<int>(v.x);
    NextPos.y = static_cast<int>(v.y);

    //一旦全部trueに
    C.Down = true, C.Up = true, C.Left = true, C.Right = true;
    //マス目の端だとマスの外側の方向には行けない
    if (NextPos.x <= 0)
        C.Left = false;
    if (NextPos.x >= MAP_X - 1)
        C.Right = false;
    if (NextPos.y <= 0)
        C.Up = false;
    if (NextPos.y >= MAP_Y - 1)
        C.Down = false;

    //上下左右のマスを見て通れるマスでなければ行けない
    //すでにfalseなら見る必要がない
    TILE t = {};
    if (C.Left == true)
    {
        NextPos.x--;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Left = false;

        NextPos.x++;
    }
    if (C.Right == true)
    {
        NextPos.x++;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Right = false;
        NextPos.x--;
    }
    if (C.Up == true)
    {
        NextPos.y--;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Up = false;
        NextPos.y++;
    }
    if (C.Down == true)
    {
        NextPos.y++;
        t = Map->GetTile(NextPos);
        if (t == TILE_WALL)
            C.Down = false;
        NextPos.y--;
    }

    return C;
}

ObjectKind CMoveRule::GetAheadMoveObject(Int2 pos, DIRECTION dir, CObjectStore& objectStore)
{
    ObjectKind ret = KIND_NON;
    Int2 p = pos;
    int id = -1;

    switch (dir)
    {
    case DIRECTION_UP:
        p.y--;
        break;
    case DIRECTION_DOWN:
        p.y++;
        break;
    case DIRECTION_LEFT:
        p.x--;
        break;
    case DIRECTION_RIGHT:
        p.x++;
        break;
    default:
        return KIND_NON;
    }

    id = objectStore.CollisionObject(p);

    if (id != -1)
    {
        ret = objectStore.GetKind(id);
    }

    return ret;
}