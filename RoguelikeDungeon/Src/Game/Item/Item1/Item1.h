#pragma once
#include "../Item.h"


class CItem1 : public CFieldItem {
public:
    CItem1();


    void Init() override;
    void Load() override;
    void Step() override;
    void Draw() override;
    void Exit() override;

    bool Use(Int2* plPos, std::vector<CObject*>& targets) override;

protected:
};
