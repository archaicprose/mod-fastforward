#ifndef FF_CLASS_DK_MGR_H
#define FF_CLASS_DK_MGR_H

#include "FFClassHandler.h"

class FFDeathKnightClassHandler : public FFClassHandler {
private:
    void LearnWeaponSkills(Player* p) override;
    void LearnArmorSkills(Player* p) override { LearnPlateSkill(p, level); }
public:
    FFDeathKnightClassHandler(uint8 pLevel, FFSpec* spec) : FFClassHandler(pLevel, spec) {}
    ~FFDeathKnightClassHandler() {}

    void LearnSkills(Player* p) override;
    void HandleClassFeatures(Player* p) override;
};

#endif // !FF_CLASS_DK_MGR_H
