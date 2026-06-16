#include"ItemModelManager.h"
#include<DxLib.h>
#include"../Common.h"

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
	if (!m_hndl.empty())
	{
		return;
	}

	VECTOR Scale = VGet(0.5f, 0.5f, 0.5f);
	VECTOR Rotation = VGet(0.0f, DX_PI_F, 0.0f);
	int hndl = LoadModelWithScale("Data/Model/Item1.x", Scale, Rotation);
	m_hndl.push_back(hndl);

	hndl = LoadModelWithScale("Data/Model/Item2.x", Scale, Rotation);
	m_hndl.push_back(hndl);

	hndl = LoadModelWithScale("Data/Model/Item3.x", Scale, Rotation);
	m_hndl.push_back(hndl);

	hndl = LoadModelWithScale("Data/Model/Item4.x", Scale, Rotation);
	m_hndl.push_back(hndl);


}

int CItemModelManager::GetModel(int modelId) const {
	if (modelId < 0 || modelId >= static_cast<int>(m_hndl.size())) {
		return -1;
	}

	return m_hndl[modelId];
}