#pragma once
#include "../Item.h"

class CItem2 : public CItem {
public:
    CItem2();
    ~CItem2() override;

    const char* GetName() const override;
    bool Use(CPlayer* player, const std::vector<CObject*>& targets) override;

protected:
    void InitItemType() override;
};
