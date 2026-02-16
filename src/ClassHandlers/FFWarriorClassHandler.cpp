#include "FFWarriorClassHandler.h"

const std::unordered_map<uint8, std::vector<uint32>> FF_WARRIOR_SPELLS =
{
    { 4, { 772, 100 } },
    { 6, { 34428, 6343 } },
    { 8, { 1715, 284 } },
    { 10, { 71, 7386, 355, 2687, 6546 } },
    { 12, { 7384, 72, 5242 } },
    { 14, { 6572, 1160 } },
    { 16, { 2565, 694, 285 } },
    { 18, { 676, 8198 } },
    { 20, { 20230, 845, 12678, 6547 } },
    { 22, { 5246, 6192 } },
    { 24, { 6574, 5308, 1608, 6190 } },
    { 26, { 1161, 6178 } },
    { 28, { 871, 8204 } },
    { 30, { 2458, 20252, 1464, 6548, 7369 } },
    { 32, { 18499, 20658, 11564, 11549 } },
    { 34, { 11554, 7379 } },
    { 36, { 1680 } },
    { 38, { 6552, 8205, 8820 } },
    { 40, { 23922, 11572, 11565, 20660, 11608 } },
    { 42, { 11550 } },
    { 44, { 11600, 11555 } },
    { 46, { 11578, 11604 } },
    { 48, { 23923, 11580, 11566, 20661 } },
    { 50, { 1719, 11573, 11609 } },
    { 52, { 11551 } },
    { 54, { 11605, 11601, 11556, 23924 } },
    { 56, { 20662, 11567 } },
    { 58, { 11581 } },
    { 60, { 25289, 23925, 20569, 11574, 25286, 25288 } },
    { 61, { 25241 } },
    { 62, { 25202 } },
    { 63, { 25269 } },
    { 64, { 23920 } },
    { 65, { 25234 } },
    { 66, { 29707, 25258 } },
    { 67, { 25264 } },
    { 68, { 469, 25208, 25231 } },
    { 69, { 25242, 2048 } },
    { 70, { 30356, 3411, 30324, 30357, 25203, 25236 } },
    { 71, { 64382, 46845 } },
    { 72, { 47519, 47449 } },
    { 73, { 47501, 47470 } },
    { 74, { 47439, 47474 } },
    { 75, { 55694, 47487 } },
    { 76, { 47450, 47465 } },
    { 77, { 47520 } },
    { 78, { 47436, 47502 } },
    { 79, { 47475, 47437 } },
    { 80, { 57755, 47488, 57823, 47440, 47471 } },
};

//const std::vector<uint32> FF_WARRIOR_TALENT_A = {};
//const std::vector<uint32> FF_WARRIOR_TALENT_F = {};
//const std::vector<uint32> FF_WARRIOR_TALENT_P = {};

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
    FFClassHandler::LearnForLevel(p, FF_WARRIOR_SPELLS);
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
