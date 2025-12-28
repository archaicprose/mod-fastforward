#include "FFShared.h"

FFSpec* FFSpecMgr::GetSpecInfo(uint8 spec) {
    FFSpec* info = new FFSpec();
    info->fforiginal = spec;
    switch (spec)
    {
    case WARRIOR_ARMS:
        info->ffclass = CLASS_WARRIOR;
        info->ffspec = TALENT_TREE_WARRIOR_ARMS;
        break;
    case WARRIOR_FURY:
        info->ffclass = CLASS_WARRIOR;
        info->ffspec = TALENT_TREE_WARRIOR_FURY;
        break;
    case WARRIOR_PROTECTION:
        info->ffclass = CLASS_WARRIOR;
        info->ffspec = TALENT_TREE_WARRIOR_PROTECTION;
        break;
    case PALADIN_HOLY:
        info->ffclass = CLASS_PALADIN;
        info->ffspec = TALENT_TREE_PALADIN_HOLY;
        break;
    case PALADIN_PROTECTION:
        info->ffclass = CLASS_PALADIN;
        info->ffspec = TALENT_TREE_PALADIN_PROTECTION;
        break;
    case PALADIN_RETRIBUTION:
        info->ffclass = CLASS_PALADIN;
        info->ffspec = TALENT_TREE_PALADIN_RETRIBUTION;
        break;
    case HUNTER_BEAST_MASTERY:
        info->ffclass = CLASS_HUNTER;
        info->ffspec = TALENT_TREE_HUNTER_BEAST_MASTERY;
        break;
    case HUNTER_MARKSMANSHIP:
        info->ffclass = CLASS_HUNTER;
        info->ffspec = TALENT_TREE_HUNTER_MARKSMANSHIP;
        break;
    case HUNTER_SURVIVAL:
        info->ffclass = CLASS_HUNTER;
        info->ffspec = TALENT_TREE_HUNTER_SURVIVAL;
        break;
    case ROGUE_ASSASSINATION:
        info->ffclass = CLASS_ROGUE;
        info->ffspec = TALENT_TREE_ROGUE_ASSASSINATION;
        break;
    case ROGUE_COMBAT:
        info->ffclass = CLASS_ROGUE;
        info->ffspec = TALENT_TREE_ROGUE_COMBAT;
        break;
    case ROGUE_SUBTLETY:
        info->ffclass = CLASS_ROGUE;
        info->ffspec = TALENT_TREE_ROGUE_SUBTLETY;
        break;
    case PRIEST_DISCIPLINE:
        info->ffclass = CLASS_PRIEST;
        info->ffspec = TALENT_TREE_PRIEST_DISCIPLINE;
        break;
    case PRIEST_HOLY:
        info->ffclass = CLASS_PRIEST;
        info->ffspec = TALENT_TREE_PRIEST_HOLY;
        break;
    case PRIEST_SHADOW:
        info->ffclass = CLASS_PRIEST;
        info->ffspec = TALENT_TREE_PRIEST_SHADOW;
        break;
    case DEATH_KNIGHT_BLOOD:
        info->ffclass = CLASS_DEATH_KNIGHT;
        info->ffspec = TALENT_TREE_DEATH_KNIGHT_BLOOD;
        break;
    case DEATH_KNIGHT_FROST:
        info->ffclass = CLASS_DEATH_KNIGHT;
        info->ffspec = TALENT_TREE_DEATH_KNIGHT_FROST;
        break;
    case DEATH_KNIGHT_UNHOLY:
        info->ffclass = CLASS_DEATH_KNIGHT;
        info->ffspec = TALENT_TREE_DEATH_KNIGHT_UNHOLY;
        break;
    case SHAMAN_ELEMENTAL:
        info->ffclass = CLASS_SHAMAN;
        info->ffspec = TALENT_TREE_SHAMAN_ELEMENTAL;
        break;
    case SHAMAN_ENHANCEMENT:
        info->ffclass = CLASS_SHAMAN;
        info->ffspec = TALENT_TREE_SHAMAN_ENHANCEMENT;
        break;
    case SHAMAN_RESTORATION:
        info->ffclass = CLASS_SHAMAN;
        info->ffspec = TALENT_TREE_SHAMAN_RESTORATION;
        break;
    case MAGE_ARCANE:
        info->ffclass = CLASS_MAGE;
        info->ffspec = TALENT_TREE_MAGE_ARCANE;
        break;
    case MAGE_FIRE:
        info->ffclass = CLASS_MAGE;
        info->ffspec = TALENT_TREE_MAGE_FIRE;
        break;
    case MAGE_FROST:
        info->ffclass = CLASS_MAGE;
        info->ffspec = TALENT_TREE_MAGE_FROST;
        break;
    case WARLOCK_AFFLICTION:
        info->ffclass = CLASS_WARLOCK;
        info->ffspec = TALENT_TREE_WARLOCK_AFFLICTION;
        break;
    case WARLOCK_DEMONOLOGY:
        info->ffclass = CLASS_WARLOCK;
        info->ffspec = TALENT_TREE_WARLOCK_DEMONOLOGY;
        break;
    case WARLOCK_DESTRUCTION:
        info->ffclass = CLASS_WARLOCK;
        info->ffspec = TALENT_TREE_WARLOCK_DESTRUCTION;
        break;
    case DRUID_BALANCE:
        info->ffclass = CLASS_DRUID;
        info->ffspec = TALENT_TREE_DRUID_BALANCE;
        break;
    case DRUID_FERAL_COMBAT:
    case DRUID_FERAL_TANK:
        info->ffclass = CLASS_DRUID;
        info->ffspec = TALENT_TREE_DRUID_FERAL_COMBAT;
        break;
    case DRUID_RESTORATION:
        info->ffclass = CLASS_DRUID;
        info->ffspec = TALENT_TREE_DRUID_RESTORATION;
        break;
    default:
        break;
    }
    return info;
}
