
#include "FastForward.h"

FastForward* FastForward::instance()
{
    static FastForward instance;
    return &instance;
}

bool FastForward::IsLevelAllowed(int level) const
{
    int upperBound = std::min((uint8)80, maxAllowableLevel);
    int lowerBound = std::max((uint8)10, minAllowableLevel);

    return lowerBound <= level && level <= upperBound;
}

class FF_WorldScript : public WorldScript
{
public:
    FF_WorldScript() : WorldScript("FF_WorldScript") { }

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        LoadFFConfig();
    }

private:
    static void LoadFFConfig() {
        sFastForward->fastforwardEnabled = sConfigMgr->GetOption<bool>("FastForward.Enable", false);
        if (sFastForward->Enabled()) {
            sFastForward->maxAllowableLevel = sConfigMgr->GetOption<uint8>("FastForward.MaxAllowableLevel", 80);
            sFastForward->minAllowableLevel = sConfigMgr->GetOption<uint8>("FastForward.MinAllowableLevel", 10);
        }
    }
};

class FF_Character_Prep : public PlayerScript
{
public:
    FF_Character_Prep() : PlayerScript("FF_Character_Prep") { }

    void OnPlayerCreate(Player *p) override
    {
        if (!sFastForward->Enabled()) {
            return;
        }

        uint32 accountId = p->GetSession()->GetAccountId();
        QueryResult result = CharacterDatabase.Query("SELECT `spec`,`level` FROM `ff_character_prep` WHERE `claimed` = 0 AND `created_on` >= NOW() - INTERVAL 1 HOUR AND `account_id` = '{}';", accountId);
        if (result)
        {
            Field* fields = result->Fetch();
            uint8 targetSpec = fields[0].Get<uint8>();
            uint8 targetLevel = fields[1].Get<uint8>();
            TeamId pTeam = p->GetTeamId();
            FFItemMgr* itemMgr = new FFItemMgr(targetLevel, p->getRace(), targetSpec);

            // Is this preparation valid for this new character?
            if (itemMgr->GetIntendedClass() != p->getClass()) {
                return;
            }

            MailDraft welcomeMail = MailDraft("Fast Forward", "You blacked out at the inn last night, ranting on about thinking you were back at the beginning again. You foolishly left all your belongings laying about the place, but I packed them into your bags when you were asleep.");

            // =======|   LOCATION  |=======
            WorldLocation dalLoc = WorldLocation(571, 5841.65f, 642.086f, 647.512f, 3.83756f); // Dalaran
            //p->TeleportTo(571, 5814.0547f, 448.982f, 658.7519f, 0.0f); // Krasus' Landing
            p->SetHomebind(dalLoc, 4395);

            // =======|   LEVEL     |=======
            p->GiveLevel(targetLevel);

            // =======|   SKILLS    |=======

            // Teach Riding
            if (targetLevel >= 20) {
                p->learnSpell(33388);
            }
            if (targetLevel >= 40) {
                p->learnSpell(33391);
            }
            if (targetLevel >= 60) {
                p->learnSpell(34090);
            }
            if (targetLevel >= 70) {
                p->learnSpell(34091);
            }
            if (targetLevel >= 77) {
                p->learnSpell(54197);
            }

            // Dual Spec
            if (targetLevel >= 40) {
                p->CastSpell(p, 63624, p->GetGUID());   // Second Talent Specialization
                p->CastSpell(p, 63680, p->GetGUID());   // Talent Switching
            }

            // Weapons & Armor
            LearnWeaponSkills(p);
            LearnPlateMailSpells(p, targetLevel);

            p->UpdateSkillsToMaxSkillsForLevel();

            // =======|   ITEMS     |=======
            uint32 bagType = (targetLevel < 60) ? 10050 :   // Mageweave
                (targetLevel < 70) ? 21841 :                // Netherweave
                41599;                                      // Frostweave
            if (p->getClass() == CLASS_DEATH_KNIGHT) {
                // DKs start with bags equipped. Rather than deal with bag-space issues, just add new bags to inventory
                p->AddItem(bagType, 5);
            }
            else {
                for (uint8 pos = INVENTORY_SLOT_BAG_START; pos < INVENTORY_SLOT_BAG_END; ++pos)
                {
                    p->EquipNewItem(pos, bagType, false);
                }
            }

            uint32 slowMountItemsA[5] = { 2414, 5872, 13321, 8631, 29743 };
            uint32 slowMountItemsH[5] = { 46099, 46100, 46308, 8592, 29221 };
            uint32 fastMountItemsA[5] = { 18776, 18786, 18774, 18766, 29745 };
            uint32 fastMountItemsH[5] = { 18796, 18795, 13334, 18790, 29224 };
            uint32 slowFlyMountItemsA[2] = { 25470, 25471 };
            uint32 slowFlyMountItemsH[2] = { 25474, 25475 };
            uint32 fastFlyMountItemsA[3] = { 25529, 25528, 25527 };
            uint32 fastFlyMountItemsH[3] = { 25531, 25532, 25533 };

            if (targetLevel >= 20 && targetLevel < 40) {
                p->AddItem(pTeam == TEAM_ALLIANCE ? slowMountItemsA[rand() % 5] : slowMountItemsH[rand() % 5], 1);
            }
            else if (targetLevel >= 40) {
                p->AddItem(pTeam == TEAM_ALLIANCE ? fastMountItemsA[rand() % 5] : fastMountItemsH[rand() % 5], 1);
            }
            if (targetLevel >= 60 && targetLevel < 70) {
                p->AddItem(pTeam == TEAM_ALLIANCE ? slowFlyMountItemsA[rand() % 2] : slowFlyMountItemsH[rand() % 2], 1);
            }
            else if (targetLevel >= 70) {
                p->AddItem(pTeam == TEAM_ALLIANCE ? fastFlyMountItemsA[rand() % 3] : fastFlyMountItemsH[rand() % 3], 1);
            }

            // Equipped items
            if (targetLevel >= 10) {
                // Remove starting items
                TryUnequipItem(p, EQUIPMENT_SLOT_CHEST);
                TryUnequipItem(p, EQUIPMENT_SLOT_LEGS);
                TryUnequipItem(p, EQUIPMENT_SLOT_FEET);
                TryUnequipItem(p, EQUIPMENT_SLOT_MAINHAND);

                // Add new items
                uint32 headItem = itemMgr->GetHeadItem();
                if (headItem != 0) { p->StoreNewItemInBestSlots(headItem, 1); }
                uint32 neckItem = itemMgr->GetNeckItem();
                if (neckItem != 0) { p->StoreNewItemInBestSlots(neckItem, 1); }
                uint32 shoulderItem = itemMgr->GetShoulderItem();
                if (shoulderItem != 0) { p->StoreNewItemInBestSlots(shoulderItem, 1); }
                uint32 chestItem = itemMgr->GetChestItem();
                if (chestItem != 0) { p->StoreNewItemInBestSlots(chestItem, 1); }
                uint32 waistItem = itemMgr->GetWaistItem();
                if (waistItem != 0) { p->StoreNewItemInBestSlots(waistItem, 1); }
                uint32 legsItem = itemMgr->GetLegsItem();
                if (legsItem != 0) { p->StoreNewItemInBestSlots(legsItem, 1); }
                uint32 feetItem = itemMgr->GetFeetItem();
                if (feetItem != 0) { p->StoreNewItemInBestSlots(feetItem, 1); }
                uint32 wristItem = itemMgr->GetWristsItem();
                if (wristItem != 0) { p->StoreNewItemInBestSlots(wristItem, 1); }
                uint32 handsItem = itemMgr->GetHandsItem();
                if (handsItem != 0) { p->StoreNewItemInBestSlots(handsItem, 1); }
                uint32 backItem = itemMgr->GetBackItem();
                if (backItem != 0) { p->StoreNewItemInBestSlots(backItem, 1); }
                uint32 ringItem1 = itemMgr->GetFingerItem1();
                if (ringItem1 != 0) { p->StoreNewItemInBestSlots(ringItem1, 1); }
                uint32 ringItem2 = itemMgr->GetFingerItem2(ringItem1);
                if (ringItem2 != 0) { p->StoreNewItemInBestSlots(ringItem2, 1); }
                uint32 trinket1 = itemMgr->GetTrinket1();
                if (trinket1 != 0) { p->StoreNewItemInBestSlots(trinket1, 1); }
                uint32 trinket2 = itemMgr->GetTrinket2(trinket1);
                if (trinket2 != 0) { p->StoreNewItemInBestSlots(trinket2, 1); }

                uint32 mainHand = itemMgr->GetMainHandWeapon();
                if (mainHand != 0) { p->StoreNewItemInBestSlots(mainHand, 1); }
                uint32 offHand = itemMgr->GetSecondaryItem(mainHand);
                if (offHand != 0) { p->StoreNewItemInBestSlots(offHand, 1); }
                uint32 ranged = itemMgr->GetRangeWeapon();
                if (ranged != 0) { p->StoreNewItemInBestSlots(ranged, 1); }

                uint32* glyphs = itemMgr->GetGlyphs();
                for (uint8 i = 0; i < 6; i++)
                {
                    uint32 glyphVal = glyphs[i];
                    if (glyphVal != 0) { p->AddItem(glyphVal, 1); }
                }

                uint32 hPot = itemMgr->GetHealthPotion();
                if (hPot != 0) { p->AddItem(hPot, 5); }
                uint32 mPot = itemMgr->GetManaPotion();
                if (mPot != 0) { p->AddItem(mPot, 5); }
                uint32 food = itemMgr->GetFood();
                if (food != 0) { p->AddItem(food, 10); }
            }

            // =======|   GOLD      |=======
            uint32 characterMoney = DetermineCharacterMoney(p, targetLevel);
            welcomeMail.AddMoney(characterMoney);

            // =======|   ABILITIES |=======
            HandleClassFeatures(p, targetLevel);

            // =======|   TAXI      |=======
            HandleTaxi(p, targetLevel);

            // =======|   REP       |=======
            float reveredReputation = 21000.0f;
            std::vector<uint32> factions = {};
            if (pTeam == TEAM_ALLIANCE) {
                factions.push_back(47);  // Ironforge
                factions.push_back(54);  // Gnomeregan Exiles
                factions.push_back(69);  // Darnassus
                factions.push_back(72);  // Stormwind
                factions.push_back(930); // Exodar
            }
            else if (pTeam == TEAM_HORDE) {
                factions.push_back(76);  // Orgrimmar
                factions.push_back(68);  // Undercity
                factions.push_back(81);  // Thunder Bluff
                factions.push_back(530); // Darkspear Trolls
                factions.push_back(911); // Silvermoon City
            }

            if (targetLevel >= 68) {
                // This may be more than necessary, but there seems to be a discrepancy between base faction values
                float unfriendlyBonus = 4200.0f;
                if (pTeam == TEAM_ALLIANCE) {
                    // Kurenai
                    p->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(978), reveredReputation + unfriendlyBonus, true);
                }
                else {
                    // The Mag'har
                    p->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(941), reveredReputation + unfriendlyBonus, true);
                }
                factions.push_back(942);  // Cenarion Expedition
                factions.push_back(1031); // Sha'tari Skyguard
                factions.push_back(933);  // The Consortium
                factions.push_back(1011); // Lower City
                factions.push_back(935);  // The Sha'tar
                factions.push_back(989);  // Keepers of Time
            }
            if (targetLevel >= 78) {
                factions.push_back(1104); // Frenzyheart Tribe
                factions.push_back(1105); // The Oracles
                factions.push_back(1073); // The Kalu'lak
                //factions.push_back(1119); // Sons of Hodir
                factions.push_back(1098); // Knights of the Ebon Blade
                factions.push_back(1156); // The Ashen Verdict
                factions.push_back(1106); // The Argent Crusade
                factions.push_back(1091); // The Wyrmrest Accord
                factions.push_back(1090); // The Kirin Tor
            }
            for (uint32 factionId : factions)
            {
                p->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(factionId), reveredReputation, true);
            }

            // =======|   SAVE     |=======
            CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
            p->SaveToDB(trans, false, false);
            welcomeMail.SendMailTo(trans, p, MailSender(MAIL_CREATURE, 34337, MAIL_STATIONERY_CHR), MAIL_CHECK_MASK_COPIED);

            CharacterDatabase.CommitTransaction(trans);

            // =======|   CLAIM     |=======
            CharacterDatabase.Execute("UPDATE acore_characters.ff_character_prep SET claimed = 1 WHERE account_id = '{}' AND level = '{}';", accountId, targetLevel);
        }
    }

