#include "FFItemMgr.h"

uint32 FFItemMgr::GetArmorForInventoryType(std::string inventoryTypesStr)
{
    std::string classStr = "4";
    std::string subclassStr = "'0','1','2','3','4'";

    switch (specInfo->ffclass)
    {
    case CLASS_WARRIOR:
    case CLASS_PALADIN:
    case CLASS_DEATH_KNIGHT:
        if (level >= 50) {
            subclassStr = "4";
        }
        else if (level >= 40) {
            // mix of mail and plate to ensure hits
            subclassStr = "3,4";
        }
        else {
            subclassStr = "3";
        }
        break;
    case CLASS_HUNTER:
    case CLASS_SHAMAN:
        if (level >= 45) {
            subclassStr = "3";
        }
        else if (level >= 40) {
            // mix of leather and mail to ensure hits
            subclassStr = "2,3";
        }
        else {
            subclassStr = "2";
        }
        break;
    case CLASS_PRIEST:
    case CLASS_MAGE:
    case CLASS_WARLOCK:
        subclassStr = "1";
        break;
    case CLASS_ROGUE:
    case CLASS_DRUID:
        subclassStr = "2";
        break;
    default:
        break;
    }

    return GetItemForInventoryType(classStr, subclassStr, inventoryTypesStr);
}

uint32 FFItemMgr::GetWeaponForInventoryType(std::string subclassStr, std::string inventoryTypesStr)
{
    std::string classStr = "2";
    return GetItemForInventoryType(classStr, subclassStr, inventoryTypesStr);
}

uint32 FFItemMgr::GetOffhandWeaponForInventoryType(std::string subclassStr, std::string inventoryTypesStr, uint32 primaryWeapon)
{
    std::string classStr = "2";
    uint8 reqLevelLow = level - 5;
    uint8 reqLevelHigh = level - 1;
    QueryResult result = WorldDatabase.Query("SELECT `entry`,`ItemLevel`,`RequiredLevel`,`stat_type1`,`stat_value1`,`stat_type2`,`stat_value2`,`stat_type3`,`stat_value3`,`stat_type4`,`stat_value4`,`stat_type5`,`stat_value5`,`stat_type6`,`stat_value6`,`stat_type7`,`stat_value7`,`stat_type8`,`stat_value8`,`stat_type9`,`stat_value9`,`stat_type10`,`stat_value10`,`dmg_min1`,`dmg_max1`,`dmg_min2`,`dmg_max2`,`armor`,`delay` FROM `item_template` WHERE class = '{}' AND subclass IN ({}) AND InventoryType IN ({}) AND RequiredLevel BETWEEN '{}' AND '{}' AND RequiredSkill = 0 AND RequiredSpell = 0 AND Quality <= 3 AND (Flags&16) = 0 AND (FlagsExtra&3) = 0 AND (AllowableClass = -1 OR AllowableClass = 262143) AND name NOT LIKE 'TEST%' AND RandomProperty = 0 AND RandomSuffix = 0 AND entry <> '{}' ORDER BY ItemLevel, BuyPrice DESC Limit 8", classStr, subclassStr, inventoryTypesStr, reqLevelLow, reqLevelHigh, primaryWeapon);
    return GetBestItemFromResults(result);
}

uint32 FFItemMgr::GetMiscForInventoryType(std::string inventoryTypesStr)
{
    return GetItemForInventoryType("4", "0", inventoryTypesStr);
}

uint32 FFItemMgr::GetItemForInventoryType(std::string classStr, std::string subclassStr, std::string inventoryTypesStr)
{
    uint8 reqLevelLow = level - 5;
    uint8 reqLevelHigh = level - 1;
    QueryResult result = WorldDatabase.Query("SELECT `entry`,`ItemLevel`,`RequiredLevel`,`stat_type1`,`stat_value1`,`stat_type2`,`stat_value2`,`stat_type3`,`stat_value3`,`stat_type4`,`stat_value4`,`stat_type5`,`stat_value5`,`stat_type6`,`stat_value6`,`stat_type7`,`stat_value7`,`stat_type8`,`stat_value8`,`stat_type9`,`stat_value9`,`stat_type10`,`stat_value10`,`dmg_min1`,`dmg_max1`,`dmg_min2`,`dmg_max2`,`armor`,`delay` FROM `item_template` WHERE class = '{}' AND subclass IN ({}) AND InventoryType IN ({}) AND RequiredLevel BETWEEN '{}' AND '{}' AND RequiredSkill = 0 AND RequiredSpell = 0 AND Quality <= 3 AND (Flags&16) = 0 AND (FlagsExtra&3) = 0 AND (AllowableClass = -1 OR AllowableClass = 262143) AND name NOT LIKE 'TEST%' AND RandomProperty = 0 AND RandomSuffix = 0 ORDER BY ItemLevel, BuyPrice DESC Limit 8", classStr, subclassStr, inventoryTypesStr, reqLevelLow, reqLevelHigh);
    return GetBestItemFromResults(result);
}

uint32 FFItemMgr::GetSecondaryItemForInventoryType(std::string classStr, std::string subclassStr, std::string inventoryTypesStr, uint32 primaryItem)
{
    uint8 reqLevelLow = level - 5;
    uint8 reqLevelHigh = level - 1;
    QueryResult result = WorldDatabase.Query("SELECT `entry`,`ItemLevel`,`RequiredLevel`,`stat_type1`,`stat_value1`,`stat_type2`,`stat_value2`,`stat_type3`,`stat_value3`,`stat_type4`,`stat_value4`,`stat_type5`,`stat_value5`,`stat_type6`,`stat_value6`,`stat_type7`,`stat_value7`,`stat_type8`,`stat_value8`,`stat_type9`,`stat_value9`,`stat_type10`,`stat_value10`,`dmg_min1`,`dmg_max1`,`dmg_min2`,`dmg_max2`,`armor`,`delay` FROM `item_template` WHERE class = '{}' AND subclass IN ({}) AND InventoryType IN ({}) AND RequiredLevel BETWEEN '{}' AND '{}' AND RequiredSkill = 0 AND RequiredSpell = 0 AND Quality <= 3 AND (Flags&16) = 0 AND (FlagsExtra&3) = 0 AND (AllowableClass = -1 OR AllowableClass = 262143) AND name NOT LIKE 'TEST%' AND RandomProperty = 0 AND RandomSuffix = 0 AND entry <> '{}' ORDER BY ItemLevel, BuyPrice DESC Limit 8", classStr, subclassStr, inventoryTypesStr, reqLevelLow, reqLevelHigh, primaryItem);
    return GetBestItemFromResults(result);
}

