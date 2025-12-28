#include "FFWarriorClassHandler.h"

void FFWarriorClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(THROW_WAR);
    p->learnSpell(TWO_H_SWORDS);
    p->learnSpell(TWO_H_MACES);
    p->learnSpell(TWO_H_AXES);
    p->learnSpell(STAVES);
    p->learnSpell(POLEARMS);
    p->learnSpell(ONE_H_SWORDS);
    p->learnSpell(ONE_H_MACES);
    p->learnSpell(ONE_H_AXES);
    p->learnSpell(GUNS);
    p->learnSpell(FIST_WEAPONS);
    p->learnSpell(DAGGERS);
    p->learnSpell(CROSSBOWS);
    p->learnSpell(BOWS);
    p->learnSpell(BLOCK);
}

void FFWarriorClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
}

void FFWarriorClassHandler::HandleClassFeatures(Player* p)
{
    if (level < 10) {
        return;
    }

    std::vector<uint32> warriorQuests = {};

    switch (p->getRace())
    {
    case RACE_ORC:
    case RACE_TROLL:
    case RACE_TAUREN:
        warriorQuests.push_back(1505);
        warriorQuests.push_back(1498);
        warriorQuests.push_back(1502);
        warriorQuests.push_back(1503);
        break;
    case RACE_UNDEAD_PLAYER:
        warriorQuests.push_back(1818);
        warriorQuests.push_back(1819);
        warriorQuests.push_back(1820);
        warriorQuests.push_back(1821);
        break;
    case RACE_NIGHTELF:
        warriorQuests.push_back(1684);
        warriorQuests.push_back(1683);
        break;
    case RACE_DWARF:
    case RACE_GNOME:
        warriorQuests.push_back(1679);
        warriorQuests.push_back(1678);
        warriorQuests.push_back(1680);
        warriorQuests.push_back(1681);
        break;
    case RACE_HUMAN:
        warriorQuests.push_back(1638);
        warriorQuests.push_back(1639);
        warriorQuests.push_back(1640);
        warriorQuests.push_back(1665);
        break;
    default:
        break;
    }

    if (level >= 30) {
        warriorQuests.push_back(1718);
        warriorQuests.push_back(1719);
    }

    for (uint32 questId : warriorQuests)
    {
        TryCompleteNewQuest(p, questId);
    }
}
