#include "FFDeathKnightClassHandler.h"

void FFDeathKnightClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(TWO_H_SWORDS);
    p->learnSpell(TWO_H_MACES);
    p->learnSpell(TWO_H_AXES);
    p->learnSpell(POLEARMS);
    p->learnSpell(ONE_H_SWORDS);
    p->learnSpell(ONE_H_MACES);
    p->learnSpell(ONE_H_AXES);
}

void FFDeathKnightClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
}

void FFDeathKnightClassHandler::HandleClassFeatures(Player* p)
{
    if (level >= 58) {
        // DK Start Skip
        // The following is an abridged version of the "DK Start Skip" Mod
        p->AddItem(6948, true); //Hearthstone
        int STARTER_QUESTS[33] = { 12593, 12619, 12842, 12848, 12636, 12641, 12657, 12678, 12679, 12680, 12687, 12698, 12701, 12706, 12716, 12719, 12720, 12722, 12724, 12725, 12727, 12733, -1, 12751, 12754, 12755, 12756, 12757, 12779, 12801, 13165, 13166 };
        int specialSurpriseQuestId = -1;
        switch (p->getRace())
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
        STARTER_QUESTS[32] = p->GetTeamId() == TEAM_ALLIANCE ? 13188 : 13189;

        for (int questId : STARTER_QUESTS)
        {
            TryCompleteNewQuest(p, questId);
        }

        p->AddItem(38664, true); //Sky Darkener's Shroud of the Unholy
        p->AddItem(39322, true); //Shroud of the North Wind

        // Reset DK to Dalaran, otherwise they get stuck in the Ebon Hold instance
        WorldLocation dalLoc = WorldLocation(571, 5841.65f, 642.086f, 647.512f, 3.83756f); // Dalaran
        p->TeleportTo(571, 5814.0547f, 448.982f, 658.7519f, 0.0f); // Krasus' Landing
        p->SetHomebind(dalLoc, 4395);
    }
}
