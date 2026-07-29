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

void CEnemy::Step(CanMove canMove,Int2 playerPosition){
	UpdateRoomVisit();
	UpdateRoomRouteProgress();

	if (IsAdjacentToPlayer(playerPosition))
	{
		FacePlayer(playerPosition);
		return;
	}

	CMap* map = CMap::GetInstance();

	int playerRoomID = map->GetRoomID(playerPosition);

	if (m_CurrentRoomID >= 0 &&
		playerRoomID == m_CurrentRoomID)
	{
		ChasePlayer(canMove, playerPosition);
		return;
	}

	Patrol(canMove);
}

void CEnemy::Exit() {
}

void CEnemy::InitRoomVisitCount(int roomCount)
{
	m_RoomVisitCount.assign(roomCount, 0);

	m_CurrentRoomID = -1;
	m_PreviousRoomID = -1;

	m_DestinationRoomID = -1;
	m_NextRoomID = -1;

	m_RoomRoute.clear();
	m_RoomRouteIndex = 0;
}

//訪問回数の更新
void CEnemy::UpdateRoomVisit()
{
	CMap* map = CMap::GetInstance();

	if (map == nullptr)
	{
		return;
	}

	//更新の前にいた部屋ID
	const int oldRoomID = m_CurrentRoomID;

	//今いる部屋のID
	//廊下にいるなら-1になる
	const int newRoomID = map->GetRoomID(m_Pos);

	//部屋が何個あるか計算
	const int roomCount = map->GetRoomNum();

	//部屋から廊下へ出たなら
	if (oldRoomID >= 0 && newRoomID == -1)
	{
		//廊下に入る前の部屋IDを保存
		m_CorridorFromRoomID = oldRoomID;
	}

	//部屋にいる場合
	if (newRoomID >= 0)
	{
		//前回いた部屋と違ったら、別の部屋に移った
		if (newRoomID != m_PreviousRoomID)
		{
			m_RoomVisitCount[newRoomID]++;
		}

		// 最後に入った部屋を保存
		m_PreviousRoomID = newRoomID;
		

		// 廊下から部屋に着いた
		m_CorridorFromRoomID = -1;
	}

	//現在どの部屋にいるか保存
	//廊下なら-1にする
	m_CurrentRoomID = newRoomID;
}

//どの部屋に行くか決める
//優先順位
//1.現在の部屋以外
//2.訪問回数が最も少ない
//3.BFS距離が最も短い
//4.それでも同じならランダム
int CEnemy::SelectDestinationRoom(int currentRoomID)
{
	CMap* Map = CMap::GetInstance();

	//部屋の数を取得
	int roomCount = Map->GetRoomNum();

	//その部屋との距離を取得
	std::vector<int> distance = Map->CalcRoomDistance(currentRoomID);

	int minimumVisitCount = INT_MAX;
	std::vector<int> visitCandidates;

	for (int roomID = 0;roomID < static_cast<int>(roomCount);roomID++)
	{
		// 別の部屋へ向かう
		if (roomID == currentRoomID)continue;

		//繋がっていない部屋ならやめる
		if (distance[roomID] < 0)continue;

		int visitCount = m_RoomVisitCount[roomID];

		//現在の最小値より低いなら更新
		if (visitCount < minimumVisitCount)
		{
			//最小値を変更
			minimumVisitCount = visitCount;

			//リセットする
			visitCandidates.clear();
			visitCandidates.push_back(roomID);
		}
		else if (visitCount == minimumVisitCount)
		{
			visitCandidates.push_back(roomID);
		}
	}

	if (visitCandidates.empty())return -1;
	

	int minimumDistance = INT_MAX;
	std::vector<int> distanceCandidates;

	for (int roomID : visitCandidates)
	{
		if (distance[roomID] < minimumDistance)
		{
			minimumDistance = distance[roomID];

			distanceCandidates.clear();
			distanceCandidates.push_back(roomID);
		}
		else if (distance[roomID] == minimumDistance)
		{
			distanceCandidates.push_back(roomID);
		}
	}

	if (distanceCandidates.empty())return -1;
	

	int randomIndex =GetRand(static_cast<int>(distanceCandidates.size()) - 1);

	return distanceCandidates[randomIndex];
}

bool CEnemy::StartPatrol()
{
	if (m_CurrentRoomID < 0)
	{
		return false;
	}

	CMap* Map = CMap::GetInstance();

	m_DestinationRoomID =SelectDestinationRoom(m_CurrentRoomID);

	if (m_DestinationRoomID < 0)return false;

	m_RoomRoute = Map->FindRoomRoute(m_CurrentRoomID,m_DestinationRoomID);

	if (m_RoomRoute.size() < 2)
	{
		m_RoomRoute.clear();
		return false;
	}

	// 0番目は現在の部屋
	// 1番目が次に向かう部屋
	m_RoomRouteIndex = 1;
	m_NextRoomID = m_RoomRoute[m_RoomRouteIndex];

	return true;
}

