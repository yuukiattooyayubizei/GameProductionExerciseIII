#pragma once
#include "../Item.h"

class CItem3 : public CItem {
public:
    CItem3();
    ~CItem3() override;

    const char* GetName() const override;
    bool Use(CPlayer* player, const std::vector<CObject*>& targets) override;

protected:
    void InitItemType() override;
};
