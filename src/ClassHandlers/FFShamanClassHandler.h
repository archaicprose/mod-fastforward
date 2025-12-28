#ifndef FF_CLASS_SHAMAN_MGR_H
#define FF_CLASS_SHAMAN_MGR_H

#include "FFClassHandler.h"

class FFShamanClassHandler : public FFClassHandler {
private:
    void LearnWeaponSkills(Player* p) override;
    void LearnArmorSkills(Player* p) override { LearnMailSkill(p, level); }
public:
    FFShamanClassHandler(uint8 pLevel, FFSpec* spec) : FFClassHandler(pLevel, spec) {}
    ~FFShamanClassHandler() {}

    void LearnSkills(Player* p) override;
    void HandleClassFeatures(Player* p) override;
};

#endif // !FF_CLASS_SHAMAN_MGR_H
