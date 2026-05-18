#pragma once
#include<DxLib.h>
#include"../Scene.h"
#include "../../Object/Player/Player.h"
#include <vector>
#include "../../map/map.h"

//部屋の最小値
static const int ROOM_MIN = 5;
//部屋の最大値
static const int ROOM_MAX = 7;

class CPlayScene 
{
private:
	//プレイの状態遷移
	enum tagPlayScene
	{
		PLAY_SCENE_INIT,
		PLAY_SCENE_LOAD,
		PLAY_SCENE_LOOP,
		PLAY_SCENE_END,

		PLAY_SCENE_NUM,
	};
	tagPlayScene m_tagPlayScene;//状態遷移

	std::vector<CObject*> m_Object;
	CPlayer* m_Player;
	CMap m_Map;

	bool m_PlayerTurn;

public:
	//コンストラクタ・デストラクタ
	CPlayScene();
	~CPlayScene();

	//既に生成されているオブジェクトと座標が被っているかどうか
	bool CollsionObject(Int2& pos) const;

	//オブジェクト、アイテム、敵など全てと被っているかどうか
	bool CollsionAll(Int2 pos);

	Int2 FindSpawnPos();

	CanMove GetCanMove(Int2 pos);

	//移動先にObjectがいるかどうか
	ObjectKind GetAheadMoveObject(Int2 pos, DIRECTION dir);

	//敵生成
	void CreateEnemy(int CreateNum = 1);
	//プレイヤー生成
	void CreatePlayer();

	//メイン処理
	int Loop();
	//描画処理
	void Draw();

private:
	//初期化
	void Init(void);

	//破棄
	void Exit();

	//ロード
	void Load();

	//制御
	int Step();
};