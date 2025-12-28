#include "FFShamanClassHandler.h"

void FFShamanClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(TWO_H_MACES);
    p->learnSpell(TWO_H_AXES);
    p->learnSpell(STAVES);
    p->learnSpell(ONE_H_MACES);
    p->learnSpell(ONE_H_AXES);
    p->learnSpell(FIST_WEAPONS);
    p->learnSpell(DAGGERS);
    p->learnSpell(BLOCK);
}

void FFShamanClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
}

void FFShamanClassHandler::HandleClassFeatures(Player* p)
{
    std::vector<uint32> shamanQuests = {};

    if (level >= 4) {
        // Call of Earth
        switch (p->getRace())
        {
        case RACE_ORC:
        case RACE_TROLL:
            shamanQuests.push_back(1516);
            shamanQuests.push_back(1517);
            shamanQuests.push_back(1518);
            break;
        case RACE_TAUREN:
            shamanQuests.push_back(1519);
            shamanQuests.push_back(1520);
            shamanQuests.push_back(1521);
            break;
        case RACE_DRAENEI:
            shamanQuests.push_back(9449);
            shamanQuests.push_back(9450);
            shamanQuests.push_back(9451);
            break;
        default:
            break;
        }
    }
    if (level >= 10) {
        // Call of Fire
        switch (p->getRace())
        {
        case RACE_ORC:
        case RACE_TROLL:
        case RACE_TAUREN:
            shamanQuests.push_back(1522);
            shamanQuests.push_back(1524);
            shamanQuests.push_back(1525);
            shamanQuests.push_back(1526);
            shamanQuests.push_back(1527);
            break;
        case RACE_DRAENEI:
            shamanQuests.push_back(9462);
            shamanQuests.push_back(9464);
            shamanQuests.push_back(9465);
            shamanQuests.push_back(9467);
            shamanQuests.push_back(9468);
            shamanQuests.push_back(9461);
            shamanQuests.push_back(9555);
            break;
        default:
            break;
        }
    }
    if (level >= 20) {
        // Call of Water
        switch (p->getRace())
        {
        case RACE_ORC:
        case RACE_TROLL:
        case RACE_TAUREN:
            shamanQuests.push_back(1528);
            shamanQuests.push_back(1530);
            shamanQuests.push_back(1535);
            shamanQuests.push_back(1536);
            shamanQuests.push_back(1534);
            shamanQuests.push_back(220);
            shamanQuests.push_back(63);
            shamanQuests.push_back(100);
            shamanQuests.push_back(96);
            break;
        case RACE_DRAENEI:
            shamanQuests.push_back(9500);
            shamanQuests.push_back(9501);
            shamanQuests.push_back(9503);
            shamanQuests.push_back(9504);
            shamanQuests.push_back(9508);
            shamanQuests.push_back(9509);
            break;
        default:
            break;
        }
    }
    if (level >= 30) {
        // Call of Air
        switch (p->getRace())
        {
        case RACE_ORC:
        case RACE_TROLL:
            shamanQuests.push_back(1531);
            break;
        case RACE_TAUREN:
            shamanQuests.push_back(1532);
            break;
        case RACE_DRAENEI:
            shamanQuests.push_back(9547);
            shamanQuests.push_back(9552);
            shamanQuests.push_back(9553);
            shamanQuests.push_back(9554);
            break;
        default:
            break;
        }
    }

    for (uint32 questId : shamanQuests)
    {
        TryCompleteNewQuest(p, questId);
    }
}
