#ifndef FF_CLASS_PRIEST_MGR_H
#define FF_CLASS_PRIEST_MGR_H

#include "FFClassHandler.h"

class FFPriestClassHandler : public FFClassHandler {
private:
    void LearnWeaponSkills(Player* p) override;
public:
    FFPriestClassHandler(uint8 pLevel, FFSpec* spec) : FFClassHandler(pLevel, spec) {}
    ~FFPriestClassHandler() {}

    void LearnSkills(Player* p) override;
    void HandleClassFeatures(Player* p) override;
};

#endif // !FF_CLASS_PRIEST_MGR_H
