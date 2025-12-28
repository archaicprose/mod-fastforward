/*
 * Handles class leveling for character generation.
 */

#ifndef FF_CLASS_MGR_H
#define FF_CLASS_MGR_H

#include "ScriptMgr.h"
#include "FFShared.h"

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

class FFClassHandler {
private:
    virtual void LearnWeaponSkills(Player* p) {}
    virtual void LearnArmorSkills(Player* p) {}
protected:
    FFSpec* specInfo;
    uint8 level;

    static bool TryCompleteNewQuest(Player* p, uint32 questId);
    static void LearnPlateSkill(Player* p, uint8 targetLevel) {
        if (targetLevel >= 40) {
            p->learnSpell(750); // Plate
        }
    }

    static void LearnMailSkill(Player* p, uint8 targetLevel) {
        if (targetLevel >= 40) {
            p->learnSpell(8737); // Mail
        }
    }
public:
    FFClassHandler(uint8 pLevel, FFSpec* spec)
    {
        specInfo = spec;
        level = pLevel;
    }
    ~FFClassHandler() {}

    virtual void LearnSkills(Player* p);
    virtual void HandleClassFeatures(Player* p) {}
};

#endif // !FF_CLASS_MGR_H
