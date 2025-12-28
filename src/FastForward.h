#ifndef AZEROTHCORE_FASTFORWARD_H
#define AZEROTHCORE_FASTFORWARD_H

#include "Chat.h"
#include "Config.h"
#include "DataMap.h"
#include "Mail.h"
#include "MapMgr.h"
#include "Object.h"
#include "Pet.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptMgr.h"
#include "FFItemMgr.h"
#include "FFClassHandler.h"
#include "ClassHandlers/FFWarriorClassHandler.h"
#include "ClassHandlers/FFPaladinClassHandler.h"
#include "ClassHandlers/FFHunterClassHandler.h"
#include "ClassHandlers/FFRogueClassHandler.h"
#include "ClassHandlers/FFPriestClassHandler.h"
#include "ClassHandlers/FFDeathKnightClassHandler.h"
#include "ClassHandlers/FFShamanClassHandler.h"
#include "ClassHandlers/FFMageClassHandler.h"
#include "ClassHandlers/FFWarlockClassHandler.h"
#include "ClassHandlers/FFDruidClassHandler.h"

class FastForward
{
public:
    static FastForward* instance();

    bool fastforwardEnabled;
    uint8 maxAllowableLevel, minAllowableLevel;

    [[nodiscard]] bool Enabled() const { return fastforwardEnabled; }
    [[nodiscard]] bool IsLevelAllowed(int level) const;
};

#define sFastForward FastForward::instance()

#endif //AZEROTHCORE_FASTFORWARD_H
