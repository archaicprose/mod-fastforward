#include "FFPaladinClassHandler.h"

const std::unordered_map<uint8, std::vector<uint32>> FF_PALADIN_SPELLS =
{
    { 4, { 20271, 19740 } },
    { 6, { 498, 639 } },
    { 8, { 1152, 853 } },
    { 10, { 1022, 633, 10290 } },
    { 12, { 7328, 53408, 19834 } },
    { 14, { 31789, 647, 19742 } },
    { 16, { 62124, 25780, 7294 } },
    { 18, { 1044 } },
    { 20, { 26573, 20217, 879, 5502, 19750, 643 } },
    { 22, { 20164, 19746, 1026, 19835 } },
    { 24, { 10326, 5599, 10322, 19850, 5588 } },
    { 26, { 1038, 10298, 19939 } },
    { 28, { 53407, 5614, 19876 } },
    { 30, { 20165, 19752, 1042, 10291, 2800, 20116 } },
    { 32, { 19836, 19888 } },
    { 34, { 642, 19852, 19940 } },
    { 36, { 5615, 19891, 10324, 10299 } },
    { 38, { 20166, 10278, 3472 } },
    { 40, { 5589, 1032, 19895, 20922 } },
    { 42, { 4987, 19941, 19837 } },
    { 44, { 24275, 10312, 19853, 19897 } },
    { 46, { 6940, 10328, 10300 } },
    { 48, { 20772, 19899 } },
    { 50, { 2812, 10310, 10292, 19942, 20923 } },
    { 52, { 25782, 10313, 19838, 24274, 19896 } },
    { 54, { 10308, 10329, 25894, 19854 } },
    { 56, { 10301, 19898 } },
    { 58, { 19943 } },
    { 60, { 25898, 25899, 25916, 10314, 10293, 10318, 25292, 20924, 25290, 24239, 25291, 20773, 19900, 25918 } },
    { 62, { 32223, 27135 } },
    { 63, { 27151 } },
    { 65, { 27142, 27143 } },
    { 66, { 27150, 27137 } },
    { 68, { 27180, 27138, 27152 } },
    { 69, { 27139, 27154 } },
    { 70, { 31884, 27173, 27136, 27153, 27141, 27140, 27149 } },
    { 71, { 54428, 48937, 48935 } },
    { 72, { 48816, 48949 } },
    { 73, { 48931, 48800, 48933 } },
    { 74, { 48805, 48941, 48784 } },
    { 75, { 53600, 48781, 48818 } },
    { 76, { 54043, 48943 } },
    { 77, { 48945, 48936, 48938 } },
    { 78, { 48817, 48788, 48947 } },
    { 79, { 48785, 48932, 48801, 48942, 48934, 48950 } },
    { 80, { 53601, 61411, 48819, 48782, 48806 } },
};

const std::unordered_map<uint8, std::vector<uint32>> FF_PALADIN_SPELLS_A =
{
    { 20, { 13819 } },
    { 40, { 23214 } },
    { 64, { 31801 } },
};

const std::unordered_map<uint8, std::vector<uint32>> FF_PALADIN_SPELLS_H =
{
    { 20, { 34769 } },
    { 40, { 34767 } },
    { 64, { 53736 } },
};

//const std::vector<uint32> FF_PALADIN_TALENT_H = {};
//const std::vector<uint32> FF_PALADIN_TALENT_P = {};
//const std::vector<uint32> FF_PALADIN_TALENT_R = {};

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
    FFClassHandler::LearnForLevel(p, FF_PALADIN_SPELLS);

    // FIXME - this is inefficient
    if (p->GetTeamId() == TEAM_ALLIANCE) {
        FFClassHandler::LearnForLevel(p, FF_PALADIN_SPELLS_A);
    }
    else {
        FFClassHandler::LearnForLevel(p, FF_PALADIN_SPELLS_H);
    }
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
