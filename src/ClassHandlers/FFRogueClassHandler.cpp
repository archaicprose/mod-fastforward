#include "FFRogueClassHandler.h"

const std::unordered_map<uint8, std::vector<uint32>> FF_ROGUE_SPELLS =
{
    { 4, { 921, 53 } },
    { 6, { 1776, 1757 } },
    { 8, { 5277, 6760 } },
    { 10, { 5171, 6770, 2983 } },
    { 12, { 1766, 2589 } },
    { 14, { 8647, 703, 1758 } },
    { 16, { 1966, 6761 } },
    { 18, { 8676 } },
    { 20, { 51722, 1943, 2590 } },
    { 22, { 1725, 1856, 1759, 8631 } },
    { 24, { 6762, 2836 } },
    { 26, { 1833, 8724 } },
    { 28, { 2591, 8639, 6768, 2070 } },
    { 30, { 1760, 408, 1842, 8632 } },
    { 32, { 8623 } },
    { 34, { 2094, 8696, 8725 } },
    { 36, { 8721, 8640 } },
    { 38, { 8633, 8621 } },
    { 40, { 1860, 8624, 8637 } },
    { 42, { 6774, 1857, 11267 } },
    { 44, { 11273, 11279 } },
    { 46, { 11289, 11293 } },
    { 48, { 11297, 11299 } },
    { 50, { 8643, 26669, 11268 } },
    { 52, { 11303, 11274, 11280 } },
    { 54, { 11294, 11290 } },
    { 56, { 11300 } },
    { 58, { 11269, 11305 } },
    { 60, { 11275, 31016, 11281, 25300, 25302 } },
    { 61, { 26839 } },
    { 62, { 32645, 26889, 26861 } },
    { 64, { 27448, 26679, 26865 } },
    { 66, { 31224, 27441 } },
    { 68, { 26863, 26867 } },
    { 69, { 32684 } },
    { 70, { 5938, 26862, 26884, 48673, 48689 } },
    { 71, { 51724 } },
    { 72, { 48658 } },
    { 73, { 48667 } },
    { 74, { 57992, 48671, 48656 } },
    { 75, { 57934, 48690, 48675 } },
    { 76, { 48674, 48637 } },
    { 78, { 48659 } },
    { 79, { 48668, 48672 } },
    { 80, { 51723, 48638, 48676, 57993, 48691, 48657 } },
};

//const std::vector<uint32> FF_ROGUE_TALENT_A = {};
//const std::vector<uint32> FF_ROGUE_TALENT_C = {};
//const std::vector<uint32> FF_ROGUE_TALENT_S = {};

void FFRogueClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(ONE_H_SWORDS);
    p->learnSpell(ONE_H_MACES);
    p->learnSpell(ONE_H_AXES);
    p->learnSpell(GUNS);
    p->learnSpell(FIST_WEAPONS);
    p->learnSpell(DAGGERS);
    p->learnSpell(CROSSBOWS);
    p->learnSpell(BOWS);
}

void FFRogueClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
    FFClassHandler::LearnForLevel(p, FF_ROGUE_SPELLS);
}

void FFRogueClassHandler::HandleClassFeatures(Player* /* p */)
{

}
