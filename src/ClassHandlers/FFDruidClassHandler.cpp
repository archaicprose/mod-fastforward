#include "FFDruidClassHandler.h"

const std::unordered_map<uint8, std::vector<uint32>> FF_DRUID_SPELLS =
{
    { 1, { 1126 } },
    { 4, { 8921, 774 } },
    { 6, { 467, 5177 } },
    { 8, { 339, 5186 } },
    { 10, { 8924, 99, 16689, 5232, 1058 } },
    { 12, { 50769, 5229, 8936 } },
    { 14, { 5187, 5211, 782, 5178 } },
    { 16, { 783, 1066, 779, 1430, 8925 } },
    { 18, { 16857, 770, 2637, 8938, 6808, 1062, 16810 } },
    { 20, { 768, 5215, 20484, 2912, 1079, 5188, 1735, 1082, 6756 } },
    { 22, { 5221, 2908, 2090, 8926, 5179 } },
    { 24, { 1075, 2782, 1822, 5217, 50768, 8939, 780 } },
    { 26, { 2893, 1850, 5189, 8949, 6809 } },
    { 28, { 5209, 8998, 2091, 3029, 8927, 9492, 16811, 5195 } },
    { 30, { 740, 5180, 8940, 6800, 5234, 6798, 20739 } },
    { 32, { 9490, 6778, 5225, 22568, 6785 } },
    { 34, { 3627, 769, 8928, 8950, 8972, 8914, 1823 } },
    { 36, { 22842, 9493, 9005, 50767, 6793, 8941 } },
    { 38, { 6780, 5196, 8955, 8903, 16812, 8992, 18657, 5201 } },
    { 40, { 29166, 9634, 62600, 8907, 20719, 8910, 20742, 8929, 9000, 16914, 8918, 22827 } },
    { 42, { 9745, 6787, 9747, 9750, 8951 } },
    { 44, { 22812, 9756, 1824, 9754, 9752, 9758 } },
    { 46, { 8983, 9821, 9829, 9823, 8905, 9833, 9839 } },
    { 48, { 9849, 9856, 22828, 50766, 9845, 16813, 9852 } },
    { 50, { 9866, 20747, 21849, 17401, 9880, 9884, 9875, 9888, 9862 } },
    { 52, { 9898, 9834, 9840, 9892, 9894 } },
    { 54, { 9830, 9904, 9901, 9910, 9912, 9857, 9908 } },
    { 56, { 22829, 9827, 9889 } },
    { 58, { 33982, 9841, 9850, 17329, 18658, 9881, 33986, 9835, 9853, 9867, 9876 } },
    { 60, { 33943, 25297, 9846, 9885, 20748, 25298, 31018, 31709, 21850, 50765, 17402, 25299, 9863, 9896, 9858 } },
    { 61, { 27001, 26984 } },
    { 62, { 22570, 26978, 26998 } },
    { 63, { 24248, 26981, 26987 } },
    { 64, { 33763, 27003, 26992, 26997 } },
    { 65, { 33357, 26980 } },
    { 66, { 33745 } },
    { 66, { 27005, 27006 } },
    { 67, { 27000, 27008, 26986, 26996 } },
    { 68, { 26989, 27009, 33983, 33987 } },
    { 69, { 27004, 26979, 26994, 26982, 26985, 50764 } },
    { 70, { 40120, 33786, 26991, 27012, 27002, 26990, 26995, 26983, 26988 } },
    { 71, { 62078, 49799, 48442, 50212, 48559 } },
    { 72, { 48573, 48576, 48464, 48450, 48561 } },
    { 73, { 48567, 48479, 48578, 48569 } },
    { 74, { 49802, 53307, 48459, 48377 } },
    { 75, { 52610, 48571, 48462, 48565, 48440, 48563, 48446 } },
    { 76, { 48575 } },
    { 77, { 48562, 48443, 49803, 48560 } },
    { 78, { 48574, 53308, 48465, 48577, 53312 } },
    { 79, { 48477, 48480, 48461, 48378, 50213, 48570, 48579 } },
    { 80, { 50464, 48463, 48470, 48451, 48566, 48469, 48564, 48568, 48441, 48572, 49800, 48447, 48467, 50763 } },
};

//const std::vector<uint32> FF_DRUID_TALENT_B = {};
//const std::vector<uint32> FF_DRUID_TALENT_F = {};
//const std::vector<uint32> FF_DRUID_TALENT_R = {};
//const std::vector<uint32> FF_DRUID_TALENT_G = {}; // Guardian

void FFDruidClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(TWO_H_MACES);
    p->learnSpell(STAVES);
    p->learnSpell(POLEARMS);
    p->learnSpell(ONE_H_MACES);
    p->learnSpell(FIST_WEAPONS);
    p->learnSpell(DAGGERS);
}

void FFDruidClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
    FFClassHandler::LearnForLevel(p, FF_DRUID_SPELLS);
}

void FFDruidClassHandler::HandleClassFeatures(Player* p)
{
    std::vector<uint32> druidQuests = {};

    if (level >= 10) {
        switch (p->GetTeamId())
        {
        case TEAM_ALLIANCE:
            druidQuests.push_back(5923);
            druidQuests.push_back(5921);
            druidQuests.push_back(5929);
            druidQuests.push_back(5931);
            druidQuests.push_back(6001);
            break;
        case TEAM_HORDE:
            druidQuests.push_back(5926);
            druidQuests.push_back(5922);
            druidQuests.push_back(5930);
            druidQuests.push_back(5932);
            druidQuests.push_back(6002);
            break;
        default:
            break;
        }
    }
    if (level >= 16) {
        switch (p->GetTeamId())
        {
        case TEAM_ALLIANCE:
            druidQuests.push_back(26);
            druidQuests.push_back(29);
            druidQuests.push_back(272);
            druidQuests.push_back(5061);
            break;
        case TEAM_HORDE:
            druidQuests.push_back(27);
            druidQuests.push_back(28);
            druidQuests.push_back(30);
            druidQuests.push_back(31);
            break;
        default:
            break;
        }
    }

    for (uint32 questId : druidQuests)
    {
        TryCompleteNewQuest(p, questId);
    }

    // Ensure druids actually have Teleport: Moonglade
    if (!p->HasSpell(18960)) {
        p->learnSpell(18960);
    }
}
