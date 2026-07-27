#include"Sound.h"
#include<DxLib.h>

static const char* SOUND_PATH[SOUND_NUM] = {
	"Data/Music/Title.mp3",
	"Data/Music/Result.mp3",
	"Data/Music/Result.mp3",
};

SOUND_MANAGER g_soundManager;

//データを初期化
void InitSound()
{
	for (int i = 0; i < SOUND_NUM; i++)
	{
		g_soundManager.m_hndl[i] = -1;
	}
}
//画像データをロード
void LoadSound()
{
	for (int i = 0; i < SOUND_NUM; i++)
	{
		if (g_soundManager.m_hndl[i] == -1)
		{
			g_soundManager.m_hndl[i] = LoadSoundMem(SOUND_PATH[i]);
		}
	}
}

//終了前に行う
void ExitSound()
{
	for (int i = 0; i < SOUND_NUM; i++)
	{
		if (g_soundManager.m_hndl[i] != -1)
		{
			DeleteSoundMem(g_soundManager.m_hndl[i]);
		}
	}
}

void RequestSound(int soundID, int type)
{
	PlaySoundMem(g_soundManager.m_hndl[soundID], type);
}

void StopSound(int soundID)
{
	StopSoundMem(g_soundManager.m_hndl[soundID]);
}

void StopAllSound()
{
	for (int i = 0; i < SOUND_NUM; i++)
	{
		StopSound(i);
	}
}