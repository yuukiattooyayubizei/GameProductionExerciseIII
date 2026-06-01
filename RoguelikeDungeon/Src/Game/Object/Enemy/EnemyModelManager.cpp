#include"EnemyModelManager.h"
#include <DxLib.h>

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
	int hndl = MV1LoadModel("Data/Model/Enemy1.x");
	MV1SetScale(hndl, VGet(0.5f, 0.5f, 0.5f));
	MV1SetRotationXYZ(hndl, VGet(0.0f, DX_PI_F, 0.0f));
	m_hndl.push_back(hndl);

	hndl = MV1LoadModel("Data/Model/Enemy2.x");
	MV1SetScale(hndl, VGet(0.5f, 0.5f, 0.5f));
	MV1SetRotationXYZ(hndl, VGet(0.0f, DX_PI_F, 0.0f));
	m_hndl.push_back(hndl);

	hndl = MV1LoadModel("Data/Model/Enemy3.x");
	MV1SetScale(hndl, VGet(0.5f, 0.5f, 0.5f));
	MV1SetRotationXYZ(hndl, VGet(0.0f, DX_PI_F, 0.0f));
	m_hndl.push_back(hndl);

	hndl = MV1LoadModel("Data/Model/Enemy4.x");
	MV1SetScale(hndl, VGet(0.5f, 0.5f, 0.5f));
	MV1SetRotationXYZ(hndl, VGet(0.0f, DX_PI_F, 0.0f));
	m_hndl.push_back(hndl);
}

int CEnemyModelManager::GetModel(int modelId) const {
	if (modelId < 0 || modelId >= static_cast<int>(m_hndl.size())) {
		return -1;
	}

	return m_hndl[modelId];
}