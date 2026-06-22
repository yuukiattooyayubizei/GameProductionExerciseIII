#pragma once
#include<vector>
#include"../ObjectCommon.h"

class CEnemyModelManager
{
private:
    static constexpr char ENEMY_MODEL_PATH_FRONT[] = "Data/Model/Enemy";
    static constexpr char ENEMY_MODEL_PATH_BACK[] = ".x";
private:
    void LoadModel();
public:
    CEnemyModelManager();
    ~CEnemyModelManager();

    int GetModel(EnemyType Modelid)const;
private:
    std::vector<int> m_hndl;
};