private:
    static void TryUnequipItem(Player* player, uint8 slot) {
        Item* unequipItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if (!unequipItem) {
            return;
        }

        ItemPosCountVec unequipDestination;
        uint8 unequipMsg = player->CanStoreItem(NULL_BAG, NULL_SLOT, unequipDestination, unequipItem, false);
        if (unequipMsg == EQUIP_ERR_OK) {
            player->RemoveItem(INVENTORY_SLOT_BAG_0, slot, true);
            player->StoreItem(unequipDestination, unequipItem, true);
        }
        else {
            player->MoveItemFromInventory(INVENTORY_SLOT_BAG_0, slot, true);
            CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
            unequipItem->DeleteFromInventoryDB(trans);
            unequipItem->SaveToDB(trans);

            std::string subject = player->GetSession()->GetAcoreString(LANG_NOT_EQUIPPED_ITEM);
            MailDraft(subject, "There were problems with equipping one or several items").AddItem(unequipItem).SendMailTo(trans, player, MailSender(player, MAIL_STATIONERY_GM), MAIL_CHECK_MASK_COPIED);

            CharacterDatabase.CommitTransaction(trans);
        }
    }

    static uint32 DetermineCharacterMoney(Player* player, uint8 targetLevel) {

        // quartic function that follows accumulated skill costs at a level
        double calculatedMoneyFraction = 0.314851 * std::pow(targetLevel, 4) - 20.6574 * std::pow(targetLevel, 3) + 1081.84 * std::pow(targetLevel, 2) - 17149.4 * targetLevel + 83223.5;
        uint32 characterMoney = (uint32)calculatedMoneyFraction;

        switch (player->getClass())
        {
        case CLASS_WARRIOR:
        case CLASS_PALADIN:
        case CLASS_DEATH_KNIGHT:
            // plate+mail repairs
            characterMoney += targetLevel * 12500;
            break;
        case CLASS_HUNTER:
        case CLASS_SHAMAN:
            // leather+mail repairs
            characterMoney += targetLevel * 7500;
            break;
        case CLASS_DRUID:
            // additional gold for form abilities & leather repairs
            characterMoney += targetLevel * 5000;
            break;
        case CLASS_ROGUE:
            // leather repairs
            characterMoney += targetLevel * 2500;
            break;
        default:
            characterMoney += targetLevel * 1250;
            break;
        }

        return characterMoney;
    }

    static void HandleClassFeatures(Player* player, uint8 targetLevel) {
        switch (player->getClass())
        {
        case CLASS_WARRIOR:
            HandleWarriorQuests(player, targetLevel);
            break;
        case CLASS_PALADIN:
            HandlePaladinQuests(player, targetLevel);
            break;
        case CLASS_HUNTER:
            HandleHunterQuests(player, targetLevel);
            HandleHunterPets(player, targetLevel);
            break;
        case CLASS_DEATH_KNIGHT:
            if (targetLevel >= 58) {
                HandleDKStartSkip(player);
            }
            break;
        case CLASS_SHAMAN:
            HandleShamanQuests(player, targetLevel);
            break;
        case CLASS_WARLOCK:
            HandleWarlockQuests(player, targetLevel);
            break;
        case CLASS_DRUID:
            HandleDruidQuests(player, targetLevel);
            break;
        default:
            break;
        }
    }

    enum WeaponProficiencies
    {
        BLOCK = 107,
        BOWS = 264,
        CROSSBOWS = 5011,
        DAGGERS = 1180,
        FIST_WEAPONS = 15590,
        GUNS = 266,
        ONE_H_AXES = 196,
        ONE_H_MACES = 198,
        ONE_H_SWORDS = 201,
        POLEARMS = 200,
        SHOOT = 5019,
        STAVES = 227,
        TWO_H_AXES = 197,
        TWO_H_MACES = 199,
        TWO_H_SWORDS = 202,
        WANDS = 5009,
        THROW_WAR = 2567
    };

    static void LearnWeaponSkills(Player* player) {
        switch (player->getClass())
        {
        case CLASS_WARRIOR:
            player->learnSpell(THROW_WAR);
            player->learnSpell(TWO_H_SWORDS);
            player->learnSpell(TWO_H_MACES);
            player->learnSpell(TWO_H_AXES);
            player->learnSpell(STAVES);
            player->learnSpell(POLEARMS);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(ONE_H_AXES);
            player->learnSpell(GUNS);
            player->learnSpell(FIST_WEAPONS);
            player->learnSpell(DAGGERS);
            player->learnSpell(CROSSBOWS);
            player->learnSpell(BOWS);
            player->learnSpell(BLOCK);
            break;
        case CLASS_PRIEST:
            player->learnSpell(WANDS);
            player->learnSpell(STAVES);
            player->learnSpell(SHOOT);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(DAGGERS);
            break;
        case CLASS_PALADIN:
            player->learnSpell(TWO_H_SWORDS);
            player->learnSpell(TWO_H_MACES);
            player->learnSpell(TWO_H_AXES);
            player->learnSpell(POLEARMS);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(ONE_H_AXES);
            player->learnSpell(BLOCK);
            break;
        case CLASS_ROGUE:
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(ONE_H_AXES);
            player->learnSpell(GUNS);
            player->learnSpell(FIST_WEAPONS);
            player->learnSpell(DAGGERS);
            player->learnSpell(CROSSBOWS);
            player->learnSpell(BOWS);
            break;
        case CLASS_DEATH_KNIGHT:
            player->learnSpell(TWO_H_SWORDS);
            player->learnSpell(TWO_H_MACES);
            player->learnSpell(TWO_H_AXES);
            player->learnSpell(POLEARMS);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(ONE_H_AXES);
            break;
        case CLASS_MAGE:
            player->learnSpell(WANDS);
            player->learnSpell(STAVES);
            player->learnSpell(SHOOT);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(DAGGERS);
            break;
        case CLASS_SHAMAN:
            player->learnSpell(TWO_H_MACES);
            player->learnSpell(TWO_H_AXES);
            player->learnSpell(STAVES);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(ONE_H_AXES);
            player->learnSpell(FIST_WEAPONS);
            player->learnSpell(DAGGERS);
            player->learnSpell(BLOCK);
            break;
        case CLASS_HUNTER:
            player->learnSpell(THROW_WAR);
            player->learnSpell(TWO_H_SWORDS);
            player->learnSpell(TWO_H_AXES);
            player->learnSpell(STAVES);
            player->learnSpell(POLEARMS);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(ONE_H_AXES);
            player->learnSpell(GUNS);
            player->learnSpell(FIST_WEAPONS);
            player->learnSpell(DAGGERS);
            player->learnSpell(CROSSBOWS);
            player->learnSpell(BOWS);
            break;
        case CLASS_DRUID:
            player->learnSpell(TWO_H_MACES);
            player->learnSpell(STAVES);
            player->learnSpell(POLEARMS);
            player->learnSpell(ONE_H_MACES);
            player->learnSpell(FIST_WEAPONS);
            player->learnSpell(DAGGERS);
            break;
        case CLASS_WARLOCK:
            player->learnSpell(WANDS);
            player->learnSpell(STAVES);
            player->learnSpell(SHOOT);
            player->learnSpell(ONE_H_SWORDS);
            player->learnSpell(DAGGERS);
            break;
        default:
            break;
        }
    }

    static void LearnPlateMailSpells(Player* player, uint8 targetLevel) {
        if (targetLevel >= 40) {
            switch (player->getClass())
            {
            case CLASS_WARRIOR:
            case CLASS_PALADIN:
            case CLASS_DEATH_KNIGHT:
                player->learnSpell(750); // Plate
                break;
            case CLASS_SHAMAN:
            case CLASS_HUNTER:
                player->learnSpell(8737); // Mail
                break;
            default:
                break;
            }
        }
    }

    static void HandleWarriorQuests(Player* p, uint8 level) {
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

    static void HandlePaladinQuests(Player* p, uint8 level) {
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

    static void HandleHunterQuests(Player* p, uint8 targetLevel) {
        if (targetLevel < 10) {
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

    static const uint32 TAME_BEAST_SPELL = 13481;
    //static const uint32 OWL_STORMPIKE_OWL = 14283;
    //static const uint32 DEVILSAUR_DEVILSAUR = 6498;
    static const uint32 BEAR_ASHENVALE_BEAR = 3810;
    //static const uint32 RAVAGER_BLOODMYST_HATCHLING = 17525;
    static const uint32 WOLF_COYOTE = 834;
    static const uint32 PET_MAX_HAPPINESS = 1048000;

    static void HandleHunterPets(Player* p, uint8 targetLevel) {

        if (targetLevel < 10 || p->IsExistPet()) {
            return;
        }

        /*
         * Need to investigate if there's a tricky way to fill the stable. Currently, it looks like
         * the way the methods are written limits the specific stable slots to Current, First, and Last.
         */

         // add bear
         /*Pet* bearPet = p->CreateTamedPetFrom(BEAR_ASHENVALE_BEAR, TAME_BEAST_SPELL);
         if (bearPet) {
             bearPet->InitTalentForLevel();
             bearPet->SavePetToDB(PET_SAVE_FIRST_STABLE_SLOT);
         }*/
         // add wolf
        /*Pet* wolfPet = p->CreatePet(WOLF_COYOTE, TAME_BEAST_SPELL);
        wolfPet->SetPower(POWER_HAPPINESS, PET_MAX_HAPPINESS);
        wolfPet->SetName("Fluffy");*/
    }

    static void HandleDKStartSkip(Player* player) {
        // The following is an abridged version of the "DK Start Skip" Mod
        player->AddItem(6948, true); //Hearthstone
        int STARTER_QUESTS[33] = { 12593, 12619, 12842, 12848, 12636, 12641, 12657, 12678, 12679, 12680, 12687, 12698, 12701, 12706, 12716, 12719, 12720, 12722, 12724, 12725, 12727, 12733, -1, 12751, 12754, 12755, 12756, 12757, 12779, 12801, 13165, 13166 };
        int specialSurpriseQuestId = -1;
        switch (player->getRace())
        {
        case RACE_TAUREN:
            specialSurpriseQuestId = 12739;
            break;
        case RACE_HUMAN:
            specialSurpriseQuestId = 12742;
            break;
        case RACE_NIGHTELF:
            specialSurpriseQuestId = 12743;
            break;
        case RACE_DWARF:
            specialSurpriseQuestId = 12744;
            break;
        case RACE_GNOME:
            specialSurpriseQuestId = 12745;
            break;
        case RACE_DRAENEI:
            specialSurpriseQuestId = 12746;
            break;
        case RACE_BLOODELF:
            specialSurpriseQuestId = 12747;
            break;
        case RACE_ORC:
            specialSurpriseQuestId = 12748;
            break;
        case RACE_TROLL:
            specialSurpriseQuestId = 12749;
            break;
        case RACE_UNDEAD_PLAYER:
            specialSurpriseQuestId = 12750;
            break;
        }

        STARTER_QUESTS[22] = specialSurpriseQuestId;
        STARTER_QUESTS[32] = player->GetTeamId() == TEAM_ALLIANCE ? 13188 : 13189;

        for (int questId : STARTER_QUESTS)
        {
            TryCompleteNewQuest(player, questId);
        }

        player->AddItem(38664, true); //Sky Darkener's Shroud of the Unholy
        player->AddItem(39322, true); //Shroud of the North Wind

        // Reset DK to Dalaran, otherwise they get stuck in the Ebon Hold instance
        WorldLocation dalLoc = WorldLocation(571, 5841.65f, 642.086f, 647.512f, 3.83756f); // Dalaran
        player->TeleportTo(571, 5814.0547f, 448.982f, 658.7519f, 0.0f); // Krasus' Landing
        player->SetHomebind(dalLoc, 4395);
    }

    static void HandleShamanQuests(Player* p, uint8 level) {

        std::vector<uint32> shamanQuests = {};

        if (level >= 4) {
            // Call of Earth
            switch (p->getRace())
            {
            case RACE_ORC:
            case RACE_TROLL:
                shamanQuests.push_back(1516);
                shamanQuests.push_back(1517);
                shamanQuests.push_back(1518);
                break;
            case RACE_TAUREN:
                shamanQuests.push_back(1519);
                shamanQuests.push_back(1520);
                shamanQuests.push_back(1521);
                break;
            case RACE_DRAENEI:
                shamanQuests.push_back(9449);
                shamanQuests.push_back(9450);
                shamanQuests.push_back(9451);
                break;
            default:
                break;
            }
        }
        if (level >= 10) {
            // Call of Fire
            switch (p->getRace())
            {
            case RACE_ORC:
            case RACE_TROLL:
            case RACE_TAUREN:
                shamanQuests.push_back(1522);
                shamanQuests.push_back(1524);
                shamanQuests.push_back(1525);
                shamanQuests.push_back(1526);
                shamanQuests.push_back(1527);
                break;
            case RACE_DRAENEI:
                shamanQuests.push_back(9462);
                shamanQuests.push_back(9464);
                shamanQuests.push_back(9465);
                shamanQuests.push_back(9467);
                shamanQuests.push_back(9468);
                shamanQuests.push_back(9461);
                shamanQuests.push_back(9555);
                break;
            default:
                break;
            }
        }
        if (level >= 20) {
            // Call of Water
            switch (p->getRace())
            {
            case RACE_ORC:
            case RACE_TROLL:
            case RACE_TAUREN:
                shamanQuests.push_back(1528);
                shamanQuests.push_back(1530);
                shamanQuests.push_back(1535);
                shamanQuests.push_back(1536);
                shamanQuests.push_back(1534);
                shamanQuests.push_back(220);
                shamanQuests.push_back(63);
                shamanQuests.push_back(100);
                shamanQuests.push_back(96);
                break;
            case RACE_DRAENEI:
                shamanQuests.push_back(9500);
                shamanQuests.push_back(9501);
                shamanQuests.push_back(9503);
                shamanQuests.push_back(9504);
                shamanQuests.push_back(9508);
                shamanQuests.push_back(9509);
                break;
            default:
                break;
            }
        }
        if (level >= 30) {
            // Call of Air
            switch (p->getRace())
            {
            case RACE_ORC:
            case RACE_TROLL:
                shamanQuests.push_back(1531);
                break;
            case RACE_TAUREN:
                shamanQuests.push_back(1532);
                break;
            case RACE_DRAENEI:
                shamanQuests.push_back(9547);
                shamanQuests.push_back(9552);
                shamanQuests.push_back(9553);
                shamanQuests.push_back(9554);
                break;
            default:
                break;
            }
        }

        for (uint32 questId : shamanQuests)
        {
            TryCompleteNewQuest(p, questId);
        }
    }

    static void HandleWarlockQuests(Player* p, uint8 level) {

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

    static void HandleDruidQuests(Player* p, uint8 level) {
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

    static bool TryCompleteNewQuest(Player* player, uint32 questId) {
        if (player->GetQuestStatus(questId) == QUEST_STATUS_NONE)
        {
            player->AddQuest(sObjectMgr->GetQuestTemplate(questId), nullptr);
            player->CompleteQuest(questId);
            player->RewardQuest(sObjectMgr->GetQuestTemplate(questId), 0, player, false);
            return true;
        }
        return false;
    }

    static void HandleTaxi(Player* p, uint8 targetLevel)
    {
        // Based on faction change logic
        TaxiMask const& factionMask = p->GetTeamId() == TEAM_HORDE ? sHordeTaxiNodesMask : sAllianceTaxiNodesMask;
        for (auto const& itr : sTaxiPathSetBySource)
        {
            uint32 nodeId = itr.first;
            uint8 field = (uint8)((nodeId - 1) / 32);
            uint32 submask = 1 << ((nodeId - 1) % 32);

            if ((factionMask[field] & submask) == 0)
            {
                continue;
            }

            TaxiPathSetForSource const& taxiPaths = itr.second;
            if (taxiPaths.empty())
            {
                continue;
            }

            TaxiPathEntry const* taxiPath = taxiPaths.begin()->second;
            if (!taxiPath)
            {
                continue;
            }

            TaxiPathNodeList const& taxiNodePaths = sTaxiPathNodesByPath[taxiPath->ID];
            if (taxiNodePaths.empty())
            {
                continue;
            }

            TaxiPathNodeEntry const* pathNode = taxiNodePaths.front();
            if (!pathNode)
            {
                continue;
            }

            AreaTableEntry const* zone = sAreaTableStore.LookupEntry(sMapMgr->GetZoneId(PHASEMASK_NORMAL, pathNode->mapid, pathNode->x, pathNode->y, pathNode->z));
            if (!zone)
            {
                continue;
            }

            WorldSession* session = p->GetSession();
            LFGDungeonEntry const* lfgDungeon = GetZoneLFGDungeonEntry(zone->area_name[session->GetSessionDbLocaleIndex()], session->GetSessionDbLocaleIndex());
            if (!lfgDungeon)
            {
                continue;
            }

            // Get level from LFGDungeonEntry because the one from AreaTableEntry is not valid
            // If area level is too big, do not add new taxi
            if (lfgDungeon->MinLevel > targetLevel)
            {
                continue;
            }

            p->m_taxi.SetTaximaskNode(nodeId);
        }
    }
};

class FF_Command : public CommandScript
{
public:
    FF_Command() : CommandScript("FF_Command") { }

    Acore::ChatCommands::ChatCommandTable GetCommands() const override
    {
        static Acore::ChatCommands::ChatCommandTable FFSubcommandTable =
        {
            Acore::ChatCommands::ChatCommandBuilder("prep",         HandlePrepCommand,      SEC_PLAYER,     Acore::ChatCommands::Console::No),
            Acore::ChatCommands::ChatCommandBuilder("cancel",       HandleCancelCommand,    SEC_PLAYER,     Acore::ChatCommands::Console::No),
            Acore::ChatCommands::ChatCommandBuilder("prepfor",      HandlePrepForCommand,   SEC_GAMEMASTER, Acore::ChatCommands::Console::Yes),
            Acore::ChatCommands::ChatCommandBuilder("cancelfor",    HandleCancelForCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::Yes),
            Acore::ChatCommands::ChatCommandBuilder("",             HandleFFCommand,        SEC_PLAYER,     Acore::ChatCommands::Console::Yes)
        };

        static Acore::ChatCommands::ChatCommandTable FFCommandBaseTable =
        {
             Acore::ChatCommands::ChatCommandBuilder("ff", FFSubcommandTable)
        };

        return FFCommandBaseTable;
    }

    static bool HandleFFCommand(ChatHandler* handler) {
        if (!sFastForward->Enabled()) { return true; }

        handler->PSendSysMessage("(FF) FastForward on version 1.1");
        return true;
    }

    static bool HandlePrepCommand(ChatHandler* handler, int pSpec, int pLevel) {
        if (!sFastForward->Enabled()) { return true; }

        if (!sFastForward->IsLevelAllowed(pLevel)) {
            handler->PSendSysMessage("Level out of allowable range for pregenerated characters.");
            return false;
        }

        if (pSpec <= 0 || (pSpec > 30 && pSpec != 99)) {
            handler->PSendSysMessage("Spec must correspond to a valid class talent tree spec. See FastForward spec list.");
            return false;
        }

        uint32 accountId = handler->GetSession()->GetAccountId();
        PrepCharacterCore(accountId, pSpec, pLevel);
        handler->PSendSysMessage("Fast-forward successful. Character matching criteria will be auto-scaled on first login.");

        return true;
    }

    static bool HandlePrepForCommand(ChatHandler* handler, std::string accountName, int spec, int level) {
        if (!sFastForward->Enabled()) { return true; }

        if (!sFastForward->IsLevelAllowed(level)) {
            handler->PSendSysMessage("Level out of allowable range for pregenerated characters.");
            return false;
        }

        if (spec <= 0 || (spec > 30 && spec != 99)) {
            handler->PSendSysMessage("Spec must correspond to a valid class talent tree spec. See FastForward spec list.");
            return false;
        }

        uint32 accountId = LookupAccountByName(handler, accountName);
        if (accountId > 0) {
            PrepCharacterCore(accountId, spec, level);
            handler->PSendSysMessage("Fast-forward successful. Character matching criteria will be auto-scaled on first login.");
        }

        return true;
    }

    static bool PrepCharacterCore(uint32 accountId, int spec, int level) {
        CharacterDatabase.Execute("INSERT INTO `ff_character_prep` (`account_id`,`spec`,`level`,`stage`,`claimed`,`created_on`) VALUES ('{}', '{}', '{}', '0', '0', NOW());", accountId, spec, level);
        return true;
    }

    static bool HandleCancelCommand(ChatHandler* handler) {
        if (!sFastForward->Enabled()) { return true; }

        uint32 accountId = handler->GetSession()->GetAccountId();
        CancelPrepCore(accountId);
        handler->PSendSysMessage("Character prep associated with this account has been canceled.");

        return true;
    }

    static bool HandleCancelForCommand(ChatHandler* handler, std::string accountName) {
        if (!sFastForward->Enabled()) { return true; }

        uint32 accountId = LookupAccountByName(handler, accountName);
        if (accountId > 0) {
            CancelPrepCore(accountId);
            handler->PSendSysMessage("Character prep associated with this account has been canceled.");
        }

        return true;
    }

    static bool CancelPrepCore(uint32 accountId) {
        CharacterDatabase.Execute("UPDATE `ff_character_prep` SET `claimed` = 1 WHERE `claimed` = 0 AND `account_id` = '{}';", accountId);
        return true;
    }

    static uint32 LookupAccountByName(ChatHandler* handler, std::string username) {
        QueryResult result = LoginDatabase.Query("SELECT `id` FROM `account` WHERE `username` = '{}';", username);
        if (result) {
            Field* fields = result->Fetch();
            return fields[0].Get<uint32>(); // account id
        }
        else {
            handler->PSendSysMessage(Acore::StringFormat("Unable to find account with username {}.", username).c_str());
            return 0;
        }
    }
};

void AddFastForwardScripts()
{
    new FF_WorldScript();
    new FF_Character_Prep();
    new FF_Command();
}
