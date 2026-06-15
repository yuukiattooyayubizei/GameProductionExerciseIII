#include"EnemyModelManager.h"
#include <DxLib.h>
#include "../../common.h"

CEnemyModelManager::CEnemyModelManager() {
	m_hndl.clear();

	LoadModel();
}


	CEnemyModelManager::~CEnemyModelManager() {
		for (int hndl : m_hndl) {
			if (hndl != -1) {
				MV1DeleteModel(hndl);
			}
		}

		m_hndl.clear();
	}



void CEnemyModelManager::LoadModel() {
	VECTOR Scale = VGet(0.5f, 0.5f, 0.5f);
	VECTOR Rotation = VGet(0.0f, DX_PI_F, 0.0f);
	int hndl = LoadModelWithScale("Data/Model/Enemy1.x", Scale, Rotation);
	m_hndl.push_back(hndl);

	hndl = LoadModelWithScale("Data/Model/Enemy2.x", Scale, Rotation);
	m_hndl.push_back(hndl);

	hndl = LoadModelWithScale("Data/Model/Enemy3.x", Scale, Rotation);
	m_hndl.push_back(hndl);

	hndl = LoadModelWithScale("Data/Model/Enemy4.x", Scale, Rotation);
	m_hndl.push_back(hndl);
}

int CEnemyModelManager::GetModel(EnemyType modelId) const {
	if (modelId > ENEMY_NON && modelId < ENEMY_TYPE_NUM)
		return m_hndl[modelId];
	else
		return -1;
}