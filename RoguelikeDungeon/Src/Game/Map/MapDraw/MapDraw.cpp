#include"MapDraw.h"
#include <DxLib.h>

void CMapDraw::Load() {
	if (m_Corridorhndl == -1)
	{
		m_Corridorhndl = MV1LoadModel("Data/Model/Corridor.x");
		MV1SetScale(m_Corridorhndl, VGet(0.5f, 0.5f, 0.5f));
	}

	if (m_Roomhndl == -1)
	{
		m_Roomhndl = MV1LoadModel("Data/Model/Room.x");
		MV1SetScale(m_Roomhndl, VGet(0.5f, 0.5f, 0.5f));
	}

	if (m_Stairshndl == -1)
	{
		m_Stairshndl = MV1LoadModel("Data/Model/STAIRS.x");
		MV1SetScale(m_Stairshndl, VGet(0.5f, 0.5f, 0.5f));
	}

	if (m_Wallhndl == -1)
	{
		m_Wallhndl = MV1LoadModel("Data/Model/WALL.x");
		MV1SetScale(m_Wallhndl, VGet(0.5f, 0.5f, 0.5f));
	}
}

void CMapDraw::Exit() {
	if (m_Corridorhndl != -1) {
		MV1DeleteModel(m_Corridorhndl);
		m_Corridorhndl = -1;
	}
	if (m_Roomhndl != -1) {
		MV1DeleteModel(m_Roomhndl);
		m_Roomhndl = -1;
	}
	if (m_Wallhndl != -1) {
		MV1DeleteModel(m_Wallhndl);
		m_Wallhndl = -1;
	}
	if (m_Stairshndl != -1) {
		MV1DeleteModel(m_Stairshndl);
		m_Stairshndl = -1;
	}
}

void CMapDraw::Draw(CMapData& mapData,Int2 playerpos) {
	//プレイヤーの近くだけ描写
	for (int i = playerpos.y - DRAW_LENGTH_Y;i < playerpos.y + DRAW_LENGTH_Y;i++)
	{
		for (int k = playerpos.x - DRAW_LENGTH_X;k < playerpos.x + DRAW_LENGTH_X;k++)
		{

			int tile = TILE_WALL;
			Int2 nextPos = { k,i };

			// 配列内なら実際のマップを参照
			if (mapData.InvestigationMapOutside(nextPos) == false)
			{
				tile = mapData.GetTile(nextPos);
			}

			switch (tile)
			{
			case TILE_WALL:
				MV1SetPosition(m_Wallhndl, VGet(-k * TILE_SIZE, 150, i * TILE_SIZE));
				MV1DrawModel(m_Wallhndl);
				break;

			case TILE_ROOM:
				MV1SetPosition(m_Roomhndl, VGet(-k * TILE_SIZE, 150, i * TILE_SIZE));
				MV1DrawModel(m_Roomhndl);
				break;

			case TILE_CORRIDOR:
				MV1SetPosition(m_Corridorhndl, VGet(-k * TILE_SIZE, 150, i * TILE_SIZE));
				MV1DrawModel(m_Corridorhndl);
				break;

			default:
				break;
			}
		}
	}

	//階段の描画
	int centerX = 8 + 16 * mapData.GetStairsPos().x;
	int centerY = 8 + 16 * mapData.GetStairsPos().y;
	//DrawBox(centerX + 8, centerY + 8, centerX - 8, centerY - 8, GetColor(255, 0, 0), TRUE);

	float x = -mapData.GetStairsPos().x * TILE_SIZE;
	float z = mapData.GetStairsPos().y * TILE_SIZE;

	VECTOR pos1 = VGet(x - 50.0f, 150, z - 50.0f);
	VECTOR pos2 = VGet(x + 50.0f, 150 + 100.0f, z + 50.0f);
	//DrawCube3D(pos1, pos2, GetColor(0, 0, 128), GetColor(128, 128, 128), TRUE);
	MV1SetPosition(m_Stairshndl, VGet(x, 150, z));
	MV1DrawModel(m_Stairshndl);
}