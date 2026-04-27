#include <vector>
#include "../common.h"

static constexpr int MAP_X = 50;			//マップの最大横幅
static constexpr int MAP_Y = 30;			//マップの最大縦幅
static constexpr int RETRY_MAX = 100;		//部屋作成時のリトライ回数(これを超えると部屋の置き場がないとみなす)
static constexpr int MAP_SIZE_MIN = 5;		//部屋の最小の大きさ
static constexpr int MAP_SIZE_MAX = 9;		//部屋の最大の大きさ



struct Int2 {
	int x, y;
};

struct Float2 {
	float x, y;
};

struct CriateCorridor
{
	Int2 StratPos;
	Int2 MovePos;
};

struct SpecifiedRoomInformation {
	int m_CloseRoomID;			//どの部屋が一番近いか
	DIRECTION m_Direction;		//一番近い部屋の方角	
	int m_DistanceX;			//中心までのX座標の距離
	int m_DistanceY;			//中心までのY座標の距離
};

class CRoom {
private:
	Int2 m_Size;			//部屋の大きさ
	Int2 m_Pos;				//部屋の座標(左上座標)
	Float2 m_Center;		//部屋の中心座標
	int m_CloseRoom;		//最も近い部屋の番号
	bool m_IsConnectRoom;	//他の部屋とつながっているか

public:
	CRoom();

	void SetSize(int x, int y) {m_Size.x = x;
								m_Size.y = y;}
	void SetPos(int x, int y) {	m_Pos.x = x;
								m_Pos.y = y;}
	void SetCenter(float x, float y) {	m_Center.x = x;
									m_Center.y = y;}
	void SetConnect(bool is) { m_IsConnectRoom = is; }

	const Int2& GetSize() const { return m_Size; }
	const Int2& GetPos() const { return m_Pos; }
	const Float2& GetCenter() const { return m_Center; }
	bool GetConnectRoom() const { return m_IsConnectRoom; }
};

class CMap {
private:
	//マップチップ
	TILE m_Map[MAP_Y][MAP_X];
	std::vector<CRoom> m_Room;//部屋
public:
	CMap() { Init(); }

	void Init();

	//部屋どうしがかぶっているかを判定
	bool CollisionRoom(const CRoom& room);

	//部屋のサイズ決定
	CRoom RoomSizeDecision();
	//部屋の保存
	void RoomSave(const CRoom& room);

	//部屋を作成
	bool CreateRoom();

	//廊下の生成
	bool CreateCorridor();

	//一番近い部屋を特定
	SpecifiedRoomInformation SpecifiedRoom(const CRoom& room);

	//廊下につながる部屋のマスを決定
	CriateCorridor ConnectHallwayToRoom(const CRoom& room, SpecifiedRoomInformation close);

	//全部消す
	void DeleteAll();

	//掘る
	void DigCorridor(Int2 a, Int2 b);

	//階段を作成
	void CreateStairs();

	//描画
	void Draw(int x,int y);

	TILE GetTile(int x, int y) { return m_Map[y][x]; }

	CRoom GetStratRoom();
};