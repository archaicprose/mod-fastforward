#include "FFMageClassHandler.h"

void FFMageClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(WANDS);
    p->learnSpell(STAVES);
    p->learnSpell(SHOOT);
    p->learnSpell(ONE_H_SWORDS);
    p->learnSpell(DAGGERS);
}

void FFMageClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
}

void FFMageClassHandler::HandleClassFeatures(Player* p)
{

}
