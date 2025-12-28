#ifndef FF_CLASS_ROGUE_MGR_H
#define FF_CLASS_ROGUE_MGR_H

#include "FFClassHandler.h"

class FFRogueClassHandler : public FFClassHandler {
private:
    void LearnWeaponSkills(Player* p) override;
public:
    FFRogueClassHandler(uint8 pLevel, FFSpec* spec) : FFClassHandler(pLevel, spec) {}
    ~FFRogueClassHandler() {}

    void LearnSkills(Player* p) override;
    void HandleClassFeatures(Player* p) override;
};

#endif // !FF_CLASS_ROGUE_MGR_H
