#include"EnemyModelManager.h"
#include<DxLib.h>
#include"../../Common.h"
#include <string>

const char ENEMY_MODEL_PATH_FRONT[] = "Data/Model/Enemy";
const char ENEMY_MODEL_PATH_BACK[] = ".x";

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
	for (int i = 1;i < 5;i++) {
		std::string path =
			std::string(ENEMY_MODEL_PATH_FRONT) +
			std::to_string(i) +
			ENEMY_MODEL_PATH_BACK;

		int hndl = LoadModelWithScale(path.c_str(), Scale, Rotation);
		m_hndl.push_back(hndl);
	}
}

int CEnemyModelManager::GetModel(EnemyType modelId) const {
	if (modelId > ENEMY_NON && modelId < ENEMY_TYPE_NUM)
		return m_hndl[modelId];
	else
		return -1;
}