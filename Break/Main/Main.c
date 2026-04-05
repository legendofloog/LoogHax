#include  "Main.h"

bool DidUnitBreak(){
	if (gDebuffTable[gBattleTarget.unit.index].skillState & SKILLSTATE_BREAK){
		return false;
	}
	int j = 0;
	while (BreakExemptCharacterList[j] != 0){
		if (BreakExemptCharacterList[j] == gBattleTarget.unit.pCharacterData->number){
			return false; //if target's char ID is on exempt list, no break allowed
		}
		j++;
	}
	int k = 0;
	while (BreakExemptClassList[k] != 0){
		if (BreakExemptClassList[k] == gBattleTarget.unit.pClassData->number){
			return false; //if target's class ID is on exempt list, no break allowed
		}
		k++;
	}
	if ((gBattleActor.battleAttack > gBattleTarget.battleDefense)){ //did unit do damage
		int i = 0;
		while (BreakTargetTable[i].breakerWType != 0xFF){
			if ((gBattleActor.weaponAttributes & IA_REVERTTRIANGLE) && !(gBattleTarget.weaponAttributes & IA_REVERTTRIANGLE)){
				if ((BreakTargetTable[i].breakerWType == GetItemType(gBattleTarget.weaponBefore)) && (BreakTargetTable[i].brokenWType == GetItemType(gBattleActor.weaponBefore))){
					return true;
				}
			}
			else if ((gBattleTarget.weaponAttributes & IA_REVERTTRIANGLE) && !(gBattleActor.weaponAttributes & IA_REVERTTRIANGLE)){
				if ((BreakTargetTable[i].breakerWType == GetItemType(gBattleTarget.weaponBefore)) && (BreakTargetTable[i].brokenWType == GetItemType(gBattleActor.weaponBefore))){
					return true;
				}
			}
			else{
				if ((BreakTargetTable[i].breakerWType == GetItemType(gBattleActor.weaponBefore)) && (BreakTargetTable[i].brokenWType == GetItemType(gBattleTarget.weaponBefore))){
					return true;
				}
				
			}
			i++;
		}
	}
	return false;
}

void BreakPostBattle(){

	// ignore non combat actions
	if (gActionData.unitActionType != UNIT_ACTION_COMBAT){
		return;
	}

	Unit* target = &gBattleTarget.unit;

	// unset break
	bool alreadyBroken = false;
	if (gDebuffTable[target->index].skillState & SKILLSTATE_BREAK){
		gDebuffTable[target->index].skillState &= ~SKILLSTATE_BREAK;
		alreadyBroken = true;
	}

	// ignore dead people
	if (target->curHP <= 0){
		return;
	}

	// make sure the enemy is real
	if (!target->pClassData){
		return;
	}

	// try to apply break
	if(DidUnitBreak() & (!alreadyBroken) && (gDebuffTable[target->index].skillState & SKILLSTATE_BROKEN_IN_BATTLE)){
		gDebuffTable[target->index].skillState |= SKILLSTATE_BREAK;
	}
	//unset the broken in battle bit at the end
	gDebuffTable[target->index].skillState &= ~SKILLSTATE_BROKEN_IN_BATTLE; //gets rid of mid battle break
}

void ClearActiveFactionBreakStatus(){
	Unit* unit;
	int i;
	for (i = gChapterData.currentPhase + 1; i < gChapterData.currentPhase + 0x40; i++) {
        unit = GetUnit(i);

        if (UNIT_IS_VALID(unit)){
			gDebuffTable[unit->index].skillState &= ~SKILLSTATE_BREAK; //undo break status for current actors
			gDebuffTable[unit->index].skillState &= ~SKILLSTATE_BROKEN_IN_BATTLE;
		} 
    }
}