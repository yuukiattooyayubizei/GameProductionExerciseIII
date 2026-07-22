#pragma once
#include "../Object.h"
#include "EnemyModelManager.h"
#include "../../Map/RoomLink/RoomLink.h"
#include <vector>

enum EnemyMoveType {
    MOVE_WALK,
    MOVE_TRACK,
};

class CEnemy : public CObject {
protected:
    EnemyType m_EnemyType;
    CEnemyModelManager* m_ModelManager = nullptr;
    EnemyMoveType m_MoveType;

    //int m_DestinationRoomID;        //次にどこの部屋に向かいたいか


private:
    //それぞれの部屋に何回行ったか
    std::vector<int> m_RoomVisitCount;

    // どの部屋にいるか
    // 廊下にいる場合は-1
    int m_CurrentRoomID = -1;

    // 最後にいた部屋はどこか
    int m_PreviousRoomID = -1;

    // 目的地
    int m_DestinationRoomID = -1;

    // 次はどの部屋に行けばいいか
    int m_NextRoomID = -1;

    // そこに行くのにどう進むべきか
    std::vector<int> m_RoomRoute;

    // m_RoomRouteの何番目まで進んだか
    int m_RoomRouteIndex = 0;

    //廊下に入る前の部屋のID
    int m_CorridorFromRoomID = -1;
    
public:
    CEnemy(CEnemyModelManager* modelManager);
    virtual ~CEnemy() override;

    virtual void Init() override;
    void InitRoomVisitCount(int roomCount);
    virtual void Load() override;
    virtual void Step(CanMove canmove, Int2 playerPos) override;
    virtual void Draw() override;
    virtual void Exit() override;
protected:
    virtual void InitStatus() = 0;
    virtual void InitModelType() = 0;
private:
    //訪問回数の更新
    void UpdateRoomVisit();
    //次どの部屋に行くか
    int SelectDestinationRoom(int currentRoomID);
    //巡回経路の作成
    bool StartPatrol();

    //X軸とY軸の両方に差がないとき
    DIRECTION GetDirectionToPosition(Int2 current, Int2 target);
    //X軸とY軸の両方に差があるとき
    std::vector<DIRECTION> GetDirectionsToPosition(Int2 current, Int2 target);

    //部屋の中だとGatoに向かう
    void MoveToGate(const RoomLink& link, CanMove canMove);

    bool CanMoveDirection(const CanMove& canMove, DIRECTION direction);

    Int2 GetStartGate(const RoomLink& link,int currentRoomID) const;

    int FindRoutePosition(const std::vector<Int2>& route, Int2 position);

    bool GetNextCorridorPosition(const RoomLink& link, int fromRoomID, Int2 currentPosition, Int2& nextPosition);

    void MoveAlongCorridor(const RoomLink& link, int fromRoomID, CanMove canMove);

    void UpdateRoomRouteProgress();

    void Patrol(CanMove canMove);

    int GetRouteCurrentRoomID() const;

    //いろいろリセット
    void ResetPatrolRoute();

    //プレイヤーと隣接しているか
    bool IsAdjacentToPlayer(Int2 playerPos) const;

    //プレイヤーに向きを合わせる
    void FacePlayer(Int2 playerPos);

    //プレイヤーを追う
    void ChasePlayer(CanMove canMove,Int2 playerPos);

    //巡回を再生成する
    bool RepairPatrolRoute();
};