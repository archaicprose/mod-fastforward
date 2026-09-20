
#ifndef FF_SHARED_H
#define FF_SHARED_H

#include "Player.h"

struct FFSpec {
    TalentTree ffspec;
    Classes ffclass;
    uint8 fforiginal;
};

enum FFSpecDefinition {
    WARRIOR_ARMS = 1,
    WARRIOR_FURY = 2,
    WARRIOR_PROTECTION = 3,
    PALADIN_HOLY = 4,
    PALADIN_PROTECTION = 5,
    PALADIN_RETRIBUTION = 6,
    HUNTER_BEAST_MASTERY = 7,
    HUNTER_MARKSMANSHIP = 8,
    HUNTER_SURVIVAL = 9,
    ROGUE_ASSASSINATION = 10,
    ROGUE_COMBAT = 11,
    ROGUE_SUBTLETY = 12,
    PRIEST_DISCIPLINE = 13,
    PRIEST_HOLY = 14,
    PRIEST_SHADOW = 15,
    DEATH_KNIGHT_BLOOD = 16,
    DEATH_KNIGHT_FROST = 17,
    DEATH_KNIGHT_UNHOLY = 18,
    SHAMAN_ELEMENTAL = 19,
    SHAMAN_ENHANCEMENT = 20,
    SHAMAN_RESTORATION = 21,
    MAGE_ARCANE = 22,
    MAGE_FIRE = 23,
    MAGE_FROST = 24,
    WARLOCK_AFFLICTION = 25,
    WARLOCK_DEMONOLOGY = 26,
    WARLOCK_DESTRUCTION = 27,
    DRUID_BALANCE = 28,
    DRUID_FERAL_COMBAT = 29,
    DRUID_RESTORATION = 30,
    DRUID_FERAL_TANK = 99,  // special value for stat prioritization

    UNDECIDED_PROTECTION = 101, // Protection spec, Warrior or Paladin
    UNDECIDED_HOLY = 102,       // Holy spec, Paladin or Priest
    UNDECIDED_FROST = 103,      // Frost spec, Mage or Death Knight
    UNDECIDED_RESTORATION = 104 // Restoration spec, Shaman or Druid
};

enum FFStageDefinition {
    TIER_ONE = 101, // Molten Core
    TIER_TWO = 102, // Blackwing Lair
    TIER_THREE = 103, // Naxxramas
    TIER_FOUR = 104, // Karazhan, Gruul's Lair, Magtheridon's Lair
    TIER_FIVE = 105, // Serpentshrine Cavern, The Eye
    TIER_SIX = 106, // Battle for Mount Hyjal, Black Temple, Sunwell Plateau
    TIER_SEVEN = 107, // Naxxramas, Obsidian Sanctum
    TIER_EIGHT = 108, // Ulduar
    TIER_NINE = 109, // Crusaders' Coliseum
    TIER_TEN = 110, // Icecrown Citadel
};

class FFSpecMgr {
public:
    static FFSpec* GetSpecInfo(uint8 spec, uint8 classActual);
};

#endif // !FF_SHARED_H
