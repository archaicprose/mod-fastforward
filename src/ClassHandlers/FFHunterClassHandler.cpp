#include "FFHunterClassHandler.h"

const std::unordered_map<uint8, std::vector<uint32>> FF_HUNTER_SPELLS =
{
    {2, {1494}},
    {4, {1978, 13163}},
    {6, {1130, 3044}},
    {8, {5116, 3127, 14260}},
    {10, {13165, 13549, 19883}},
    {12, {136, 2974, 14281, 20736}},
    {14, {1002, 6197, 1513}},
    {16, {1495, 5118, 13795, 14261}},
    {18, {2643, 13550, 14318, 19884}},
    {20, {674, 781, 1499, 3111, 14282, 34074}},
    {22, {3043, 14323}},
    {24, {1462, 14262, 19885}},
    {26, {3045, 13551, 14302, 19880}},
    {28, {3661, 13809, 14283, 14319}},
    {30, {5384, 13161, 14269, 14288, 14326}},
    {32, {14263, 1543, 19878}},
    {34, {13813, 13552}},
    {36, {3034, 3662, 14284, 14303}},
    {38, {14320}},
    {40, {1510, 13159, 14264, 14310, 14324, 19882}},
    {42, {13553, 14289}},
    {44, {13542, 14270, 14285, 14316}},
    {46, {14304, 14327, 20043}},
    {48, {14265, 14321}},
    {50, {13554, 14294, 19879, 56641}},
    {52, {13543, 14286}},
    {54, {14290, 14317}},
    {56, {14266, 14305, 20190}},
    {58, {13555, 14271, 14295, 14322, 14325}},
    {60, {13544, 14287, 14311, 19263, 19801, 25296, 25294, 25295}},
    {61, {27025}},
    {62, {34120}},
    {63, {27014}},
    {65, {27023}},
    {66, {34026}},
    {67, {27016, 27021, 27022}},
    {68, {27044, 27045, 27046, 34600}},
    {69, {27019}},
    {70, {34477, 36916}},
    {71, {48995, 49051, 49066, 53351}},
    {72, {49055}},
    {73, {49044, 49000}},
    {74, {48989, 49047, 58431, 61846}},
    {75, {53271, 61005}},
    {76, {49071, 53338}},
    {77, {48996, 49067, 49052, 425777}},
    {78, {49056}},
    {79, {49001, 49045}},
    {80, {48990, 49048, 53339, 58434, 60192, 61006, 61847, 62757}},
};

//const std::vector<uint32> FF_HUNTER_TALENT_BM = {};
//const std::vector<uint32> FF_HUNTER_TALENT_M = {};
//const std::vector<uint32> FF_HUNTER_TALENT_S = {};

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
    FFClassHandler::LearnForLevel(p, FF_HUNTER_SPELLS);
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
