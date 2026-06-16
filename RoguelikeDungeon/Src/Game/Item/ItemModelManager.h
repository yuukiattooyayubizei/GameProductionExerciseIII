#pragma once
#include<vector>

class CItemModelManager
{
public:
    CItemModelManager();
    ~CItemModelManager();

    void LoadModel();
    int GetModel(int Modelid)const;
private:
    std::vector<int> m_hndl;
};