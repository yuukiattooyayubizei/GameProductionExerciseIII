#include"Enemy.h"
#include"../../Map/Map.h"

bool CanMoveDir(CanMove canmove, DIRECTION dir)
{
	switch (dir)
	{
	case DIRECTION_UP:    return canmove.Up;
	case DIRECTION_DOWN:  return canmove.Down;
	case DIRECTION_LEFT:  return canmove.Left;
	case DIRECTION_RIGHT: return canmove.Right;
	default:              return false;
	}
}

CEnemy::CEnemy(CEnemyModelManager* modelManager)
    : m_ModelManager(modelManager)
{
	m_EnemyType = ENEMY_NON;
	m_MoveType = MOVE_WALK;
}



CEnemy::~CEnemy() {
}

void CEnemy::Init() {
    m_Pos.x = 0;
    m_Pos.y = 0;
    m_Kind = KIND_ENEMY;

    InitModelType();
    InitStatus();
}

void CEnemy::Load() {
}

void CEnemy::Draw() {
    if (m_ModelManager == nullptr) {
        return;
    }

    int hndl = m_ModelManager->GetModel(m_EnemyType);

    if (hndl == -1) {
        return;
    }

    MV1SetPosition(hndl, VGet(-m_Pos.x * TILE_SIZE, 151, m_Pos.y * TILE_SIZE));

    MV1DrawModel(hndl);
}

void CEnemy::Step(CanMove canmove, Int2 playerPos) {

	CMap* Map = CMap::GetInstance();

	//プレイヤーと敵のいる部屋を探す
	int PlayerRoomNum = Map->GetRoomNum(playerPos);
	int EnemyRoomNum = Map->GetRoomNum(m_Pos);
	Int2 SubPos = SubInt2(playerPos, m_Pos);

	//プレイヤーが隣にいるならそっちに移動(攻撃)
	if(SubPos.x == 1 && SubPos.y == 0)			m_Direction = DIRECTION_RIGHT;
	else if (SubPos.x == -1 && SubPos.y == 0)	m_Direction = DIRECTION_LEFT;
	else if (SubPos.x== 0 && SubPos.y == 1)		m_Direction = DIRECTION_DOWN;
	else if (SubPos.x == 0 && SubPos.y == -1)	m_Direction = DIRECTION_UP;
	//同じ部屋にいるなら、プレイヤーの方向に向かう
	//PlayerRoomNumの-1はプレイヤーが部屋にいないという意味になるので除外
	else if (PlayerRoomNum != -1 && PlayerRoomNum == EnemyRoomNum)
	{
		std::vector<DIRECTION> chaseDirs;

		// プレイヤーとの距離を縮める方向だけ候補に入れる
		if (SubPos.x > 0) chaseDirs.push_back(DIRECTION_RIGHT);
		if (SubPos.x < 0) chaseDirs.push_back(DIRECTION_LEFT);
		if (SubPos.y > 0) chaseDirs.push_back(DIRECTION_DOWN);
		if (SubPos.y < 0) chaseDirs.push_back(DIRECTION_UP);

		// 移動可能な追跡方向だけ残す
		std::vector<DIRECTION> movableChaseDirs;

		for (DIRECTION dir : chaseDirs)
		{
			if (CanMoveDir(canmove, dir))
			{
				movableChaseDirs.push_back(dir);
			}
		}

		if (!movableChaseDirs.empty())
		{
			// 追跡可能な方向から選ぶ
			m_Direction = movableChaseDirs[GetRand(static_cast<int>(movableChaseDirs.size()) - 1)];
		}
		else
		{
			// 追跡方向に進めない場合
			m_Direction = DIRECTION_NON;
		}
	}
	else
	{
		//部屋が違うならとりあえずランダムで
		std::vector<DIRECTION> dirs;

		//行ける方向があったらそれだけ入れる
		if (canmove.Up) dirs.push_back(DIRECTION_UP);
		if (canmove.Down) dirs.push_back(DIRECTION_DOWN);
		if (canmove.Left) dirs.push_back(DIRECTION_LEFT);
		if (canmove.Right) dirs.push_back(DIRECTION_RIGHT);

		if (dirs.empty())
		{
			m_Direction = DIRECTION_NON;
		}
		else
		{
			//入れられた中から一つだけ選択
			m_Direction = dirs[GetRand(static_cast<int>(dirs.size()) - 1)];
		}
	}

}

void CEnemy::Exit() {
}