DIRECTION CEnemy::GetDirectionToPosition(Int2 current,Int2 target){

	if (current.x < target.x)return DIRECTION_RIGHT;
	if (current.x > target.x)return DIRECTION_LEFT;
	if (current.y < target.y)return DIRECTION_DOWN;
	if (current.y > target.y)return DIRECTION_UP;

	return DIRECTION_NON;
}

std::vector<DIRECTION> CEnemy::GetDirectionsToPosition(Int2 current,Int2 target){

	std::vector<DIRECTION> directions;

	int differenceX = target.x - current.x;
	int differenceY = target.y - current.y;

	// 距離が大きい方向を先にする例
	if (abs(differenceX) >= abs(differenceY))
	{
		if (differenceX > 0)directions.push_back(DIRECTION_RIGHT);
		else if (differenceX < 0)directions.push_back(DIRECTION_LEFT);
		if (differenceY > 0)directions.push_back(DIRECTION_DOWN);
		else if (differenceY < 0)directions.push_back(DIRECTION_UP);
		
	}
	else
	{
		if (differenceY > 0)directions.push_back(DIRECTION_DOWN);
		else if (differenceY < 0)directions.push_back(DIRECTION_UP);
		if (differenceX > 0)directions.push_back(DIRECTION_RIGHT);
		else if (differenceX < 0)directions.push_back(DIRECTION_LEFT);
	}

	return directions;
}

//部屋の中だとGatoに向かう
void CEnemy::MoveToGate(const RoomLink& link, CanMove canMove){
	Int2 targetGate =GetStartGate(link, m_CurrentRoomID);

	std::vector<DIRECTION> directions = GetDirectionsToPosition(m_Pos, targetGate);

	for (DIRECTION direction : directions)
	{
		if (CanMoveDirection(canMove, direction))
		{
			m_Direction = direction;
			return;
		}
	}

	// 全方向が塞がれているなら何もしない
	m_Direction = DIRECTION_NON;
}

bool CEnemy::CanMoveDirection(const CanMove& canMove,DIRECTION direction){
	switch (direction)
	{
	case DIRECTION_UP:return canMove.Up;
	case DIRECTION_DOWN:return canMove.Down;
	case DIRECTION_LEFT:return canMove.Left;
	case DIRECTION_RIGHT:return canMove.Right;

	default:
		return false;
	}
}

Int2 CEnemy::GetStartGate(const RoomLink& link, int currentRoomID) const{
	Int2 ret = { -1, -1 };

	if (currentRoomID == link.m_RoomA) ret = link.m_GateA;
	if (currentRoomID == link.m_RoomB) ret = link.m_GateB;
	
	return ret;
}

//ルート上で自分がどこにいるか
int CEnemy::FindRoutePosition(const std::vector<Int2>& route,Int2 position){

	for (int i = 0;i < static_cast<int>(route.size());i++){
		if (route[i].x == position.x &&route[i].y == position.y)return i;
	}

	return -1;
}

//どっちの方向に進むべきか
bool CEnemy::GetNextCorridorPosition(const RoomLink& link,int fromRoomID,Int2 currentPosition,Int2& nextPosition){
	int currentIndex =FindRoutePosition(link.m_Route, currentPosition);

	//どこにも行けないなら新しくルートを作り直す
	if (currentIndex < 0)
	{
		RepairPatrolRoute();
		return false;
	}

	// roomAからroomBへ進む
	if (link.m_RoomA == fromRoomID)
	{
		int nextIndex = currentIndex + 1;

		if (nextIndex >=static_cast<int>(link.m_Route.size()))return false;
		
		nextPosition = link.m_Route[nextIndex];
		return true;
	}

	// roomBからroomAへ進む
	int nextIndex = currentIndex - 1;

	if (nextIndex < 0)return false;
	

	nextPosition = link.m_Route[nextIndex];
	return true;
}

//次のマスに向かう
void CEnemy::MoveAlongCorridor(const RoomLink& link,int fromRoomID,CanMove canMove){
	Int2 nextPosition;

	if (!GetNextCorridorPosition(link,fromRoomID,m_Pos,nextPosition))
	{
		m_Direction = DIRECTION_NON;
		return;
	}

	DIRECTION direction = GetDirectionToPosition(m_Pos, nextPosition);

	if (CanMoveDirection(canMove, direction))m_Direction = direction;
	// 他の敵などに塞がれている場合は待機
	else m_Direction = DIRECTION_NON;
}

