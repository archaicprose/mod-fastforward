#ifndef FF_CLASS_WARLOCK_MGR_H
#define FF_CLASS_WARLOCK_MGR_H

#include "FFClassHandler.h"

class FFWarlockClassHandler : public FFClassHandler {
private:
    void LearnWeaponSkills(Player* p) override;
public:
    FFWarlockClassHandler(uint8 pLevel, FFSpec* spec) : FFClassHandler(pLevel, spec) {}
    ~FFWarlockClassHandler() {}

    void LearnSkills(Player* p) override;
    void HandleClassFeatures(Player* p) override;
};

#endif // !FF_CLASS_WARLOCK_MGR_H
