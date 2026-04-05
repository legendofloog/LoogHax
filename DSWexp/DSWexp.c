
#include "gbafe.h"

#define PLAY_FLAG_EXTRA_MAP (1 << 7)

int GetBattleUnitUpdatedWeaponExp(BattleUnit* battleUnit) {
    int i, result;

    if (UNIT_FACTION(&battleUnit->unit) != FACTION_BLUE){
		return -1;
	}
    if (battleUnit->unit.curHP == 0){
		return -1;
	}
    if (gChapterData.chapterStateBits & PLAY_FLAG_EXTRA_MAP){
        return -1;
	}
    if (gGameState.statebits & 0x40){ // TODO: GAME STATE BITS CONSTANTS
 		return -1;
	} 
    if (!(gBattleStats.config & BATTLE_CONFIG_ARENA)) {
        if (!battleUnit->canCounter){
 			return -1;
		}
        if (!(battleUnit->weaponAttributes & IA_REQUIRES_WEXP)){
			return -1;
		}
            
	}
    const ItemData* currentWeaponData = GetItemData(GetItemIndex(battleUnit->weaponBefore));
    u8 currentWeaponType = currentWeaponData->weaponType;
	result = battleUnit->unit.ranks[currentWeaponType];
    result += 2;

    for (i = 0; i < 8; ++i) {
        if (i == battleUnit->weaponType){
            continue;
		}
        if (battleUnit->unit.pClassData->baseRanks[i] == WPN_EXP_S){
            continue;
		}
        if (battleUnit->unit.ranks[i] < WPN_EXP_S){
            continue;
		}
        if (result >= WPN_EXP_S){
			result = WPN_EXP_S;
		}
            
        break;
    }

    if (UNIT_CATTRIBUTES(&battleUnit->unit) & CA_PROMOTED) {
        if (result > WPN_EXP_S){
            result = WPN_EXP_S;
        }
    }
    else {
        if (result > WPN_EXP_A){
            result = WPN_EXP_A;
        }    
    }

    return result;
}

s8 HasBattleUnitGainedWeaponLevel(struct BattleUnit* bu) {
    int oldWexp = bu->unit.ranks[(GetItemData(GetItemIndex(bu->weaponBefore)))->weaponType];
    int newWexp = GetBattleUnitUpdatedWeaponExp(bu);

    if (newWexp < 0)
        return FALSE;

    return GetWeaponLevelFromExp(oldWexp) != GetWeaponLevelFromExp(newWexp);
}

void UpdateUnitFromBattle(struct Unit* unit, struct BattleUnit* bu) {
    int tmp;

    unit->level = bu->unit.level;
    unit->exp   = bu->unit.exp;
    unit->curHP = bu->unit.curHP;
    unit->state = bu->unit.state;

    //gUnknown_03003060 = UNIT_ARENA_LEVEL(unit); ?? idunno what this is supposed to do

    if (bu->statusOut >= 0)
        SetUnitStatus(unit, bu->statusOut);

    unit->maxHP += bu->changeHP;
    unit->pow   += bu->changePow;
    unit->skl   += bu->changeSkl;
    unit->spd   += bu->changeSpd;
    unit->def   += bu->changeDef;
    unit->res   += bu->changeRes;
    unit->lck   += bu->changeLck;

    UnitCheckStatCaps(unit);

    tmp = GetBattleUnitUpdatedWeaponExp(bu);

    if (tmp > 0)
        unit->ranks[(GetItemData(GetItemIndex(bu->weaponBefore)))->weaponType] = tmp;

    for (tmp = 0; tmp < UNIT_ITEM_COUNT; ++tmp)
        unit->items[tmp] = bu->unit.items[tmp];

    UnitRemoveInvalidItems(unit);

    //if (bu->expGain)  @ this is irrelevant anyway
        //BWL_AddExpGained(unit->pCharacterData->number, bu->expGain); this
}

void UpdateUnitDuringBattle(struct Unit* unit, struct BattleUnit* bu) {
    int wexp;

    unit->curHP = bu->unit.curHP;

    wexp = GetBattleUnitUpdatedWeaponExp(bu);

    if (wexp > 0)
        unit->ranks[(GetItemData(GetItemIndex(bu->weaponBefore)))->weaponType] = wexp;
}