//次の部屋に入ったら経路を更新
void CEnemy::UpdateRoomRouteProgress()
{
	if (m_CurrentRoomID < 0)return;

	if (m_CurrentRoomID != m_NextRoomID)return;
	
	//次の目的地に更新
	m_RoomRouteIndex++;

	//目的地が最後なら
	if (m_RoomRouteIndex >=static_cast<int>(m_RoomRoute.size())){
		m_DestinationRoomID = -1;
		m_NextRoomID = -1;

		m_RoomRoute.clear();
		m_RoomRouteIndex = 0;

		return;
	}

	//次の部屋へ向かう
	m_NextRoomID = m_RoomRoute[m_RoomRouteIndex];
}

void CEnemy::Patrol(CanMove canMove)
{
	CMap* map = CMap::GetInstance();

	if (map == nullptr)
	{
		m_Direction = DIRECTION_NON;
		return;
	}

	// 巡回経路がなければ、新しい目的地を選ぶ
	if (m_DestinationRoomID < 0)
	{
		if (!StartPatrol())
		{
			m_Direction = DIRECTION_NON;
			return;
		}
	}

	int fromRoomID;

	if (m_CurrentRoomID >= 0)
		fromRoomID = m_CurrentRoomID;
	
	else
		fromRoomID = m_CorridorFromRoomID;
	

	if (fromRoomID < 0 || m_NextRoomID < 0)
	{
		ResetPatrolRoute();
		m_Direction = DIRECTION_NON;
		return;
	}

	const RoomLink* link = map->FindRoomLink(fromRoomID,m_NextRoomID);

	if (link == nullptr)
	{
		ResetPatrolRoute();
		m_Direction = DIRECTION_NON;
		return;
	}
	
	 //部屋の中にいるなら
	if (m_CurrentRoomID >= 0)
	{
		const Int2 startGate = GetStartGate(*link,m_CurrentRoomID);

		//座標が範囲外ならリセット
		if (startGate.x < 0 ||startGate.y < 0){
			ResetPatrolRoute();
			m_Direction = DIRECTION_NON;
			return;
		}

		//ゲートについているなら
		if (m_Pos.x == startGate.x &&m_Pos.y == startGate.y)
		{
			MoveAlongCorridor(*link,m_CurrentRoomID,canMove);
			return;
		}
		//ここに来たらゲートについていない
		MoveToGate(*link, canMove);
		return;
	}

	//廊下にいるなら 
	MoveAlongCorridor(*link,m_CorridorFromRoomID,canMove);
}

int CEnemy::GetRouteCurrentRoomID() const
{
	if (m_CurrentRoomID >= 0) return m_CurrentRoomID;
	

	return m_CorridorFromRoomID;
}

void CEnemy::ResetPatrolRoute(){
	m_DestinationRoomID = -1;
	m_NextRoomID = -1;

	m_RoomRoute.clear();
	m_RoomRouteIndex = 0;

	m_CorridorFromRoomID = -1;
}

bool CEnemy::IsAdjacentToPlayer(Int2 playerPos) const
{
	//プレイヤーとの距離を計算
	Int2 difference =SubInt2(playerPos, m_Pos);

	//それぞれの方向を見る
	if (difference.x == 1 &&difference.y == 0)return true;
	if (difference.x == -1 &&difference.y == 0)return true;
	if (difference.x == 0 &&difference.y == 1)return true;
	if (difference.x == 0 &&difference.y == -1)return true;
	
	//ここまで来たら隣にいない
	return false;
}

void CEnemy::FacePlayer(Int2 playerPos)
{
	//プレイヤーとの距離を計算
	Int2 difference = SubInt2(playerPos, m_Pos);

	//プレイヤーが隣にいたらその方向を向く
	if (difference.x == 1 && difference.y == 0)
	{
		m_Direction = DIRECTION_RIGHT;
		return;
	}
	if (difference.x == -1 && difference.y == 0)
	{
		m_Direction = DIRECTION_LEFT;
		return;
	}
	if (difference.x == 0 && difference.y == 1)
	{
		m_Direction = DIRECTION_DOWN;
		return;
	}
	if (difference.x == 0 && difference.y == -1)
	{
		m_Direction = DIRECTION_UP;
		return;
	}

	//隣にいないならどこも向かない
	m_Direction = DIRECTION_NON;
}

