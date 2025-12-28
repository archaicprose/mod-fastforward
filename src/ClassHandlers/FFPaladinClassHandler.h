#ifndef FF_CLASS_PALADIN_MGR_H
#define FF_CLASS_PALADIN_MGR_H

#include "FFClassHandler.h"

class FFPaladinClassHandler : public FFClassHandler {
private:
    void LearnWeaponSkills(Player* p) override;
    void LearnArmorSkills(Player* p) override { LearnPlateSkill(p, level); }
public:
    FFPaladinClassHandler(uint8 pLevel, FFSpec* spec) : FFClassHandler(pLevel, spec) {}
    ~FFPaladinClassHandler() {}

    void LearnSkills(Player* p) override;
    void HandleClassFeatures(Player* p) override;
};

#endif // !FF_CLASS_PALADIN_MGR_H