uint32 FFItemMgr::GetBestItemFromResults(QueryResult result) {
    uint32 bestItem = 0;
    double highestScore = 0.0f;
    if (result) {
        do
        {
            Field* fields = result->Fetch();

            // sanity check usable
            uint8 requiredLevel = fields[2].Get<uint8>();
            if (level >= requiredLevel) {

                // prepare scoring fields
                uint16 itemLevel = fields[1].Get<uint16>();
                std::vector<uint8> statArray;
                std::vector<std::int32_t> statValArray;
                for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; i++)
                {
                    uint8 statTypeIndex = 3 + i * 2;

                    if (fields[statTypeIndex].Get<uint8>() == 0) {
                        break;
                    }

                    statArray.push_back(fields[statTypeIndex].Get<uint8>());
                    statValArray.push_back(fields[statTypeIndex + 1].Get<int32_t>());
                }

                float dmgMin1 = fields[23].Get<float>();
                float dmgMax1 = fields[24].Get<float>();
                float dmgMin2 = fields[25].Get<float>();
                float dmgMax2 = fields[26].Get<float>();
                uint32 armor = fields[27].Get<uint32>();
                uint16 delay = fields[28].Get<uint16>();

                double itemScore = itemScorer->GetScoreForItemStats(itemLevel, statArray, statValArray, dmgMin1, dmgMax1, dmgMin2, dmgMax2, armor, delay);

                if (itemScore > highestScore) {
                    highestScore = itemScore;
                    bestItem = fields[0].Get<uint32>();
                }
            }
        } while (result->NextRow());
    }

    return bestItem;
}

Classes FFItemMgr::GetIntendedClass()
{
    return specInfo->ffclass;
}

uint32 FFItemMgr::GetHeadItem()
{
    return GetArmorForInventoryType("1");
}

uint32 FFItemMgr::GetNeckItem()
{
    return GetMiscForInventoryType("2");
}

uint32 FFItemMgr::GetShoulderItem()
{
    return GetArmorForInventoryType("3");
}

uint32 FFItemMgr::GetChestItem()
{
    return GetArmorForInventoryType("5,20");
}

uint32 FFItemMgr::GetWaistItem()
{
    return GetArmorForInventoryType("6");
}

uint32 FFItemMgr::GetLegsItem()
{
    return GetArmorForInventoryType("7");
}

uint32 FFItemMgr::GetFeetItem()
{
    return GetArmorForInventoryType("8");
}

uint32 FFItemMgr::GetWristsItem()
{
    return GetArmorForInventoryType("9");
}

uint32 FFItemMgr::GetHandsItem()
{
    return GetArmorForInventoryType("10");
}

uint32 FFItemMgr::GetFingerItem1()
{
    return GetMiscForInventoryType("11");
}

uint32 FFItemMgr::GetFingerItem2(uint32 primaryFinger)
{
    return (level >= 20) ? GetSecondaryItemForInventoryType("4", "0", "11", primaryFinger) : 0;
}

uint32 FFItemMgr::GetTrinket1()
{
    return (level >= 40) ? GetMiscForInventoryType("12") : 0;
}

uint32 FFItemMgr::GetTrinket2(uint32 primaryTrinket)
{
    return (level >= 58) ? GetSecondaryItemForInventoryType("4", "0", "12", primaryTrinket) : 0;
}

uint32 FFItemMgr::GetBackItem()
{
    // all cloaks are cloth
    return GetItemForInventoryType("4", "1", "16");
}

uint32 FFItemMgr::GetMainHandWeapon()
{
    uint32 mainHandForClass = 0;

    switch (specInfo->ffspec)
    {
    case TALENT_TREE_WARRIOR_ARMS:
        mainHandForClass = GetWeaponForInventoryType("1,5,6,8,13,15,17", "17");
        break;
    case TALENT_TREE_WARRIOR_FURY:
        if (level >= 60 || level < 20) {
            mainHandForClass = GetWeaponForInventoryType("1,5,8", "17"); // 2H Axes, Maces, Swords
        }
        else {
            mainHandForClass = GetWeaponForInventoryType("0,4,7,13,15", "13,21"); // 1H Dual Wield
        }
        break;
    case TALENT_TREE_WARRIOR_PROTECTION:
    case TALENT_TREE_ROGUE_ASSASSINATION:
    case TALENT_TREE_ROGUE_COMBAT:
    case TALENT_TREE_ROGUE_SUBTLETY:
        mainHandForClass = GetWeaponForInventoryType("0,4,7,13,15", "13,21");
        break;
    case TALENT_TREE_PALADIN_HOLY:
    case TALENT_TREE_PALADIN_PROTECTION:
        mainHandForClass = GetWeaponForInventoryType("0,4,7", "13,21");
        break;
    case TALENT_TREE_PALADIN_RETRIBUTION:
        mainHandForClass = GetWeaponForInventoryType("1,5,6,8", "17");
        break;
    case TALENT_TREE_HUNTER_BEAST_MASTERY:
    case TALENT_TREE_HUNTER_MARKSMANSHIP:
    case TALENT_TREE_HUNTER_SURVIVAL:
        if (level < 20) {
            mainHandForClass = GetWeaponForInventoryType("1,8,13,15", "17");
        }
        else {
            mainHandForClass = GetWeaponForInventoryType("0,7,13,15", "13,21");
        }
        break;
    case TALENT_TREE_PRIEST_DISCIPLINE:
    case TALENT_TREE_PRIEST_HOLY:
    case TALENT_TREE_PRIEST_SHADOW:
        mainHandForClass = GetWeaponForInventoryType("4,10,15", "13,17,21");
        break;
    case TALENT_TREE_DEATH_KNIGHT_BLOOD:
    case TALENT_TREE_DEATH_KNIGHT_FROST:
    case TALENT_TREE_DEATH_KNIGHT_UNHOLY:
        mainHandForClass = GetWeaponForInventoryType("1,5,6,8", "17");
        break;
    case TALENT_TREE_SHAMAN_ELEMENTAL:
    case TALENT_TREE_SHAMAN_RESTORATION:
        mainHandForClass = GetWeaponForInventoryType("10,15", "13,21,17");
        break;
    case TALENT_TREE_SHAMAN_ENHANCEMENT:
        mainHandForClass = GetWeaponForInventoryType("0,4,13,15", "13,21");
        break;
    case TALENT_TREE_MAGE_ARCANE:
    case TALENT_TREE_MAGE_FIRE:
    case TALENT_TREE_MAGE_FROST:
    case TALENT_TREE_WARLOCK_AFFLICTION:
    case TALENT_TREE_WARLOCK_DEMONOLOGY:
    case TALENT_TREE_WARLOCK_DESTRUCTION:
        mainHandForClass = GetWeaponForInventoryType("7,10,15", "13,17,21");
        break;
    case TALENT_TREE_DRUID_BALANCE:
    case TALENT_TREE_DRUID_RESTORATION:
        mainHandForClass = GetWeaponForInventoryType("10,15", "13,17,21");
        break;
    case TALENT_TREE_DRUID_FERAL_COMBAT:
        mainHandForClass = GetWeaponForInventoryType("10,15", "13,17,21");
        break;
    default:
        break;
    }

    return mainHandForClass;
}

