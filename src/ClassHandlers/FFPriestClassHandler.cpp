#include "FFPriestClassHandler.h"

void FFPriestClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(WANDS);
    p->learnSpell(STAVES);
    p->learnSpell(SHOOT);
    p->learnSpell(ONE_H_MACES);
    p->learnSpell(DAGGERS);
}

void FFPriestClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
}

void FFPriestClassHandler::HandleClassFeatures(Player* p)
{

}
