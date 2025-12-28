#ifndef FF_CLASS_DRUID_MGR_H
#define FF_CLASS_DRUID_MGR_H

#include "FFClassHandler.h"

class FFDruidClassHandler : public FFClassHandler {
private:
    void LearnWeaponSkills(Player* p) override;
public:
    FFDruidClassHandler(uint8 pLevel, FFSpec* spec) : FFClassHandler(pLevel, spec) {}
    ~FFDruidClassHandler() {}

    void LearnSkills(Player* p) override;
    void HandleClassFeatures(Player* p) override;
};
#endif // !FF_CLASS_DRUID_MGR_H
