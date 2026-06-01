#pragma once
#include <vector>

class CEnemyModelManager
{
public:
    CEnemyModelManager();
    ~CEnemyModelManager();

    void LoadModel();
    int GetModel(int Modelid)const;



private:
    std::vector<int> m_hndl;
};