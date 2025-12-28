#ifndef FF_CLASS_MAGE_MGR_H
#define FF_CLASS_MAGE_MGR_H

#include "FFClassHandler.h"

class FFMageClassHandler : public FFClassHandler {
private:
    void LearnWeaponSkills(Player* p) override;
public:
    FFMageClassHandler(uint8 pLevel, FFSpec* spec) : FFClassHandler(pLevel, spec) {}
    ~FFMageClassHandler() {}

    void LearnSkills(Player* p) override;
    void HandleClassFeatures(Player* p) override;
};

#endif // !FF_CLASS_MAGE_MGR_H
