
#include "gbafe.h"

void ApplyUnitDefaultPromotion(struct Unit* unit) {
    const struct ClassData* promotedClass = GetClassData(unit->pClassData->promotion);
    const struct ClassData* currentClass = unit->pClassData;

    int i;

    // Apply stat ups

    unit->maxHP += (promotedClass->baseHP - currentClass->baseHP);

    if (unit->maxHP > promotedClass->maxHP){
        unit->maxHP = promotedClass->maxHP;
    }
        
    unit->curHP += (promotedClass->baseHP - currentClass->baseHP);

    if (unit->curHP > unit->maxHP){
        unit->curHP = unit->maxHP;
    }

    unit->pow += (promotedClass->basePow - currentClass->basePow);

    if (unit->pow > promotedClass->maxPow){
        unit->pow = promotedClass->maxPow;
    }

    unit->skl += (promotedClass->baseSkl - currentClass->baseSkl);

    if (unit->skl > promotedClass->maxSkl){
        unit->skl = promotedClass->maxSkl;
    }
        

    unit->spd += (promotedClass->baseSpd - currentClass->baseSpd);

    if (unit->spd > promotedClass->maxSpd){
        unit->spd = promotedClass->maxSpd;
    }
        

    unit->def += (promotedClass->baseDef - currentClass->baseDef);

    if (unit->def > promotedClass->maxDef){
        unit->def = promotedClass->maxDef;
    }

    unit->res += (promotedClass->baseRes - currentClass->baseRes);

    if (unit->res > promotedClass->maxRes){
		unit->res = promotedClass->maxRes;
	}

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i){
        if (unit->ranks[i] - unit->pClassData->baseRanks[i] >= 0){
            unit->ranks[i] -= unit->pClassData->baseRanks[i];
        }
        else{
            unit->ranks[i] = 0;
        }
    }
        
    // Update unit class
    unit->pClassData = promotedClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i) {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > 251)
            wexp = 251;

        unit->ranks[i] = wexp;
    }

    unit->level = 1;
    unit->exp   = 0;
}

void ApplyUnitPromotion(struct Unit* unit, u8 classId) {
    const struct ClassData* promotedClass = GetClassData(classId);
    const struct ClassData* currentClass = unit->pClassData;

    int i;

    // Apply stat ups

    unit->maxHP += (promotedClass->baseHP - currentClass->baseHP);

    if (unit->maxHP > promotedClass->maxHP){
        unit->maxHP = promotedClass->maxHP;
    }
        
    unit->curHP += (promotedClass->baseHP - currentClass->baseHP);

    if (unit->curHP > unit->maxHP){
        unit->curHP = unit->maxHP;
    }

    unit->pow += (promotedClass->basePow - currentClass->basePow);

    if (unit->pow > promotedClass->maxPow){
        unit->pow = promotedClass->maxPow;
    }

    unit->skl += (promotedClass->baseSkl - currentClass->baseSkl);

    if (unit->skl > promotedClass->maxSkl){
        unit->skl = promotedClass->maxSkl;
    }
        

    unit->spd += (promotedClass->baseSpd - currentClass->baseSpd);

    if (unit->spd > promotedClass->maxSpd){
        unit->spd = promotedClass->maxSpd;
    }
        

    unit->def += (promotedClass->baseDef - currentClass->baseDef);

    if (unit->def > promotedClass->maxDef){
        unit->def = promotedClass->maxDef;
    }

    unit->res += (promotedClass->baseRes - currentClass->baseRes);

    if (unit->res > promotedClass->maxRes){
		unit->res = promotedClass->maxRes;
	}

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i){
        if (unit->ranks[i] - unit->pClassData->baseRanks[i] >= 0){
            unit->ranks[i] -= unit->pClassData->baseRanks[i];
        }
        else{
            unit->ranks[i] = 0;
        }
    }
        

    // Update unit class
    unit->pClassData = promotedClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i) {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > 251)
            wexp = 251;

        unit->ranks[i] = wexp;
    }

    unit->level = 1;
    unit->exp   = 0;
}