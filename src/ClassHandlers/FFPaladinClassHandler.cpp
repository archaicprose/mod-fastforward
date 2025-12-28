#include "FFPaladinClassHandler.h"

void FFPaladinClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(TWO_H_SWORDS);
    p->learnSpell(TWO_H_MACES);
    p->learnSpell(TWO_H_AXES);
    p->learnSpell(POLEARMS);
    p->learnSpell(ONE_H_SWORDS);
    p->learnSpell(ONE_H_MACES);
    p->learnSpell(ONE_H_AXES);
    p->learnSpell(BLOCK);
}

void FFPaladinClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
}

void FFPaladinClassHandler::HandleClassFeatures(Player* p) {
    if (level < 12) {
        return;
    }

    std::vector<uint32> paladinQuests = {};

    switch (p->getRace())
    {
    case RACE_HUMAN:
        paladinQuests.push_back(1642);
        paladinQuests.push_back(1643);
        paladinQuests.push_back(1644);
        paladinQuests.push_back(1780);
        paladinQuests.push_back(1781);
        paladinQuests.push_back(1786);
        paladinQuests.push_back(1787);
        paladinQuests.push_back(1788);
        break;
    case RACE_DWARF:
        paladinQuests.push_back(1646);
        paladinQuests.push_back(1647);
        paladinQuests.push_back(1648);
        paladinQuests.push_back(1778);
        paladinQuests.push_back(1779);
        paladinQuests.push_back(1783);
        paladinQuests.push_back(1784);
        paladinQuests.push_back(1785);
        break;
    case RACE_BLOODELF:
        paladinQuests.push_back(9677);
        paladinQuests.push_back(9678);
        paladinQuests.push_back(9681);
        paladinQuests.push_back(9684);
        paladinQuests.push_back(9685);
        break;
    case RACE_DRAENEI:
        paladinQuests.push_back(10366);
        paladinQuests.push_back(9598);
        paladinQuests.push_back(9600);
        break;
    default:
        break;
    }

    for (uint32 questId : paladinQuests)
    {
        TryCompleteNewQuest(p, questId);
    }
}