uint32 FFItemMgr::GetSecondaryItem(uint32 primaryWeapon)
{
    uint32 secondaryItemForClass = 0;

    switch (specInfo->ffspec)
    {
    case TALENT_TREE_WARRIOR_FURY:
        if (level >= 60) {
            secondaryItemForClass = GetOffhandWeaponForInventoryType("1,5,8", "17", primaryWeapon); // Titan Grip
        }
        else if (level >= 20) {
            secondaryItemForClass = GetOffhandWeaponForInventoryType("0,4,7,13,15", "13,22", primaryWeapon); // Dual Wield
        }
        break;
    case TALENT_TREE_WARRIOR_PROTECTION:
    case TALENT_TREE_PALADIN_PROTECTION:
    case TALENT_TREE_PALADIN_HOLY:
        secondaryItemForClass = GetItemForInventoryType("4", "6", "14"); // Shield
        break;
    case TALENT_TREE_HUNTER_BEAST_MASTERY:
    case TALENT_TREE_HUNTER_MARKSMANSHIP:
    case TALENT_TREE_HUNTER_SURVIVAL:
        if (level >= 20) {
            // Dual Wield for stats
            secondaryItemForClass = GetOffhandWeaponForInventoryType("0,7,13,15", "13,22", primaryWeapon);
        }
        break;
    case TALENT_TREE_ROGUE_ASSASSINATION:
    case TALENT_TREE_ROGUE_COMBAT:
    case TALENT_TREE_ROGUE_SUBTLETY:
        if (level >= 10) {
            secondaryItemForClass = GetOffhandWeaponForInventoryType("0,4,7,13,15", "13,22", primaryWeapon); // Dual Wield
        }
        break;
    case TALENT_TREE_SHAMAN_ENHANCEMENT:
        if (level < 40) {
            secondaryItemForClass = GetItemForInventoryType("4", "6", "14"); // Shield
        }
        else {
            secondaryItemForClass = GetOffhandWeaponForInventoryType("0,4,13,15", "13,22", primaryWeapon); // Dual Wield
        }
        break;
    default:
        break;
    }

    return secondaryItemForClass;
}

uint32 FFItemMgr::GetRangeWeapon()
{
    uint32 rangedItemForClass = 0;

    switch (specInfo->ffspec)
    {
    case TALENT_TREE_WARRIOR_ARMS:
    case TALENT_TREE_WARRIOR_FURY:
    case TALENT_TREE_WARRIOR_PROTECTION:
    case TALENT_TREE_ROGUE_ASSASSINATION:
    case TALENT_TREE_ROGUE_COMBAT:
    case TALENT_TREE_ROGUE_SUBTLETY:
        rangedItemForClass = GetWeaponForInventoryType("2,3,16,18", "15,25,26");
        break;
    case TALENT_TREE_HUNTER_BEAST_MASTERY:
    case TALENT_TREE_HUNTER_MARKSMANSHIP:
    case TALENT_TREE_HUNTER_SURVIVAL:
        rangedItemForClass = GetWeaponForInventoryType("2,3,18", "15,25,26");
        break;
    case TALENT_TREE_PRIEST_DISCIPLINE:
    case TALENT_TREE_PRIEST_HOLY:
    case TALENT_TREE_PRIEST_SHADOW:
    case TALENT_TREE_MAGE_ARCANE:
    case TALENT_TREE_MAGE_FIRE:
    case TALENT_TREE_MAGE_FROST:
    case TALENT_TREE_WARLOCK_AFFLICTION:
    case TALENT_TREE_WARLOCK_DEMONOLOGY:
    case TALENT_TREE_WARLOCK_DESTRUCTION:
        rangedItemForClass = GetWeaponForInventoryType("19", "26");
        break;
    default:
        break;
    }

    return rangedItemForClass;
}

