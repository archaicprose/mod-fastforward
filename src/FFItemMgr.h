/*
 * Compares and selects item for character generation.
 */

#ifndef FF_ITEM_MGR_H
#define FF_ITEM_MGR_H

#include "ScriptMgr.h"
#include "Player.h"
#include "ItemTemplate.h"
#include "FFShared.h"


class FFItemScorer {
private:
	uint8 priorityStat1;
	uint8 priorityStat2;
	uint8 priorityStat3;
	uint8 priorityStat4;
	uint8 priorityStat5;

public:
	FFItemScorer(TalentTree ffspec, uint8 pSpec);
	~FFItemScorer() {

	}
	double GetScoreForItemStats(uint16 itemLevel, std::vector<uint8> statArray, std::vector<std::int32_t> statValArray, float dmgMin1, float dmgMax1, float dmgMin2, float dmgMax2, uint32 armor, uint16 delay);
};

class FFItemMgr
{
private: 
	FFSpec* specInfo;
	FFItemScorer* itemScorer;
	uint8 level;
	uint8 race;

	uint32 GetArmorForInventoryType(std::string inventoryTypesStr);
    uint32 GetWeaponForInventoryType(std::string subclassStr, std::string inventoryTypesStr);
    uint32 GetOffhandWeaponForInventoryType(std::string subclassStr, std::string inventoryTypesStr, uint32 primaryWeapon);
    uint32 GetMiscForInventoryType(std::string inventoryTypesStr);
	uint32 GetItemForInventoryType(std::string classStr, std::string subclassStr, std::string inventoryTypesStr);
    uint32 GetSecondaryItemForInventoryType(std::string classStr, std::string subclassStr, std::string inventoryTypesStr, uint32 primaryWeapon);
    uint32 GetBestItemFromResults(QueryResult result);
public:
	FFItemMgr(uint8 pLevel, uint8 pRace, FFSpec* spec) {
		specInfo = spec;
		itemScorer = new FFItemScorer(spec->ffspec, spec->fforiginal);
		race = pRace;
		level = pLevel;
	}
	~FFItemMgr() {
	}

    Classes GetIntendedClass();

	uint32 GetHeadItem();
    uint32 GetNeckItem();
    uint32 GetShoulderItem();
    uint32 GetChestItem();
    uint32 GetWaistItem();
    uint32 GetLegsItem();
    uint32 GetFeetItem();
    uint32 GetWristsItem();
    uint32 GetHandsItem();
    uint32 GetFingerItem1();
    uint32 GetFingerItem2(uint32 primaryFinger);
    uint32 GetTrinket1();
    uint32 GetTrinket2(uint32 primaryTrinket);
    uint32 GetBackItem();
    uint32 GetMainHandWeapon();
    uint32 GetSecondaryItem(uint32 primaryWeapon);
    uint32 GetRangeWeapon();
    uint32* GetGlyphs();
    uint32 GetHealthPotion();
    uint32 GetManaPotion();
    uint32 GetFood();
};
#endif
