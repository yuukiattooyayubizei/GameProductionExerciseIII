#include"Object.h"

CObject::CObject() {
	m_Atk = 0;
	m_Direction = DIRECTION_NON;
	m_Kind = KIND_NON;
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_HP = 0;
	m_MaxHP = 0;
}

bool CObject::ForcedMove(DIRECTION dir, CMap map ,int movenum) {
	for (int i = 0;i < movenum;i++) {
		Int2 NextPos = AddInt2(m_Pos, DirectionToInt2(dir));

		//今の座標で移動できる方向を調べる
		CanMove canmove = map.GetCanMove(m_Pos);

		switch (dir)
		{
		case DIRECTION_NON:
			break;
		case DIRECTION_UP:
			//上方向に行けるなら行く
			if (canmove.Up == true)
				m_Pos = NextPos;
			//行けなかったらその時点で終わり
			else
				return false;
			
			break;
		case DIRECTION_LEFT:
			//上方向に行けるなら行く
			if (canmove.Left == true)
				m_Pos = NextPos;
			//行けなかったらその時点で終わり
			else
				return false;
			break;
		case DIRECTION_DOWN:
			//上方向に行けるなら行く
			if (canmove.Down == true)
				m_Pos = NextPos;
			//行けなかったらその時点で終わり
			else
				return false;
			break;
		case DIRECTION_RIGHT:
			//上方向に行けるなら行く
			if (canmove.Right == true)
				m_Pos = NextPos;
			//行けなかったらその時点で終わり
			else
				return false;
			break;
		default:
			break;
		}
	}

	return true;
}