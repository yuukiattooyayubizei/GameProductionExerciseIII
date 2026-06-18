#pragma once
#include"../../Common.h"
#include <vector>

// アイテム1個分の情報
struct ConnectRoomData
{
	int ConnectRoom;				//この部屋と繋がっている部屋のID
	Int2 ConnectCorridorPos;		//廊下に隣接している部屋の座標
};

class CRoom {
private:
	Int2 m_Size;											//部屋の大きさ
	Int2 m_Pos;												//部屋の座標(左上座標)
	Float2 m_Center;										//部屋の中心座標
	int m_CloseRoom;										//最も近い部屋の番号
	bool m_IsConnectRoom;									//他の部屋とつながっているか
	std::vector<ConnectRoomData> m_ConnectAheadRoom;		//この部屋と繋がっている部屋に関する情報

public:
	CRoom();

	void SetSize(int x, int y) {
		m_Size.x = x;
		m_Size.y = y;
	}
	void SetPos(int x, int y) {
		m_Pos.x = x;
		m_Pos.y = y;
	}
	void SetCenter(float x, float y) {
		m_Center.x = x;
		m_Center.y = y;
	}
	void SetConnect(bool is) { m_IsConnectRoom = is; }

	const Int2& GetSize() const { return m_Size; }
	const Int2& GetPos() const { return m_Pos; }
	const Float2& GetCenter() const { return m_Center; }
	bool GetConnectRoom() const { return m_IsConnectRoom; }

	//繋がっている部屋のIDを追加
	void AddConnectAheadRoom(int id, Int2 Pos); 

	std::vector<ConnectRoomData> GetConnectAheadRoom() { return m_ConnectAheadRoom; }
	//m_ConnectAheadRoomのIDだけ取得
	std::vector<int> GetConnectAheadRoomID();

	Int2 GetConnectAheadRoomPos(int id);

	bool CollisionRoom(Int2 i);
};