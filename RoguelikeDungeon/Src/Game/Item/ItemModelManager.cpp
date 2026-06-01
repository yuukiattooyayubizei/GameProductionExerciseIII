#include"ItemModelManager.h"
#include <DxLib.h>

CItemModelManager::CItemModelManager() {
	m_hndl.clear();

	LoadModel();
}


	CItemModelManager::~CItemModelManager() {
		for (int hndl : m_hndl) {
			if (hndl != -1) {
				MV1DeleteModel(hndl);
			}
		}

		m_hndl.clear();
	}



void CItemModelManager::LoadModel() {
	int hndl = MV1LoadModel("Data/Model/Item1.x");
	MV1SetScale(hndl, VGet(0.5f, 0.5f, 0.5f));
	MV1SetRotationXYZ(hndl, VGet(0.0f, DX_PI_F, 0.0f));
	m_hndl.push_back(hndl);

	hndl = MV1LoadModel("Data/Model/Item2.x");
	MV1SetScale(hndl, VGet(0.5f, 0.5f, 0.5f));
	MV1SetRotationXYZ(hndl, VGet(0.0f, DX_PI_F, 0.0f));
	m_hndl.push_back(hndl);

	hndl = MV1LoadModel("Data/Model/Item3.x");
	MV1SetScale(hndl, VGet(0.5f, 0.5f, 0.5f));
	MV1SetRotationXYZ(hndl, VGet(0.0f, DX_PI_F, 0.0f));
	m_hndl.push_back(hndl);

	hndl = MV1LoadModel("Data/Model/Item4.x");
	MV1SetScale(hndl, VGet(0.5f, 0.5f, 0.5f));
	MV1SetRotationXYZ(hndl, VGet(0.0f, DX_PI_F, 0.0f));
	m_hndl.push_back(hndl);


}

int CItemModelManager::GetModel(int modelId) const {
	if (modelId < 0 || modelId >= static_cast<int>(m_hndl.size())) {
		return -1;
	}

	return m_hndl[modelId];
}