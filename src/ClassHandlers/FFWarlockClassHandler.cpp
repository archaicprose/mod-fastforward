#include "FFWarlockClassHandler.h"

const std::unordered_map<uint8, std::vector<uint32>> FF_WARLOCK_SPELLS =
{
    { 1, { 688, 348 } },
    { 4, { 702, 172 } },
    { 6, { 1454, 695 } },
    { 8, { 980, 5782 } },
    { 10, { 1120, 707, 6201, 696 } },
    { 12, { 705, 755, 1108 } },
    { 14, { 689, 6222 } },
    { 16, { 5697, 1455 } },
    { 18, { 5676, 693, 1014 } },
    { 20, { 698, 5784, 706, 5740, 1094, 1088, 3698 } },
    { 22, { 126, 699, 6202, 6205 } },
    { 24, { 5138, 6223, 5500, 8288 } },
    { 26, { 132, 1456, 1714, 17919 } },
    { 28, { 710, 6366, 1106, 6217, 3699 } },
    { 30, { 709, 1098, 2941, 1949, 1086, 20752 } },
    { 32, { 1490, 7646, 6213, 6229 } },
    { 34, { 7648, 6219, 17920, 5699 } },
    { 36, { 2362, 11687, 3700, 7641, 17951 } },
    { 38, { 11711, 7651, 8289 } },
    { 40, { 23161, 11665, 5484, 20755, 11733 } },
    { 42, { 11683, 6789, 11707, 11739, 17921 } },
    { 44, { 11659, 11671, 11693, 11725 } },
    { 46, { 11677, 11721, 11729, 17952, 11688, 11699 } },
    { 48, { 6353, 18647, 11712, 17727 } },
    { 50, { 11719, 11667, 17922, 17925, 20756, 11734 } },
    { 52, { 11694, 11675 } },
    { 52, { 11660, 11708, 11740 } },
    { 54, { 17928, 11672, 11684, 11700 } },
    { 56, { 6215, 17924, 11689, 17953 } },
    { 58, { 11726, 17926, 11713, 17923, 11678, 11730 } },
    { 60, { 603, 11722, 11695, 25309, 11668, 11735, 11661, 25307, 28610, 17728, 25311, 20757 } },
    { 61, { 27224 } },
    { 62, { 28176, 27219 } },
    { 64, { 29722, 27211 } },
    { 65, { 27210, 27216 } },
    { 66, { 29858, 28172, 27250 } },
    { 67, { 27217, 27218, 27259 } },
    { 68, { 29893, 27223, 27213, 27222, 27230 } },
    { 69, { 28189, 27209, 27228, 27220, 27215, 30909, 27212 } },
    { 70, { 27243, 30910, 32231, 30459, 30545, 27238, 27260 } },
    { 71, { 50511, 47812 } },
    { 72, { 61191, 47819, 47886, 47890 } },
    { 73, { 47871, 47859, 47863 } },
    { 74, { 47837, 47814, 47892, 47808, 60219 } },
    { 75, { 47897, 47835, 47824, 47810 } },
    { 76, { 47884, 47856, 47793 } },
    { 77, { 47813, 47855 } },
    { 78, { 47865, 47857, 47891, 47860, 47823, 47888 } },
    { 79, { 47864, 47893, 47809, 47820, 47878, 47815 } },
    { 80, { 58887, 48020, 57946, 47836, 48018, 47811, 47867, 61290, 47825, 47838, 47889, 60220 } },
};

const std::vector<uint32> FF_WARLOCK_TALENT_A = {};
const std::vector<uint32> FF_WARLOCK_TALENT_DEMO = {};
const std::vector<uint32> FF_WARLOCK_TALENT_DESTRO = {};

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
    FFClassHandler::LearnForLevel(p, FF_WARLOCK_SPELLS);
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
