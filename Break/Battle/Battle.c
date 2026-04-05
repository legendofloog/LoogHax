#include "Battle.h"

void BattleUnwind(void) {
    ClearBattleHits();

    // this do { ... } while (0); is required for match
    // which is kind of neat because it implies scrapped plans for supporting some accost kind of thing
    do {
        struct BattleUnit* attacker;
        struct BattleUnit* defender;
        BattleGetBattleUnitOrder(&attacker, &defender);

        gBattleHitIterator->info |= BATTLE_HIT_INFO_BEGIN;

        if (!BattleGenerateRoundHits(attacker, defender)) {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_RETALIATE;

            if (!BattleGenerateRoundHits(defender, attacker) && BattleGetFollowUpOrder(&attacker, &defender)) {
                gBattleHitIterator->attributes = BATTLE_HIT_ATTR_FOLLOWUP;

                BattleGenerateRoundHits(attacker, defender);
            }
        }
    } while (FALSE);
	//gDebuffTable[gBattleTarget.unit.index].skillState &= ~SKILLSTATE_BROKEN_IN_BATTLE; //gets rid of mid battle break
    gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
}

s8 BattleGenerateRoundHits(struct BattleUnit* attacker, struct BattleUnit* defender) {
    int i, count;
    u16 attrs; // NOTE: this is a bug! attrs are 19 bits in FE8 (they're 16 bits in previous games)

    if (!(attacker->weapon.number) && !(attacker->weapon.durability)){
		return FALSE;
	}
        

	if (gDebuffTable[attacker->unit.index].skillState & SKILLSTATE_BROKEN_IN_BATTLE){
		return FALSE; //checks if attacker is not broken and not the battle actor
	}

    attrs = gBattleHitIterator->attributes;
    count = GetBattleUnitHitCount(attacker);

    for (i = 0; i < count; ++i) {
        gBattleHitIterator->attributes |= attrs;

        if (BattleGenerateHit(attacker, defender))
            return TRUE;
    }

    return FALSE;
}

void BattleGenerateHitEffects(struct BattleUnit* attacker, struct BattleUnit* defender) {
    attacker->wexpMultiplier++;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS)) {
        if (DidUnitBreak() && (attacker->unit.pCharacterData->number == gBattleActor.unit.pCharacterData->number)){
            if (gBattleStats.config & BATTLE_CONFIG_REAL){
                if (gDebuffTable[gBattleTarget.unit.index].skillState & SKILLSTATE_BROKEN_IN_BATTLE){ //if this already set, do not set break

                }
                else{
                    gDebuffTable[gBattleTarget.unit.index].skillState |= SKILLSTATE_BROKEN_IN_BATTLE;
			        gBattleHitIterator->attributes |= BATTLE_HIT_BREAK;
                }
                
            }
		}
		
		switch (GetItemWeaponEffect(attacker->weapon)) {

        case WPN_EFFECT_POISON:
            // Poison defender

            defender->statusOut = UNIT_STATUS_POISON;
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

            // "Ungray" defender if it was petrified (as it won't be anymore)
            if (defender->unit.statusIndex == UNIT_STATUS_PETRIFY || defender->unit.statusIndex == UNIT_STATUS_13)
                defender->unit.state = defender->unit.state &~ US_UNSELECTABLE;

            break;

        case WPN_EFFECT_HPHALVE:
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPHALVE;
            break;

        } // switch (GetItemWeaponEffect(attacker->weapon))

    	if ((GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL) && (BattleRoll1RN(31 - attacker->unit.lck, FALSE))) {
        	gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_DEVIL;

        	attacker->unit.curHP -= gBattleStats.damage;

        	if (attacker->unit.curHP < 0)
            	attacker->unit.curHP = 0;
    		}
		else {
        	if (gBattleStats.damage > defender->unit.curHP)
            	gBattleStats.damage = defender->unit.curHP;

        	defender->unit.curHP -= gBattleStats.damage;

        	if (defender->unit.curHP < 0)
            	defender->unit.curHP = 0;
    	}

        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN) {
            if (attacker->unit.maxHP < (attacker->unit.curHP + gBattleStats.damage))
                attacker->unit.curHP = attacker->unit.maxHP;
            else
                attacker->unit.curHP += gBattleStats.damage;

            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
        }

            if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_PETRIFY) {
                switch (gChapterData.currentPhase) {

                case FACTION_BLUE:
                    if (UNIT_FACTION(&defender->unit) == FACTION_BLUE)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                case FACTION_RED:
                    if (UNIT_FACTION(&defender->unit) == FACTION_RED)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                case FACTION_GREEN:
                    if (UNIT_FACTION(&defender->unit) == FACTION_GREEN)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                } // switch (gRAMChapterData.faction)

                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_PETRIFY;
            }
    }

    gBattleHitIterator->hpChange = gBattleStats.damage;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS) || attacker->weaponAttributes & (IA_UNCOUNTERABLE | IA_MAGIC)) {
        attacker->weapon = GetItemAfterUse(attacker->weapon);

        if (!(attacker->weapon.number) && !(attacker->weapon.durability))
            attacker->weaponBroke = TRUE;
    }
}


