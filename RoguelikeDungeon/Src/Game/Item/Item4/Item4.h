#pragma once
#include "../Item.h"

class CItem4 : public CItem {
public:
    CItem4();
    ~CItem4() override;

    const char* GetName() const override;
    bool Use(CPlayer* player, const std::vector<CObject*>& targets) override;

protected:
    void InitItemType() override;
};
