#include "FFShamanClassHandler.h"

const std::unordered_map<uint8, std::vector<uint32>> FF_SHAMAN_SPELLS =
{
    { 1, { 8017 } },
    { 4, { 8042 } },
    { 6, { 2484, 332 } },
    { 8, { 324, 5730, 8018, 8044, 529 } },
    { 10, { 8024, 8075, 8050 } },
    { 12, { 1535, 370, 2008, 547 } },
    { 14, { 8045, 548, 8154 } },
    { 16, { 57994, 2645, 526, 325, 8019 } },
    { 18, { 8143, 913, 8027, 8052, 6390 } },
    { 20, { 52127, 8033, 8004, 8056, 915, 6363 } },
    { 22, { 131, 8498 } },
    { 24, { 905, 939, 8155, 8181, 8046, 10399, 20609, 8160 } },
    { 26, { 6196, 5675, 8030, 8190, 943 } },
    { 28, { 546, 8038, 8008, 8227, 6391, 8053, 8184, 52129 } },
    { 30, { 66842, 51730, 36936, 20608, 8232, 8177, 556, 6375, 10595, 6364 } },
    { 32, { 8512, 421, 8012, 945, 6041, 959, 8499 } },
    { 34, { 10406, 6495, 52131, 8058 } },
    { 36, { 20610, 10412, 10495, 16339, 8010, 10585 } },
    { 38, { 8170, 10391, 6392, 8249, 8161, 10456, 10478 } },
    { 40, { 1064, 8235, 930, 66843, 10447, 51988, 6365, 8134 } },
    { 40, { 6377, 8005, 52134 } },
    { 42, { 10537, 11314 } },
    { 44, { 10466, 10392, 10407, 10600 } },
    { 46, { 10472, 10496, 16341, 10586, 10622 } },
    { 48, { 10413, 10427, 2860, 10395, 52136, 10431, 10526, 16355, 20776 } },
    { 50, { 66844, 10486, 51991, 10462, 15207, 10437 } },
    { 52, { 11315, 10442, 10448, 10467 } },
    { 54, { 10623, 10408, 10479 } },
    { 55, { 52138 } },
    { 56, { 10432, 16342, 10605, 10396, 15208, 10587, 10497 } },
    { 58, { 10428, 10473, 10538, 16356, 16387 } },
    { 60, { 10414, 10463, 10468, 29228, 16362, 10438, 10601, 51992, 20777, 25357, 25361 } },
    { 61, { 25422, 25546 } },
    { 62, { 25448, 24398 } },
    { 63, { 25391, 25508, 25439, 25469 } },
    { 64, { 3738, 25489 } },
    { 65, { 25528, 25570, 25552 } },
    { 66, { 2062, 25420, 25500 } },
    { 67, { 25525, 25449, 25557, 25560 } },
    { 68, { 2894, 25505, 25423, 25464, 25563 } },
    { 69, { 25590, 33736, 25454, 25567, 25533, 25574 } },
    { 70, { 25442, 25547, 25396, 25457, 51993, 25472, 25509 } },
    { 71, { 58801, 58785, 58794, 58755, 58649, 58580, 58771, 58699 } },
    { 72, { 49275 } },
    { 73, { 58731, 49235, 49237, 58751 } },
    { 74, { 49230, 55458, 49270 } },
    { 75, { 51505, 49232, 49272, 58581, 58652, 49280, 57622, 58703, 58737, 58741, 58746, 61649 } },
    { 76, { 57960, 58789, 58803, 58795, 58756, 58773 } },
    { 77, { 49276 } },
    { 78, { 58753, 49236, 58734, 58582 } },
    { 79, { 49238, 49231 } },
    { 80, { 51514, 51994, 58790, 58804, 61657, 58757, 60043, 49271, 58643, 55459, 49233, 49273, 49281, 58656, 58704, 58739, 58774, 58749, 49277, 58745, 58796 } }
};

const std::unordered_map<uint8, std::vector<uint32>> FF_SHAMAN_SPELLS_A =
{
    { 70, { 32182 } },
};

const std::unordered_map<uint8, std::vector<uint32>> FF_SHAMAN_SPELLS_H =
{
    { 70, { 2825 } },
};



const std::vector<uint32> FF_SHAMAN_TALENT_EL = {};
const std::vector<uint32> FF_SHAMAN_TALENT_EN = {};
const std::vector<uint32> FF_SHAMAN_TALENT_R = {};

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
    FFClassHandler::LearnForLevel(p, FF_SHAMAN_SPELLS);

    // FIXME - this is inefficient
    if (p->GetTeamId() == TEAM_ALLIANCE) {
        FFClassHandler::LearnForLevel(p, FF_SHAMAN_SPELLS_A);
    }
    else {
        FFClassHandler::LearnForLevel(p, FF_SHAMAN_SPELLS_H);
    }
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
