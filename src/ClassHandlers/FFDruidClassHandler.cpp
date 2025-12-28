#include "FFDruidClassHandler.h"

void FFDruidClassHandler::LearnWeaponSkills(Player* p)
{
    p->learnSpell(TWO_H_MACES);
    p->learnSpell(STAVES);
    p->learnSpell(POLEARMS);
    p->learnSpell(ONE_H_MACES);
    p->learnSpell(FIST_WEAPONS);
    p->learnSpell(DAGGERS);
}

void FFDruidClassHandler::LearnSkills(Player* p)
{
    FFClassHandler::LearnSkills(p);
}

void FFDruidClassHandler::HandleClassFeatures(Player* p)
{
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
