#pragma once
#include <vector>
#include "../ObjectCommon.h"

class CEnemyModelManager
{
public:
    CEnemyModelManager();
    ~CEnemyModelManager();

    void LoadModel();
    int GetModel(EnemyType Modelid)const;



private:
    std::vector<int> m_hndl;
};