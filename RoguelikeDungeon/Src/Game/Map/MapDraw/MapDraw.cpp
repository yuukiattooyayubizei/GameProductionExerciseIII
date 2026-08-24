#include"MapDraw.h"

void CMapDraw::SetModelBrightness(int modelHandle, float brightness)
{
	if (modelHandle == -1)
	{
		return;
	}

	//à√Ç≥ÇÃó Çê›íË
	const COLOR_F color = GetColorF(brightness, brightness, brightness, 1.0f);

	//ê›íËÇµÇΩà√Ç≥Ç…Ç∑ÇÈ
	MV1SetDifColorScale(modelHandle, color);
	MV1SetAmbColorScale(modelHandle, color);
	MV1SetEmiColorScale(modelHandle, color);
}

void CMapDraw::Load() {
	VECTOR scale = VGet(0.5f, 0.5f, 0.5f);
	m_Corridorhndl = LoadModelWithScale("Data/Model/Corridor.x", scale);
	m_Roomhndl = LoadModelWithScale("Data/Model/Room.x", scale);
	m_Stairshndl = LoadModelWithScale("Data/Model/STAIRS.x", scale);
	m_Wallhndl = LoadModelWithScale("Data/Model/WALL.x", scale);

	SetModelBrightness(m_Corridorhndl, 0.40f);
	SetModelBrightness(m_Roomhndl, 0.40f);
	SetModelBrightness(m_Stairshndl, 0.40f);
	SetModelBrightness(m_Wallhndl, 0.40f);
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

void CMapDraw::DrawMapModel(int modelHandle,const Int2& position,float brightness){
    if (modelHandle == -1)
    {
        return;
    }

    SetModelBrightness(modelHandle, brightness);

    MV1SetPosition(
        modelHandle,
        VGet(
            -position.x * TILE_SIZE,
            150.0f,
            position.y * TILE_SIZE
        )
    );

    MV1DrawModel(modelHandle);
}

void CMapDraw::Draw(CMapData& mapData, Int2 playerPos)
{
    for (int y = playerPos.y - DRAW_LENGTH_Y;
        y < playerPos.y + DRAW_LENGTH_Y;
        ++y)
    {
        for (int x = playerPos.x - DRAW_LENGTH_X;
            x < playerPos.x + DRAW_LENGTH_X;
            ++x)
        {
            const Int2 tilePos = { x, y };

            TILE tile = TILE_WALL;

            // É}ÉbÉvì‡Ç»ÇÁé¿ç€ÇÃÉ^ÉCÉãÇéÊìæ
            if (!mapData.InvestigationMapOutside(tilePos))
            {
                tile = mapData.GetTile(tilePos);
            }

            int modelHandle = -1;

            switch (tile)
            {
            case TILE_WALL:
                modelHandle = m_Wallhndl;
                break;

            case TILE_ROOM:
            case TILE_CORRIDOR_ADJACENT_ROOM:
                modelHandle = m_Roomhndl;
                break;

            case TILE_CORRIDOR:
                modelHandle = m_Corridorhndl;
                break;

            default:
                continue;
            }

            const bool isVisible =mapData.IsVisibleFrom(playerPos, tilePos);

            //éãäEì‡Ç»ÇÁñæÇÈÇ≥1.0éãäEäOÇ»ÇÁñæÇÈÇ≥0.4
            const float brightness =isVisible? 1.0f: 0.4f;

            DrawMapModel(modelHandle,tilePos,brightness);
        }
    }

    // äKíi
    const Int2 stairsPos = mapData.GetStairsPos();

    const bool isStairsVisible =mapData.IsVisibleFrom(playerPos, stairsPos);

    //éãäEäOÇæÇ¡ÇΩÇÁà√Ç≠Ç∑ÇÈ
    const float stairsBrightness =isStairsVisible? 1.0f: 0.4f;

    SetModelBrightness(m_Stairshndl,stairsBrightness);

    MV1SetPosition(m_Stairshndl,VGet(-stairsPos.x * TILE_SIZE,150.0f,stairsPos.y * TILE_SIZE)
    );

    MV1DrawModel(m_Stairshndl);

    // éüÇÃï`âÊÇ…à√Ç≥ÇécÇ≥Ç»Ç¢ÇΩÇﬂÅAç≈å„Ç…ñﬂÇ∑
    SetModelBrightness(m_Wallhndl, 1.0f);
    SetModelBrightness(m_Roomhndl, 1.0f);
    SetModelBrightness(m_Corridorhndl, 1.0f);
    SetModelBrightness(m_Stairshndl, 1.0f);
}

//void CMapDraw::Draw(CMapData& mapData,Int2 playerpos) {
//	//ÉvÉåÉCÉÑÅ[ÇÃãﬂÇ≠ÇæÇØï`é 
//	for (int i = playerpos.y - DRAW_LENGTH_Y;i < playerpos.y + DRAW_LENGTH_Y;i++)
//	{
//		for (int k = playerpos.x - DRAW_LENGTH_X;k < playerpos.x + DRAW_LENGTH_X;k++)
//		{
//
//			int tile = TILE_WALL;
//			Int2 nextPos = { k,i };
//
//			// îzóÒì‡Ç»ÇÁé¿ç€ÇÃÉ}ÉbÉvÇéQè∆
//			if (mapData.InvestigationMapOutside(nextPos) == false)
//			{
//				tile = mapData.GetTile(nextPos);
//			}
//
//			switch (tile)
//			{
//			case TILE_WALL:
//				MV1SetPosition(m_Wallhndl, VGet(-k * TILE_SIZE, 150, i * TILE_SIZE));
//				MV1DrawModel(m_Wallhndl);
//				break;
//
//			case TILE_ROOM:
//				//òLâ∫Ç…ó◊ê⁄ÇµÇΩïîâÆÇ‡ï`âÊÇÕïÅí ÇÃïîâÆÇ∆ìØÇ∂
//			case TILE_CORRIDOR_ADJACENT_ROOM:
//				MV1SetPosition(m_Roomhndl, VGet(-k * TILE_SIZE, 150, i * TILE_SIZE));
//				MV1DrawModel(m_Roomhndl);
//				break;
//
//			case TILE_CORRIDOR:
//				MV1SetPosition(m_Corridorhndl, VGet(-k * TILE_SIZE, 150, i * TILE_SIZE));
//				MV1DrawModel(m_Corridorhndl);
//				break;
//
//			default:
//				break;
//			}
//		}
//	}
//
//	//äKíiÇÃï`âÊ
//	int centerX = 8 + 16 * mapData.GetStairsPos().x;
//	int centerY = 8 + 16 * mapData.GetStairsPos().y;
//
//	float x = -mapData.GetStairsPos().x * TILE_SIZE;
//	float z = mapData.GetStairsPos().y * TILE_SIZE;
//
//	VECTOR pos1 = VGet(x - 50.0f, 150, z - 50.0f);
//	VECTOR pos2 = VGet(x + 50.0f, 150 + 100.0f, z + 50.0f);
//	MV1SetPosition(m_Stairshndl, VGet(x, 150, z));
//	MV1DrawModel(m_Stairshndl);
//}