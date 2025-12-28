#ifndef FF_CLASS_WARRIOR_MGR_H
#define FF_CLASS_WARRIOR_MGR_H

#include "FFClassHandler.h"

class FFWarriorClassHandler : public FFClassHandler {
private:
    void LearnWeaponSkills(Player* p) override;
    void LearnArmorSkills(Player* p) override { LearnPlateSkill(p, level); }
public:
    FFWarriorClassHandler(uint8 pLevel, FFSpec* spec) : FFClassHandler(pLevel, spec) {}
    ~FFWarriorClassHandler() {}

    void LearnSkills(Player* p) override;
    void HandleClassFeatures(Player* p) override;
};

#endif // !FF_CLASS_WARRIOR_MGR_H