void New_BattleInitTargetCanCounter(){

	// eggs
	if (gBattleTarget.unit.pClassData->number == 0x34 || gBattleTarget.unit.pClassData->number == 0x62){
		gBattleTarget.weapon.number = 0;
		gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// attacker weapon is uncounterable
	if (gBattleActor.weaponAttributes & IA_UNCOUNTERABLE){
		gBattleTarget.weapon.number = 0;
		gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// target weapon is uncounterable
	if (gBattleTarget.weaponAttributes & IA_UNCOUNTERABLE){
		gBattleTarget.weapon.number = 0;
		gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// attacker is berserked and both units are blue
	if ((gBattleActor.unit.statusIndex == UNIT_STATUS_BERSERK) && (gBattleActor.unit.index & FACTION_BLUE) && (gBattleTarget.unit.index & FACTION_BLUE)){
		gBattleTarget.weapon.number = 0;
		gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

    // attacker has dazzle
	if (gSkillTester(&gBattleActor.unit, DazzleIDLink)){
		gBattleTarget.weapon.number = 0;
        gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// defender is broken
	if (gDebuffTable[gBattleTarget.unit.index].skillState & SKILLSTATE_BREAK){
		gBattleTarget.weapon.number = 0;
		gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

}

s8 CanUnitUseWeapon(Unit* unit, Item item) {
    if (item.number == 0 && item.durability == 0){
		return FALSE;
	}

	if (gDebuffTable[unit->index].skillState & SKILLSTATE_BREAK){
		return FALSE;
	}

    if (!(GetItemAttributes(item) & IA_WEAPON))
        return FALSE;

    if (GetItemAttributes(item) & IA_LOCK_ANY) {
        // Check for item locks

        if ((GetItemAttributes(item) & IA_LOCK_1) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_1))
            return FALSE;

        if ((GetItemAttributes(item) & IA_LOCK_4) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_4))
            return FALSE;

        if ((GetItemAttributes(item) & IA_LOCK_5) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_5))
            return FALSE;

        if ((GetItemAttributes(item) & IA_LOCK_6) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_6))
            return FALSE;

        if ((GetItemAttributes(item) & IA_LOCK_7) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_7))
            return FALSE;

        if ((GetItemAttributes(item) & IA_LOCK_2) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_2))
            return FALSE;

        // Monster lock is special
        if (GetItemAttributes(item) & IA_LOCK_3) {
            if (!(UNIT_CATTRIBUTES(unit) & CA_LOCK_3))
                return FALSE;

            return TRUE;
        }

        if (GetItemAttributes(item) & IA_UNUSABLE)
            if (!(IsItemUnsealedForUnit(unit, item)))
                return FALSE;
    }

    if ((unit->statusIndex == UNIT_STATUS_SILENCED) && (GetItemAttributes(item) & IA_MAGIC)){
		return FALSE;
	}
        
    int wRank = GetItemRequiredExp(item);
    int uRank = (unit->ranks[GetItemType(item)]);

    return (uRank >= wRank) ? TRUE : FALSE;
}

int MapMain_SwitchPhases(void) {

    ClearActiveFactionGrayedStates();
    SMS_UpdateFromGameData();
    SwitchGameplayPhase();
    ClearActiveFactionBreakStatus(); //after the switch, clear acting faction break status

    if (RunPhaseSwitchEvents() == 1) {
        return 0;
    }

    return 1;
}