uint32* FFItemMgr::GetGlyphs()
{
    // Major, Minor, Major, Minor, Minor, Major
    uint32* glyphArray = new uint32[6];
    switch (specInfo->ffspec)
    {
    case TALENT_TREE_WARRIOR_ARMS:
        glyphArray[0] = 43423;  // Glyph of Rending
        glyphArray[1] = 43399;  // Glyph of Thunder Clap
        glyphArray[2] = 43416;  // Glyph of Execution
        glyphArray[3] = 49084;  // Glyph of Command
        glyphArray[4] = 43396;  // Glyph of Bloodrage
        glyphArray[5] = 43421;  // Glyph of Mortal Strike
        break;
    case TALENT_TREE_WARRIOR_FURY:
        glyphArray[0] = 43418;  // Glyph of Heroic Strike
        glyphArray[1] = 43395;  // Glyph of Battle
        glyphArray[2] = 43432;  // Glyph of Whirlwind
        glyphArray[3] = 49084;  // Glyph of Command
        glyphArray[4] = 43396;  // Glyph of Bloodrage
        glyphArray[5] = 43423;  // Glyph of Rending
        break;
    case TALENT_TREE_WARRIOR_PROTECTION:
        glyphArray[0] = level < 70 ?
            43429 :     // Glyph of Taunt
            45793;      // Glyph of Vigilance
        glyphArray[1] = 43399;  // Glyph of Thunder Clap
        glyphArray[2] = level < 40 ?
            43414 :     // Glyph of Cleaving
            43425;      // Glyph of Blocking
        glyphArray[3] = 43397;  // Glyph of Charge
        glyphArray[4] = 49084;  // Glyph of Command
        glyphArray[5] = 43415;  // Glyph of Devastate
        break;
    case TALENT_TREE_PALADIN_HOLY:
        glyphArray[0] = 41106;  // Glyph of Holy Light
        glyphArray[1] = 43367;  // Glyph of Lay on Hands
        glyphArray[2] = 57034;  // Glyph of Seal of Light
        glyphArray[3] = 43366;  // Glyph of Blessing of Wisdom
        glyphArray[4] = 43365;  // Glyph of Blessing of Kings
        glyphArray[5] = 45741;  // Glyph of Beacon of Light
        break;
    case TALENT_TREE_PALADIN_PROTECTION:
        glyphArray[0] = 41100;  // Glyph of Righteous Defense
        glyphArray[1] = 43367;  // Glyph of Lay on Hands
        glyphArray[2] = 41092;  // Glyph of Judgement
        glyphArray[3] = 43365;  // Glyph of Blessing of Kings
        glyphArray[4] = 43368;  // Glyph of Sense Undead
        glyphArray[5] = 45745;  // Glyph of Divine Plea
        break;
    case TALENT_TREE_PALADIN_RETRIBUTION:
        glyphArray[0] = 41092;  // Glyph of Judgement
        glyphArray[1] = 43340;  // Glyph of Blessing of Might
        glyphArray[2] = 41099;  // Glyph of Consecration
        glyphArray[3] = 43365;  // Glyph of Blessing of Kings
        glyphArray[4] = 43368;  // Glyph of Sense Undead
        glyphArray[5] = 43869;  // Glyph of Seal of Vengeance
        break;
    case TALENT_TREE_HUNTER_BEAST_MASTERY:
        glyphArray[0] = 42912;  // Glyph of Serpent Sting
        glyphArray[1] = 43350;  // Glyph of Mend Pet
        glyphArray[2] = 42902;  // Glyph of Bestial Wrath
        glyphArray[3] = 43351;  // Glyph of Feign Death
        glyphArray[4] = 43338;  // Glyph of Revive Pet
        glyphArray[5] = 42914;  // Glyph of Steady Shot
        break;
    case TALENT_TREE_HUNTER_MARKSMANSHIP:
        glyphArray[0] = 42912;  // Glyph of Serpent Sting
        glyphArray[1] = 43350;  // Glyph of Mend Pet
        glyphArray[2] = level < 62 ?
            45733 :     // Glyph of Explosive Trap
            42914;      // Glyph of Steady Shot
        glyphArray[3] = 43351;  // Glyph of Feign Death
        glyphArray[4] = 43338;  // Glyph of Revive Pet
        glyphArray[5] = 45732;  // Glyph of Kill Shot
        break;
    case TALENT_TREE_HUNTER_SURVIVAL:
        glyphArray[0] = level < 60 ?
            42912 :     // Glyph of Serpent Sting
            45731;      // Glyph of Explosive Shot
        glyphArray[1] = 43350;  // Glyph of Mend Pet
        glyphArray[2] = 45733;  // Glyph of Explosive Trap
        glyphArray[3] = 43351;  // Glyph of Feign Death
        glyphArray[4] = 43338;  // Glyph of Revive Pet
        glyphArray[5] = 45732;  // Glyph of Kill Shot
        break;
    case TALENT_TREE_ROGUE_ASSASSINATION:
        glyphArray[0] = level < 50 ?
            42972 :     // Glyph of Sinister Strike
            45768;      // Glyph of Mutilate
        glyphArray[1] = 43380;  // Glyph of Vanish
        glyphArray[2] = level < 60 ?
            42969 :     // Glyph of Rupture
            45761;      // Glyph of Hunger for Blood
        glyphArray[3] = 43376;  // Glyph of Distract
        glyphArray[4] = 43378;  // Glyph of Safe Fall
        glyphArray[5] = 42971;  // Glyph of Vigor
        break;
    case TALENT_TREE_ROGUE_COMBAT:
        glyphArray[0] = 42972;  // Glyph of Sinister Strike
        glyphArray[1] = 43380;  // Glyph of Vanish
        glyphArray[2] = level < 60 ?
            42969 :     // Glyph of Rupture
            45762;      // Glyph of Killing Spree
        glyphArray[3] = 43379;  // Glyph of Blurred Speed
        glyphArray[4] = 43378;  // Glyph of Safe Fall
        glyphArray[5] = 45767;  // Glyph of Tricks of the Trade
        break;
    case TALENT_TREE_ROGUE_SUBTLETY:
        glyphArray[0] = level < 50 ?
            42972 :     // Glyph of Sinister Strike
            45764;      // Glyph of Shadow Dance
        glyphArray[1] = 43380;  // Glyph of Vanish
        glyphArray[2] = 42967;  // Glyph of Hemorrhage
        glyphArray[3] = 43376;  // Glyph of Distract
        glyphArray[4] = 43378;  // Glyph of Safe Fall
        glyphArray[5] = 42971;  // Glyph of Vigor
        break;
    case TALENT_TREE_PRIEST_DISCIPLINE:
        glyphArray[0] = 42408;  // Glyph of Power Word: Shield
        glyphArray[1] = 43371;  // Glyph of Fortitude
        glyphArray[2] = 42400;  // Glyph of Flash Heal
        glyphArray[3] = 43372;  // Glyph of Shadow Protection
        glyphArray[4] = 43370;  // Glyph of Levitate
        glyphArray[5] = 45756;  // Glyph of Penance
        break;
    case TALENT_TREE_PRIEST_HOLY:
        glyphArray[0] = level < 20 ?
            42408 :     // Glyph of Power Word: Shield
            42400;      // Glyph of Flash Heal
        glyphArray[1] = 43371;  // Glyph of Fortitude
        glyphArray[2] = 42409;  // Glyph of Prayer of Healing
        glyphArray[3] = 43372;  // Glyph of Shadow Protection
        glyphArray[4] = 43370;  // Glyph of Levitate
        glyphArray[5] = 42396;  // Glyph of Circle of Healing
        break;
    case TALENT_TREE_PRIEST_SHADOW:
        glyphArray[0] = 42415;  // Glyph of Mind Flay
        glyphArray[1] = 43371;  // Glyph of Fortitude
        glyphArray[2] = 42407;  // Glyph of Shadow
        glyphArray[3] = 43372;  // Glyph of Shadow Protection
        glyphArray[4] = 43370;  // Glyph of Levitate
        glyphArray[5] = 42414;  // Glyph of Shadow Word: Death
        break;
    case TALENT_TREE_DEATH_KNIGHT_BLOOD:
        glyphArray[0] = 43538;  // Glyph of Dark Command
        glyphArray[1] = 43535;  // Glyph of Blood Tap
        glyphArray[2] = 45805;  // Glyph of Disease
        glyphArray[3] = 43672;  // Glyph of Pestilence
        glyphArray[4] = 43673;  // Glyph of Raise Dead
        glyphArray[5] = 43827;  // Glyph of Death Strike
        break;
    case TALENT_TREE_DEATH_KNIGHT_FROST:
        glyphArray[0] = 43543;  // Glyph of Frost Strike
        glyphArray[1] = 43544;  // Glyph of Horn of Winter
        glyphArray[2] = 45805;  // Glyph of Disease
        glyphArray[3] = 43672;  // Glyph of Pestilence
        glyphArray[4] = 43673;  // Glyph of Raise Dead
        glyphArray[5] = 43547;  // Glyph of Obliterate
        break;
    case TALENT_TREE_DEATH_KNIGHT_UNHOLY:
        glyphArray[0] = 43546;  // Glyph of Icy Touch
        glyphArray[1] = 43535;  // Glyph of Blood Tap
        glyphArray[2] = 43549;  // Glyph of the Ghoul
        glyphArray[3] = 43672;  // Glyph of Pestilence
        glyphArray[4] = 43673;  // Glyph of Raise Dead
        glyphArray[5] = 43542;  // Glyph of Death and Decay
        break;
    case TALENT_TREE_SHAMAN_ELEMENTAL:
        glyphArray[0] = 41536;  // Glyph of Lightning Bolt
        glyphArray[1] = 43386;  // Glyph of Water Shield
        glyphArray[2] = 41532;  // Glyph of Flametongue Weapon
        glyphArray[3] = 43385;  // Glyph of Renewed Life
        glyphArray[4] = 44923;  // Glyph of Thunderstorm
        glyphArray[5] = 45776;  // Glyph of Totem of Wrath
        break;
    case TALENT_TREE_SHAMAN_ENHANCEMENT:
        glyphArray[0] = 41530;  // Glyph of Fire Nova
        glyphArray[1] = 43386;  // Glyph of Water Shield
        glyphArray[2] = 41532;  // Glyph of Flametongue Weapon
        glyphArray[3] = 43385;  // Glyph of Renewed Life
        glyphArray[4] = 43388;  // Glyph of Water Walking
        glyphArray[5] = 41539;  // Glyph of Stormstrike
        break;
    case TALENT_TREE_SHAMAN_RESTORATION:
        glyphArray[0] = level < 50 ?
            41533 :     // Glyph of Healing Stream Totem
            45775;      // Glyph of Earth Shield
        glyphArray[1] = 43386;  // Glyph of Water Shield
        glyphArray[2] = 41527;  // Glyph of Earthliving Weapon
        glyphArray[3] = 43385;  // Glyph of Renewed Life
        glyphArray[4] = 43388;  // Glyph of Water Walking
        glyphArray[5] = 41517;  // Glyph of Chain Heal
        break;
    case TALENT_TREE_MAGE_ARCANE:
        glyphArray[0] = 42735;  // Glyph of Arcane Nova
        glyphArray[1] = 43339;  // Glyph of Arcane Intellect
        glyphArray[2] = level < 62 ?
            42750 :     // Glyph of Mana Gem
            44955;      // Glyph of Arcane Blast
        glyphArray[3] = 43364;  // Glyph of Slow Fall
        glyphArray[4] = 43361;  // Glyph of the Penguin
        glyphArray[5] = 42751;  // Glyph of Molten Armor
        break;
    case TALENT_TREE_MAGE_FIRE:
        glyphArray[0] = 42739;  // Glyph of Fireball
        glyphArray[1] = 43339;  // Glyph of Arcane Intellect
        glyphArray[2] = level < 60 ?
            42750 :     // Glyph of Mana Gem
            45737;      // Glyph of Living Bomb
        glyphArray[3] = 43364;  // Glyph of Slow Fall
        glyphArray[4] = 44920;  // Glyph of Blast Wave
        glyphArray[5] = 42751;  // Glyph of Molten Armor
        break;
    case TALENT_TREE_MAGE_FROST:
        glyphArray[0] = 42742;  // Glyph of Frostbolt
        glyphArray[1] = 43339;  // Glyph of Arcane Intellect
        glyphArray[2] = level < 50 ?
            42750 :     // Glyph of Mana Gem
            50045;      // Glyph of Eternal Water
        glyphArray[3] = 43364;  // Glyph of Slow Fall
        glyphArray[4] = 44920;  // Glyph of Blast Wave
        glyphArray[5] = 42751;  // Glyph of Molten Armor
        break;
    case TALENT_TREE_WARLOCK_AFFLICTION:
        glyphArray[0] = 45785;  // Glyph of Life Tap
        glyphArray[1] = 43390;  // Glyph of Soul Shard
        glyphArray[2] = 50077;  // Glyph of Quick Decay
        glyphArray[3] = 43389;  // Glyph of Unending Breath
        glyphArray[4] = 43394;  // Glyph of Souls
        glyphArray[5] = 45779;  // Glyph of Haunt
        break;
    case TALENT_TREE_WARLOCK_DEMONOLOGY:
        glyphArray[0] = 45785;  // Glyph of Life Tap
        glyphArray[1] = 43390;  // Glyph of Soul Shard
        glyphArray[2] = 50077;  // Glyph of Quick Decay
        glyphArray[3] = 43389;  // Glyph of Unending Breath
        glyphArray[4] = 43394;  // Glyph of Souls
        glyphArray[5] = 42459;  // Glyph of Felguard
        break;
    case TALENT_TREE_WARLOCK_DESTRUCTION:
        glyphArray[0] = 42464;  // Glyph of Immolate
        glyphArray[1] = 43390;  // Glyph of Soul Shard
        glyphArray[2] = 57257;  // Glyph of Incinerate
        glyphArray[3] = 43389;  // Glyph of Unending Breath
        glyphArray[4] = 43394;  // Glyph of Souls
        glyphArray[5] = 42454;  // Glyph of Conflagrate
        break;
    case TALENT_TREE_DRUID_BALANCE:
        glyphArray[0] = 40923;  // Glyph of Moonfire
        glyphArray[1] = 43335;  // Glyph of the Wild
        glyphArray[2] = 40916;  // Glyph of Starfire
        glyphArray[3] = 43331;  // Glyph of Unburdened Rebirth
        glyphArray[4] = 43674;  // Glyph of Dash
        glyphArray[5] = 40921;  // Glyph of Starfall
        break;
    case TALENT_TREE_DRUID_FERAL_COMBAT:
        if (specInfo->fforiginal == DRUID_FERAL_TANK) {
            glyphArray[0] = 40897;  // Glyph of Maul
            glyphArray[1] = 43332;  // Glyph of Thorns
            glyphArray[2] = 46372;  // Glyph of Survival Instincts
            glyphArray[3] = 43334;  // Glyph of Challenging Roar
            glyphArray[4] = 43331;  // Glyph of Unburdened Rebirth
            glyphArray[5] = 40900;  // Glyph of Mangle
        }
        else {
            glyphArray[0] = 40902;  // Glyph of Rip
            glyphArray[1] = 43335;  // Glyph of the Wild
            glyphArray[2] = 40901;  // Glyph of Shred
            glyphArray[3] = 43331;  // Glyph of Unburdened Rebirth
            glyphArray[4] = 43674;  // Glyph of Dash
            glyphArray[5] = 45604;  // Glyph of Savage Roar
        }
        break;
    case TALENT_TREE_DRUID_RESTORATION:
        glyphArray[0] = level < 40 ?
            40913 :     // Glyph of Rejuvenation
            40906;      // Glyph of Swiftmend
        glyphArray[1] = 43335;  // Glyph of the Wild
        glyphArray[2] = level < 60 ?
            50125 :     // Glyph of Rapid Rejuvenation
            45602;      // Glyph of Wild Growth
        glyphArray[3] = 43331;  // Glyph of Unburdened Rebirth
        glyphArray[4] = 43674;  // Glyph of Dash
        glyphArray[5] = 64313;  // Glyph of Nourish
        break;
    default:
        break;
    }
    return glyphArray;
}

