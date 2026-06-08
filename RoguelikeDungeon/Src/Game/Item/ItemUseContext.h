#pragma once
#include <vector>

class CPlayer;
class CObjectManager;
class CMap;

struct ItemUseContext
{
    CPlayer& player;
    CMap& map;
    CObjectManager& objectManager;
};