#pragma once
#include "../Item.h"

class CItem1 : public CItem {
public:
    CItem1();
    ~CItem1() override;

    const char* GetName() const override;
    bool Use(CPlayer* player, const std::vector<CObject*>& targets) override;

protected:
    void InitItemType() override;
};
