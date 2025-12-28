
#include "FastForward.h"
#include <chrono>

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
            auto startTime = std::chrono::high_resolution_clock::now();

            Field* fields = result->Fetch();
            uint8 targetSpec = fields[0].Get<uint8>();
            uint8 targetLevel = fields[1].Get<uint8>();
            TeamId pTeam = p->GetTeamId();
            FFSpec* specInfo = FFSpecMgr::GetSpecInfo(targetSpec);
            FFItemMgr* itemMgr = new FFItemMgr(targetLevel, p->getRace(), specInfo);

            // Is this preparation valid for this new character?
            if (itemMgr->GetIntendedClass() != p->getClass()) {
                return;
            }

            MailDraft welcomeMail = MailDraft("Fast Forward", "You blacked out at the inn last night, ranting on about thinking you were back at the beginning again. You foolishly left all your belongings laying about the place, but I packed them into your bags when you were asleep.");

            // =======|   LEVEL     |=======
            p->GiveLevel(targetLevel);

            // =======|   SKILLS    |=======

            LOG_INFO("module.fastforward", "Starting skills...");
            FFClassHandler* classHandler = GetClassHandler(targetLevel, specInfo);
            LOG_INFO("module.fastforward", "Learning skills...");
            classHandler->LearnSkills(p);
            LOG_INFO("module.fastforward", "Learning class features...");
            classHandler->HandleClassFeatures(p);

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

            auto finishTime = std::chrono::high_resolution_clock::now();
            auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - startTime);
            LOG_INFO("module.fastforward", "FastForward Complete. Total Duration {} ms", totalDuration);
        }
    }

    void OnPlayerFirstLogin(Player* p) override {

        // =======|   LOCATION  |=======
        WorldLocation dalLoc = WorldLocation(571, 5841.65f, 642.086f, 647.512f, 3.83756f); // Dalaran
        //p->TeleportTo(571, 5814.0547f, 448.982f, 658.7519f, 0.0f); // Krasus' Landing
        p->SetHomebind(dalLoc, 4395);

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

    FFClassHandler* GetClassHandler(uint8 pLevel, FFSpec* spec)
    {
        switch (spec->ffclass) {
        case CLASS_WARRIOR:
            return new FFWarriorClassHandler(pLevel, spec);
        case CLASS_PALADIN:
            return new FFPaladinClassHandler(pLevel, spec);
        case CLASS_DEATH_KNIGHT:
            return new FFDeathKnightClassHandler(pLevel, spec);
        case CLASS_HUNTER:
            return new FFHunterClassHandler(pLevel, spec);
        case CLASS_SHAMAN:
            return new FFShamanClassHandler(pLevel, spec);
        case CLASS_PRIEST:
            return new FFPriestClassHandler(pLevel, spec);
        case CLASS_MAGE:
            return new FFMageClassHandler(pLevel, spec);
        case CLASS_WARLOCK:
            return new FFWarlockClassHandler(pLevel, spec);
        case CLASS_ROGUE:
            return new FFRogueClassHandler(pLevel, spec);
        case CLASS_DRUID:
            return new FFDruidClassHandler(pLevel, spec);
        default:
            break;
        }

        return new FFClassHandler(pLevel, spec);
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