uint32 FFItemMgr::GetHealthPotion()
{
    return level > 70 ? 33447 :
        level > 55 ? 43531 :
        level > 45 ? 13446 :
        level > 35 ? 3928 :
        level > 20 ? 1710 :
        858;       
}

uint32 FFItemMgr::GetManaPotion()
{
    if (specInfo->ffclass == CLASS_DEATH_KNIGHT ||
        specInfo->ffclass == CLASS_WARRIOR ||
        specInfo->ffclass == CLASS_ROGUE) {
        return 0;
    }

    return level > 70 ? 33448 :
        level > 55 ? 43530 :
        level > 48 ? 13444 :
        level > 40 ? 13443 :
        level > 21 ? 3827 :
        3385;
}

uint32 FFItemMgr::GetFood()
{
    if (level < 70) {
        return 44836;       // Pumpkin pie
    }

    uint32 foodItem = 0;
    switch (specInfo->ffspec)
    {
    case TALENT_TREE_WARRIOR_ARMS:
    case TALENT_TREE_WARRIOR_FURY:
    case TALENT_TREE_PALADIN_RETRIBUTION:
    case TALENT_TREE_DEATH_KNIGHT_FROST:
    case TALENT_TREE_DEATH_KNIGHT_UNHOLY:
        foodItem = 43000;   // Dragonfin Filet
        break;
    case TALENT_TREE_WARRIOR_PROTECTION:
    case TALENT_TREE_DEATH_KNIGHT_BLOOD:
        foodItem = 42994;   // Rhinolicious Wormsteak
        break;
    case TALENT_TREE_PALADIN_PROTECTION:
    case TALENT_TREE_HUNTER_MARKSMANSHIP:
    case TALENT_TREE_HUNTER_SURVIVAL:
        foodItem = 42999;   // Blackened Dragonfin
        break;
    case TALENT_TREE_ROGUE_ASSASSINATION:
    case TALENT_TREE_ROGUE_COMBAT:
    case TALENT_TREE_ROGUE_SUBTLETY:
        foodItem = 42996;   // Snapper Extreme
        break;
    case TALENT_TREE_HUNTER_BEAST_MASTERY:
        foodItem = 34766;   // Poached Northern Sculpin
        break;
    case TALENT_TREE_PALADIN_HOLY:
        foodItem = 34769;   // Imperial Manta Steak
        break;
    case TALENT_TREE_PRIEST_DISCIPLINE:
    case TALENT_TREE_PRIEST_HOLY:
    case TALENT_TREE_PRIEST_SHADOW:
    case TALENT_TREE_SHAMAN_ELEMENTAL:
    case TALENT_TREE_SHAMAN_RESTORATION:
    case TALENT_TREE_DRUID_RESTORATION:
    case TALENT_TREE_MAGE_ARCANE:
    case TALENT_TREE_MAGE_FIRE:
    case TALENT_TREE_MAGE_FROST:
    case TALENT_TREE_WARLOCK_AFFLICTION:
    case TALENT_TREE_WARLOCK_DEMONOLOGY:
    case TALENT_TREE_WARLOCK_DESTRUCTION:
    case TALENT_TREE_DRUID_BALANCE:
        foodItem = 34767;   // Firecracker Salmon
        break;
    case TALENT_TREE_SHAMAN_ENHANCEMENT:
        foodItem = 43268;   // Dalaran Clam Chowder
        break;
    case TALENT_TREE_DRUID_FERAL_COMBAT:
        if (specInfo->fforiginal == DRUID_FERAL_TANK) {
            foodItem = 42994;   // Rhinolicious Wormsteak
        }
        else {
            foodItem = 43000;   // Dragonfin Filet
        }
        break;
    default:
        break;
    }

    return foodItem;
}

