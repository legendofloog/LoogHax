
#include "gbafe.h"

void ComputeBattleUnitSpecialWeaponStats(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (attacker->weaponAttributes & IA_MAGICDAMAGE) {
        
        attacker->battleAttack -= attacker->unit.pow; //take away their attack stat because we're doing fixed damage

        } // switch (GetItemIndex(attacker->weapon))
    else {
        if (GetItemWeaponEffect(attacker->weaponBefore) == WPN_EFFECT_HPHALVE) {
            attacker->battleAttack = (defender->unit.curHP + 1) >> 1;

            if (attacker->battleAttack == 0)
                attacker->battleAttack = 1;

            defender->battleDefense = 0;

            attacker->battleCritRate = 0;
            attacker->battleEffectiveCritRate = 0;
        }

        if (attacker->weaponAttributes & IA_NEGATE_DEFENSE)
            defender->battleDefense = 0;

        if (defender->unit.statusIndex == UNIT_STATUS_PETRIFY || defender->unit.statusIndex == UNIT_STATUS_13) {
            attacker->battleEffectiveHitRate = 100;

            attacker->battleEffectiveCritRate += 30;

            if (attacker->battleEffectiveCritRate > 100)
                attacker->battleEffectiveCritRate = 100;
        }
    }
}