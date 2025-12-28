#include "FFRogueClassHandler.h"

void FFRogueClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(ONE_H_SWORDS);
    p->learnSpell(ONE_H_MACES);
    p->learnSpell(ONE_H_AXES);
    p->learnSpell(GUNS);
    p->learnSpell(FIST_WEAPONS);
    p->learnSpell(DAGGERS);
    p->learnSpell(CROSSBOWS);
    p->learnSpell(BOWS);
}

void FFRogueClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
}

void FFRogueClassHandler::HandleClassFeatures(Player* p)
{

}