FFItemScorer::FFItemScorer(TalentTree ffspec, uint8 pSpec)
{
    // default stat priority
    priorityStat1 = ITEM_MOD_EXPERTISE_RATING;
    priorityStat2 = ITEM_MOD_HIT_RATING;
    priorityStat3 = ITEM_MOD_HASTE_RATING;
    priorityStat4 = ITEM_MOD_STAMINA;
    priorityStat5 = ITEM_MOD_MANA_REGENERATION;

    switch (ffspec)
    {
    case TALENT_TREE_WARRIOR_ARMS:
    case TALENT_TREE_WARRIOR_FURY:
        priorityStat1 = ITEM_MOD_HIT_RATING;
        priorityStat2 = ITEM_MOD_ARMOR_PENETRATION_RATING;
        priorityStat3 = ITEM_MOD_STRENGTH;
        priorityStat4 = ITEM_MOD_CRIT_RATING;
        priorityStat5 = ITEM_MOD_HASTE_MELEE_RATING;
        break;
    case TALENT_TREE_WARRIOR_PROTECTION:
        priorityStat1 = ITEM_MOD_DEFENSE_SKILL_RATING;
        priorityStat2 = ITEM_MOD_STAMINA;
        priorityStat3 = ITEM_MOD_HIT_MELEE_RATING;
        priorityStat4 = ITEM_MOD_BLOCK_RATING;
        priorityStat5 = ITEM_MOD_STRENGTH;
        break;
    case TALENT_TREE_PALADIN_HOLY:
        priorityStat1 = ITEM_MOD_INTELLECT;
        priorityStat2 = ITEM_MOD_HASTE_SPELL_RATING;
        priorityStat3 = ITEM_MOD_MANA_REGENERATION;
        priorityStat4 = ITEM_MOD_CRIT_SPELL_RATING;
        priorityStat5 = ITEM_MOD_STAMINA;
        break;
    case TALENT_TREE_PALADIN_PROTECTION:
        priorityStat1 = ITEM_MOD_STAMINA;
        priorityStat2 = ITEM_MOD_BLOCK_RATING;
        priorityStat3 = ITEM_MOD_HIT_RATING;
        priorityStat4 = ITEM_MOD_DODGE_RATING;
        priorityStat5 = ITEM_MOD_PARRY_RATING;
        break;
    case TALENT_TREE_PALADIN_RETRIBUTION:
        priorityStat1 = ITEM_MOD_HIT_RATING;
        priorityStat2 = ITEM_MOD_STRENGTH;
        priorityStat3 = ITEM_MOD_AGILITY;
        priorityStat4 = ITEM_MOD_ATTACK_POWER;
        priorityStat5 = ITEM_MOD_CRIT_MELEE_RATING;
        break;
    case TALENT_TREE_HUNTER_BEAST_MASTERY:
        priorityStat1 = ITEM_MOD_HIT_RATING;
        priorityStat2 = ITEM_MOD_ATTACK_POWER;
        priorityStat3 = ITEM_MOD_AGILITY;
        priorityStat4 = ITEM_MOD_CRIT_RANGED_RATING;
        priorityStat5 = ITEM_MOD_ARMOR_PENETRATION_RATING;
        break;
    case TALENT_TREE_HUNTER_MARKSMANSHIP:
        priorityStat1 = ITEM_MOD_HIT_RATING;
        priorityStat2 = ITEM_MOD_AGILITY;
        priorityStat3 = ITEM_MOD_ARMOR_PENETRATION_RATING;
        priorityStat4 = ITEM_MOD_CRIT_RANGED_RATING;
        priorityStat5 = ITEM_MOD_ATTACK_POWER;
        break;
    case TALENT_TREE_HUNTER_SURVIVAL:
        priorityStat1 = ITEM_MOD_HIT_RATING;
        priorityStat2 = ITEM_MOD_AGILITY;
        priorityStat3 = ITEM_MOD_CRIT_RANGED_RATING;
        priorityStat4 = ITEM_MOD_ATTACK_POWER;
        priorityStat5 = ITEM_MOD_ARMOR_PENETRATION_RATING;
        break;
    case TALENT_TREE_ROGUE_ASSASSINATION:
    case TALENT_TREE_ROGUE_COMBAT:
    case TALENT_TREE_ROGUE_SUBTLETY:
        priorityStat1 = ITEM_MOD_HIT_RATING;
        priorityStat2 = ITEM_MOD_AGILITY;
        priorityStat3 = ITEM_MOD_STRENGTH;
        priorityStat4 = ITEM_MOD_ATTACK_POWER;
        priorityStat5 = ITEM_MOD_CRIT_RATING;
        break;
    case TALENT_TREE_PRIEST_DISCIPLINE:
        priorityStat1 = ITEM_MOD_HASTE_RATING;
        priorityStat2 = ITEM_MOD_SPELL_POWER;
        priorityStat3 = ITEM_MOD_CRIT_RATING;
        priorityStat4 = ITEM_MOD_INTELLECT;
        priorityStat5 = ITEM_MOD_SPIRIT;
        break;
    case TALENT_TREE_PRIEST_HOLY:
        priorityStat1 = ITEM_MOD_HASTE_RATING;
        priorityStat2 = ITEM_MOD_INTELLECT;
        priorityStat3 = ITEM_MOD_SPELL_POWER;
        priorityStat4 = ITEM_MOD_SPIRIT;
        priorityStat5 = ITEM_MOD_CRIT_RATING;
        break;
    case TALENT_TREE_PRIEST_SHADOW:
        priorityStat1 = ITEM_MOD_HIT_SPELL_RATING;
        priorityStat2 = ITEM_MOD_SPELL_POWER;
        priorityStat3 = ITEM_MOD_HASTE_RATING;
        priorityStat4 = ITEM_MOD_CRIT_RATING;
        priorityStat5 = ITEM_MOD_SPIRIT;
        break;
    case TALENT_TREE_DEATH_KNIGHT_BLOOD:
        priorityStat1 = ITEM_MOD_HIT_RATING;
        priorityStat2 = ITEM_MOD_STAMINA;
        priorityStat3 = ITEM_MOD_DODGE_RATING;
        priorityStat4 = ITEM_MOD_PARRY_RATING;
        priorityStat5 = ITEM_MOD_STRENGTH;
        break;
    case TALENT_TREE_DEATH_KNIGHT_FROST:
        priorityStat1 = ITEM_MOD_HIT_RATING;
        priorityStat2 = ITEM_MOD_STRENGTH;
        priorityStat3 = ITEM_MOD_ARMOR_PENETRATION_RATING;
        priorityStat4 = ITEM_MOD_CRIT_RATING;
        priorityStat5 = ITEM_MOD_HASTE_RATING;
        break;
    case TALENT_TREE_DEATH_KNIGHT_UNHOLY:
        priorityStat1 = ITEM_MOD_HIT_RATING;
        priorityStat2 = ITEM_MOD_STRENGTH;
        priorityStat3 = ITEM_MOD_HASTE_RATING;
        priorityStat4 = ITEM_MOD_ARMOR_PENETRATION_RATING;
        priorityStat5 = ITEM_MOD_CRIT_RATING;
        break;
    case TALENT_TREE_SHAMAN_ELEMENTAL:
        priorityStat1 = ITEM_MOD_SPELL_POWER;
        priorityStat2 = ITEM_MOD_HASTE_RATING;
        priorityStat3 = ITEM_MOD_CRIT_RATING;
        priorityStat4 = ITEM_MOD_INTELLECT;
        priorityStat5 = ITEM_MOD_MANA_REGENERATION;
        break;
    case TALENT_TREE_SHAMAN_ENHANCEMENT:
        priorityStat1 = ITEM_MOD_HIT_RATING;
        priorityStat2 = ITEM_MOD_ATTACK_POWER;
        priorityStat3 = ITEM_MOD_HASTE_RATING;
        priorityStat4 = ITEM_MOD_AGILITY;
        priorityStat5 = ITEM_MOD_INTELLECT;
        break;
    case TALENT_TREE_SHAMAN_RESTORATION:
        priorityStat1 = ITEM_MOD_SPELL_POWER;
        priorityStat2 = ITEM_MOD_HASTE_RATING;
        priorityStat3 = ITEM_MOD_MANA_REGENERATION;
        priorityStat4 = ITEM_MOD_CRIT_RATING;
        priorityStat5 = ITEM_MOD_INTELLECT;
        break;
    case TALENT_TREE_MAGE_ARCANE:
    case TALENT_TREE_MAGE_FIRE:
    case TALENT_TREE_MAGE_FROST:
    case TALENT_TREE_WARLOCK_AFFLICTION:
    case TALENT_TREE_WARLOCK_DEMONOLOGY:
    case TALENT_TREE_WARLOCK_DESTRUCTION:
    case TALENT_TREE_DRUID_BALANCE:
        priorityStat1 = ITEM_MOD_HIT_SPELL_RATING;
        priorityStat2 = ITEM_MOD_SPELL_POWER;
        priorityStat3 = ITEM_MOD_CRIT_RATING;
        priorityStat4 = ITEM_MOD_SPIRIT;
        priorityStat5 = ITEM_MOD_INTELLECT;
        break;
    case TALENT_TREE_DRUID_FERAL_COMBAT:
        if (pSpec == DRUID_FERAL_TANK) {
            priorityStat1 = ITEM_MOD_STAMINA;
            priorityStat2 = ITEM_MOD_AGILITY;
            priorityStat3 = ITEM_MOD_HIT_RATING;
            priorityStat4 = ITEM_MOD_STRENGTH;
            priorityStat5 = ITEM_MOD_HASTE_RATING;
        }
        else {
            priorityStat1 = ITEM_MOD_AGILITY;
            priorityStat2 = ITEM_MOD_ARMOR_PENETRATION_RATING;
            priorityStat3 = ITEM_MOD_CRIT_RATING;
            priorityStat4 = ITEM_MOD_STRENGTH;
            priorityStat5 = ITEM_MOD_HASTE_RATING;
        }
        break;
    case TALENT_TREE_DRUID_RESTORATION:
        priorityStat1 = ITEM_MOD_SPELL_POWER;
        priorityStat2 = ITEM_MOD_HASTE_RATING;
        priorityStat3 = ITEM_MOD_MANA_REGENERATION;
        priorityStat4 = ITEM_MOD_INTELLECT;
        priorityStat5 = ITEM_MOD_SPIRIT;
        break;
    default:
        break;
    }
}

double FFItemScorer::GetScoreForItemStats(uint16 itemLevel, std::vector<uint8> statArray, std::vector<std::int32_t> statValArray, float dmgMin1, float dmgMax1, float dmgMin2, float dmgMax2, uint32 armor, uint16 delay)
{
    double score = 0.0f;
    double dps = (delay > 0) ? std::ceil((((((static_cast<double>(dmgMin1) + dmgMax1) / 2.0f) + ((dmgMin2 + dmgMax2) / 2.0f)) * 1000.0) / delay) * 100.0) / 100.0 : 0;
    score += itemLevel + dps + armor;

    for (uint8 i = 0; i < statArray.size(); i++)
    {
        if (statArray[i] == priorityStat1) { score += 2.5 * statValArray[i]; }
        else if (statArray[i] == priorityStat2) { score += 2.2 * statValArray[i]; }
        else if (statArray[i] == priorityStat3) { score += 2.0 * statValArray[i]; }
        else if (statArray[i] == priorityStat4) { score += 1.8 * statValArray[i]; }
        else if (statArray[i] == priorityStat5) { score += 1.5 * statValArray[i]; }
        else { score += 0.5 * statValArray[i]; }
    }

    return score;
}
