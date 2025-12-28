#include "FFWarlockClassHandler.h"

void FFWarlockClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(WANDS);
    p->learnSpell(STAVES);
    p->learnSpell(SHOOT);
    p->learnSpell(ONE_H_SWORDS);
    p->learnSpell(DAGGERS);
}

void FFWarlockClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
}

void FFWarlockClassHandler::HandleClassFeatures(Player* p)
{
    std::vector<uint32> warlockQuests = {};

    if (level >= 10) {
        // Voidwalker
        switch (p->getRace())
        {
        case RACE_HUMAN:
        case RACE_GNOME:
            warlockQuests.push_back(1685);
            warlockQuests.push_back(1688);
            warlockQuests.push_back(1689);
            break;
        case RACE_ORC:
        case RACE_UNDEAD_PLAYER:
            warlockQuests.push_back(1506);
            warlockQuests.push_back(1501);
            warlockQuests.push_back(1504);
            break;
        case RACE_BLOODELF:
            warlockQuests.push_back(10788);
            warlockQuests.push_back(9529);
            warlockQuests.push_back(9619);
            break;
        default:
            break;
        }
    }
    if (level >= 20) {
        // Succubus
        switch (p->getRace())
        {
        case RACE_HUMAN:
        case RACE_GNOME:
            warlockQuests.push_back(1717);
            warlockQuests.push_back(1716);
            warlockQuests.push_back(1738);
            warlockQuests.push_back(1739);
            break;
        case RACE_ORC:
        case RACE_UNDEAD_PLAYER:
            warlockQuests.push_back(1508);
            warlockQuests.push_back(1509);
            warlockQuests.push_back(1510);
            warlockQuests.push_back(1511);
            warlockQuests.push_back(1515);
            warlockQuests.push_back(1512);
            warlockQuests.push_back(1513);
            break;
        default:
            break;
        }
    }
    if (level >= 30) {
        // Succubus
        switch (p->getRace())
        {
        case RACE_HUMAN:
        case RACE_GNOME:
            warlockQuests.push_back(1798);
            warlockQuests.push_back(1758);
            warlockQuests.push_back(1802);
            warlockQuests.push_back(1804);
            warlockQuests.push_back(1795);
            break;
        case RACE_ORC:
        case RACE_UNDEAD_PLAYER:
            warlockQuests.push_back(2996);
            warlockQuests.push_back(1801);
            warlockQuests.push_back(1803);
            warlockQuests.push_back(1805);
            warlockQuests.push_back(1795);
            break;
        default:
            break;
        }
    }

    for (uint32 questId : warlockQuests)
    {
        TryCompleteNewQuest(p, questId);
    }
}
