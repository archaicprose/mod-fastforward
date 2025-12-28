#ifndef FF_CLASS_HUNTER_MGR_H
#define FF_CLASS_HUNTER_MGR_H

#include "FFClassHandler.h"

class FFHunterClassHandler : public FFClassHandler {
private:
    void LearnWeaponSkills(Player* p) override;
    void LearnArmorSkills(Player* p) override { LearnMailSkill(p, level); }
public:
    FFHunterClassHandler(uint8 pLevel, FFSpec* spec) : FFClassHandler(pLevel, spec) {}
    ~FFHunterClassHandler() {}

    void LearnSkills(Player* p) override;
    void HandleClassFeatures(Player* p) override;
};

#endif // !FF_CLASS_HUNTER_MGR_H
