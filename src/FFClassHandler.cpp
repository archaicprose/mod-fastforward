#include "FFClassHandler.h"

void FFClassHandler::LearnSkills(Player* p)
{
    // Teach Riding
    if (level >= 20) {
        p->learnSpell(33388);
    }
    if (level >= 40) {
        p->learnSpell(33391);
    }
    if (level >= 60) {
        p->learnSpell(34090);
    }
    if (level >= 70) {
        p->learnSpell(34091);
    }
    if (level >= 77) {
        p->learnSpell(54197);
    }

    // Dual Spec
    if (level >= 40) {
        p->CastSpell(p, 63624, p->GetGUID());   // Second Talent Specialization
        p->CastSpell(p, 63680, p->GetGUID());   // Talent Switching
    }

    // Weapons & Armor
    LearnWeaponSkills(p);
    LearnArmorSkills(p);

    p->UpdateSkillsToMaxSkillsForLevel();
}

bool FFClassHandler::TryCompleteNewQuest(Player* player, uint32 questId) {
    if (player->GetQuestStatus(questId) == QUEST_STATUS_NONE)
    {
        player->AddQuest(sObjectMgr->GetQuestTemplate(questId), nullptr);
        player->CompleteQuest(questId);
        player->RewardQuest(sObjectMgr->GetQuestTemplate(questId), 0, player, false);
        return true;
    }
    return false;
}

void FFClassHandler::LearnForLevel(Player* p, const std::unordered_map<uint8, std::vector<uint32>>& spellMap) {
    for (int l = 1; l <= level; l++)
    {
        if (spellMap.contains(l)) {
            std::vector<uint32> spellsAtLevel = spellMap.at(l);
            for (uint32 spell : spellsAtLevel) {
                p->learnSpell(spell);
            }
        }
    }
}