//プレイヤーを追いかける
void CEnemy::ChasePlayer(CanMove canMove,Int2 playerPos)
{
	//自分と敵の距離を見る
	Int2 difference = SubInt2(playerPos, m_Pos);

	std::vector<DIRECTION> chaseDirections;

	//プレイヤーの方向に移動する
	if (difference.x > 0)
		chaseDirections.push_back(DIRECTION_RIGHT);
	else if (difference.x < 0)
		chaseDirections.push_back(DIRECTION_LEFT);

	if (difference.y > 0)
		chaseDirections.push_back(DIRECTION_DOWN);
	else if (difference.y < 0)
		chaseDirections.push_back(DIRECTION_UP);
	
	//その方向に動けるか
	std::vector<DIRECTION> movableDirections;

	for (DIRECTION direction : chaseDirections)
	{
		//その方向に動けるか見る
		if (CanMoveDirection(canMove,direction))
			movableDirections.push_back(direction);
	}

	//4方向全てに行けないなら移動しない
	if (movableDirections.empty())
	{
		m_Direction = DIRECTION_NON;
		return;
	}

	//移動できる中からランダムに移動
	const int randomIndex = GetRand(static_cast<int>(movableDirections.size()) - 1);

	m_Direction = movableDirections[randomIndex];
}

bool CEnemy::RepairPatrolRoute()
{
	CMap* map = CMap::GetInstance();

	//何番目の部屋にいるか調べる
	const int roomID =map->GetRoomID(m_Pos);

	//どこかの部屋にいるなら
	if (roomID >= 0)
	{
		m_CurrentRoomID = roomID;
		m_CorridorFromRoomID = -1;

		// 元の目的地が無効なら、新しく選び直す
		if (m_DestinationRoomID < 0 ||m_DestinationRoomID == roomID)
		{
			m_DestinationRoomID =
				SelectDestinationRoom(roomID);
		}

		//巡回する部屋を調べる
		m_RoomRoute =map->FindRoomRoute(roomID,m_DestinationRoomID);

		//部屋数が少なすぎるならリセット
		if (m_RoomRoute.size() < 2)
		{
			ResetPatrolRoute();
			return false;
		}

		m_RoomRouteIndex = 1;
		//次に進むべき部屋をセット
		m_NextRoomID =m_RoomRoute[m_RoomRouteIndex];

		return true;
	}

	//ここまで来たなら、今廊下にいる
	const RoomLink* actualLink = map->FindRoomLinkByPosition(m_Pos);

	//リンクが存在しなかったらやめる
	if (actualLink == nullptr)
	{
		return false;
	}

	// 目的地が無効なら、一方の部屋を仮の目的地にする
	if (m_DestinationRoomID < 0)
	{
		m_DestinationRoomID =actualLink->m_RoomB;
	}

	//廊下が繋がっていたら、その廊下の両端の部屋に続くルートをそれぞれ作る
	const std::vector<int> routeFromA =map->FindRoomRoute(actualLink->m_RoomA,m_DestinationRoomID);
	const std::vector<int> routeFromB =map->FindRoomRoute(actualLink->m_RoomB,m_DestinationRoomID);

	//ルートが存在するか確かめる
	const bool canReachFromA = !routeFromA.empty();
	const bool canReachFromB = !routeFromB.empty();

	//次に向かうべき部屋のID
	int enterRoomID = -1;

	//両方の部屋に行ける場合
	if (canReachFromA && canReachFromB)
	{
		//AとBのうち近い方を選ぶ
		if (routeFromA.size() <=routeFromB.size())
		{
			enterRoomID =actualLink->m_RoomA;
		}
		else
		{
			enterRoomID =actualLink->m_RoomB;
		}
	}
	//Aの部屋にしか行けない場合
	else if (canReachFromA)
	{
		enterRoomID =actualLink->m_RoomA;
	}
	//Bの部屋にしか行けない場合
	else if (canReachFromB)
	{
		enterRoomID =actualLink->m_RoomB;
	}
	//どちらの部屋にも行けない場合
	else
	{
		// 元の目的地へ行けない場合は廊下の片方を新しい目的地にする
		enterRoomID =actualLink->m_RoomA;

		m_DestinationRoomID =enterRoomID;
	}

	if (enterRoomID == actualLink->m_RoomA)
	{
		m_CorridorFromRoomID =actualLink->m_RoomB;

		m_NextRoomID =actualLink->m_RoomA;
	}
	else
	{
		m_CorridorFromRoomID =actualLink->m_RoomA;

		m_NextRoomID =actualLink->m_RoomB;
	}

	m_CurrentRoomID = -1;

	//その部屋に行くまでの道を探す
	m_RoomRoute =map->FindRoomRoute(enterRoomID,m_DestinationRoomID);

	m_RoomRouteIndex = 0;

	return true;
}