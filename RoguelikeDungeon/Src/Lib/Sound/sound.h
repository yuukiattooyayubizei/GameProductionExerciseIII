#pragma once



enum tagSound{
	BGMID_TITLE, //タイトルBGM
	BGMID_GAME, //本編BGM
	BGMID_RESULT, //リザルトBGM
	SEID_ATTACK,//攻撃時

	SOUND_NUM,//BGM,SEの総数
};

//音全体の管理用構造体
typedef struct {
	int m_hndl[SOUND_NUM];
}SOUND_MANAGER;

//データを初期化
void InitSound();
//データをロード
void LoadSound();
//終了前に行う
void ExitSound();

//音楽再生
void RequestSound(int soundID, int type);

void StopSound(int soundID);

void StopAllSound();

