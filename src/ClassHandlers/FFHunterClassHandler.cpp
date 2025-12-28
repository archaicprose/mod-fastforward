#include "FFHunterClassHandler.h"

void FFHunterClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(THROW_WAR);
    p->learnSpell(TWO_H_SWORDS);
    p->learnSpell(TWO_H_AXES);
    p->learnSpell(STAVES);
    p->learnSpell(POLEARMS);
    p->learnSpell(ONE_H_SWORDS);
    p->learnSpell(ONE_H_AXES);
    p->learnSpell(GUNS);
    p->learnSpell(FIST_WEAPONS);
    p->learnSpell(DAGGERS);
    p->learnSpell(CROSSBOWS);
    p->learnSpell(BOWS);
}

void FFHunterClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
}

void FFHunterClassHandler::HandleClassFeatures(Player* p)
{
    if (level < 10) {
        return;
    }

    std::vector<uint32> hunterQuests = {};

    switch (p->getRace())
    {
    case RACE_ORC:
    case RACE_TROLL:
        hunterQuests.push_back(6068);
        hunterQuests.push_back(6062);
        hunterQuests.push_back(6083);
        hunterQuests.push_back(6082);
        hunterQuests.push_back(6081);
        break;
    case RACE_TAUREN:
        hunterQuests.push_back(6065);
        hunterQuests.push_back(6061);
        hunterQuests.push_back(6087);
        hunterQuests.push_back(6088);
        hunterQuests.push_back(6089);
        break;
    case RACE_BLOODELF:
        hunterQuests.push_back(9617);
        hunterQuests.push_back(9484);
        hunterQuests.push_back(9486);
        hunterQuests.push_back(9485);
        hunterQuests.push_back(9673);
        break;
    case RACE_NIGHTELF:
        hunterQuests.push_back(6071);
        hunterQuests.push_back(6063);
        hunterQuests.push_back(6101);
        hunterQuests.push_back(6102);
        hunterQuests.push_back(6103);
        break;
    case RACE_DWARF:
        hunterQuests.push_back(6074);
        hunterQuests.push_back(6064);
        hunterQuests.push_back(6084);
        hunterQuests.push_back(6085);
        hunterQuests.push_back(6086);
        break;
    case RACE_DRAENEI:
        hunterQuests.push_back(9757);
        hunterQuests.push_back(9591);
        hunterQuests.push_back(9592);
        hunterQuests.push_back(9593);
        hunterQuests.push_back(9675);
        break;
    default:
        break;
    }

    for (uint32 questId : hunterQuests)
    {
        TryCompleteNewQuest(p, questId);
    }
}
