#pragma once

enum DIRECTION {
	DIRECTION_NON,
	DIRECTION_UP,	//è„
	DIRECTION_LEFT,	//ç∂
	DIRECTION_DOWN,	//â∫
	DIRECTION_RIGHT	//âE
};

//ÇªÇÃï˚å¸Ç…ìÆÇØÇÈÇ©Ç«Ç§Ç©
typedef struct {
	bool Up;
	bool Down;
	bool Left;
	bool Right;
}CanMove;

enum TILE {
	TILE_WALL,			//ï«
	TILE_WALL_EDGE,		//ï«
	TILE_ROOM,			//ïîâÆ
	TILE_CORRIDOR,		//òLâ∫
	TILE_STAIRS			//äKíi
};