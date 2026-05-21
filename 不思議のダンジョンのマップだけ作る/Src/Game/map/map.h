#pragma once
#include <vector>
#include<DxLib.h>
#include "../common.h"
#include "../Item/Item.h"

static constexpr int MAP_X = 50;			//マップの最大横幅
static constexpr int MAP_Y = 30;			//マップの最大縦幅
static constexpr int MAP_SIZE_MIN = 5;		//部屋の最小の大きさ
static constexpr int MAP_SIZE_MAX = 9;		//部屋の最大の大きさ
static constexpr int ROOM_MARGIN = 3;		//部屋どうしの最低距離

struct RoomEdge
{
	int roomA;
	int roomB;
	int distance;
};

struct CorridorInfo
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

class UnionFind
{
private:
	std::vector<int> m_Parent;

public:
	UnionFind(int size)
	{
		m_Parent.resize(size);

		for (int i = 0; i < size; i++)
		{
			m_Parent[i] = i;
		}
	}

	int Find(int x)
	{
		if (m_Parent[x] == x)
			return x;

		m_Parent[x] = Find(m_Parent[x]);
		return m_Parent[x];
	}

	bool Same(int a, int b)
	{
		return Find(a) == Find(b);
	}

	void Unite(int a, int b)
	{
		int rootA = Find(a);
		int rootB = Find(b);

		if (rootA == rootB)
			return;

		m_Parent[rootB] = rootA;
	}
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

	bool CollsionRoom(Int2 i);
};

class CMap {
private:
	static CMap* m_Instance;
public:
	static CMap* GetInstance();

	static void DeleteInstance();
private:
	//マップチップ
	TILE m_Map[MAP_Y][MAP_X];
	std::vector<CRoom> m_Room;		//部屋

	std::vector<FieldItem> m_Item;	//落ちているアイテム

	Int2 m_StairsPos;				//階段の座標

public:
	//部屋の中のランダムな座標を取得
	Int2 GetRoomPos();
public:
	CMap() { Init(); }

	void Init();

	//座標に部屋があるかを調べる
	//返り値は部屋の番号
	//部屋がない場合は-1を返す
	int GetRoomNum(Int2 i);

	//部屋どうしがかぶっているかを判定
	bool CollisionRoomToRoom(const CRoom& room);
	//廊下とかぶっているかを判定
	bool CollisionStairs(Int2 i);

	//部屋のサイズ決定
	CRoom RoomSizeDecision();
	//部屋の保存
	void RoomSave(const CRoom& room);

	//部屋を作成
	bool CreateRoom(int CreateNum);
	//階段を作成
	void CreateStairs();

	//階段の座標を返す
	Int2 GetStairsPos() const { return m_StairsPos; }

	//----------------------------------------------
	//アイテム関連
	//----------------------------------------------

	//床落ちアイテムを作成
	//xとyを入力したらその座標に、しなかったら置けるランダムなマスから選択
	void CreateItem(int CreateNum, int x = -1, int y = -1);

	//探している座標にアイテムがあるかどうか
	ITEM_TYPE IsItemExist(Int2 i);

	//アイテムどうしが同じ座標にあるかチェック
	bool CollisionItemToItem(FieldItem& item);

	//アイテムが同じ座標にあるかチェック
	bool CollisionItem(Int2 i);

	//指定した座標のアイテムを消去
	void EraseItem(Int2 pos);

	//----------------------------------------------
	//
	//----------------------------------------------

	//廊下の生成
	bool CreateCorridor();

	//一番近い部屋を特定
	SpecifiedRoomInformation SpecifiedRoom(const CRoom& room);

	//廊下につながる部屋のマスを決定
	CorridorInfo ConnectHallwayToRoom(const CRoom& room, SpecifiedRoomInformation close);

	//全部消す
	void DeleteAll();

	//掘る
	void DigCorridor(Int2 a, Int2 b);

	//描画
	void Draw(int x,int y);

	TILE GetTile(int x, int y);

	CRoom GetStartRoom();
};