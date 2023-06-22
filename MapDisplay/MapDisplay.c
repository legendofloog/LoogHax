#include "MapDisplay.h"

void MapAnim_BeginBREAKAnim(struct Unit* unit)
{
    Decompress(
        gGfxBreakAnim,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT);

    APProc_Create(
        gAPMISSAnim,
        16*(unit->xPos - (gGameState.cameraRealPos.x>>4)) + 8,
        16*(unit->yPos - (gGameState.cameraRealPos.y>>4)) + 16,
        TILEREF(BM_OBJCHR_BANIM_EFFECT, 0), 0, 2);
}

void PutBattleForecastWeaponTriangleArrows(struct BattleForecastProc* proc) {
    int wtArrowA = 0;
    int wtArrowB = 0;

    if (DidUnitBreak()) { //at advantage
        wtArrowA = 1;
        wtArrowB = 2;
    }

    if (wtArrowB != 0) {
        UpdateStatArrowSprites((proc->x + 8) * 8 + 3, (proc->y + 11) * 8, wtArrowB == 2 ? 1 : 0);
    }

    if (wtArrowA != 0) {
        UpdateStatArrowSprites((proc->x + 2) * 8 + 3, (proc->y + 1) * 8, wtArrowA == 2 ? 1 : 0);
    }

    return;
}

/*void DisplayUnitAdditionalBlinkingIcons(void) {
    u8 protectCharacterId;
    int i;
    int x;
    int y;
    s8 displayRescueIcon;

    int poisonIconFrame;
    int sleepIconFrame;
    int berserkIconFrame;
    int silenceIconFrame;

    u16 rescuePalLut[] = {
        0xC,
        0xE,
        0xD,
    };

    if (GetChapterThing() != 2) {
        protectCharacterId = GetROMChapterStruct(gRAMChapterData.chapterIndex)->protectCharacterIndex;
    } else {
        protectCharacterId = 0;
    }

    displayRescueIcon = (GetGameClock() % 32) < 20 ? 1 : 0;

    poisonIconFrame = GetGameClock() / 8 % ARRAY_COUNT(sPoisonIconSprites);
    sleepIconFrame = GetGameClock() / 16 % ARRAY_COUNT(sSleepIconSprites);
    berserkIconFrame = GetGameClock() / 8 % ARRAY_COUNT(sBerserkIconSprites);
    silenceIconFrame = GetGameClock() / 4 % ARRAY_COUNT(sSilenceIconSprites);

    if (CheckEventId(0x84) != 0) {
        return;
    }

    PutChapterMarkedTileIconOam();

    for (i = 1; i < 0xc0; i++) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit)) {
            continue;
        }

        if (unit->state & US_HIDDEN) {
            continue;
        }

        if (GetUnitSpriteHideFlag(unit) != 0) {
            continue;
        }

        switch (unit->statusIndex) {
            case UNIT_STATUS_POISON:
                x = unit->xPos * 16 - gGameState.camera.x;
                y = unit->yPos * 16 - gGameState.camera.y;

                if (x < -16 || x > DISPLAY_WIDTH) {
                    break;
                }

                if (y < -16 || y > DISPLAY_HEIGHT) {
                    break;
                }

                CallARM_PushToSecondaryOAM(OAM1_X(0x200+x - 2), OAM0_Y(0x100+y - 4), sPoisonIconSprites[poisonIconFrame], 0);

                break;

            case UNIT_STATUS_SILENCED:
                x = unit->xPos * 16 - gGameState.camera.x;
                y = unit->yPos * 16 - gGameState.camera.y;

                if (x < -16 || x > DISPLAY_WIDTH) {
                    break;
                }

                if (y < -16 || y > DISPLAY_HEIGHT) {
                    break;
                }

                CallARM_PushToSecondaryOAM(OAM1_X(0x200+x - 2), OAM0_Y(0x100+y - 4), sSilenceIconSprites[silenceIconFrame], 0);

                break;

            case UNIT_STATUS_SLEEP:
                x = unit->xPos * 16 - gGameState.camera.x;
                y = unit->yPos * 16 - gGameState.camera.y;

                if (x < -16 || x > DISPLAY_WIDTH) {
                    break;
                }

                if (y < -16 || y > DISPLAY_HEIGHT) {
                    break;
                }

                CallARM_PushToSecondaryOAM(OAM1_X(0x200+x + 2), OAM0_Y(0x100+y), sSleepIconSprites[sleepIconFrame], 0);

                break;

            case UNIT_STATUS_BERSERK:
                x = unit->xPos * 16 - gGameState.camera.x;
                y = unit->yPos * 16 - gGameState.camera.y;

                if (x < -16 || x > DISPLAY_WIDTH) {
                    break;
                }

                if (y < -16 || y > DISPLAY_HEIGHT) {
                    break;
                }

                CallARM_PushToSecondaryOAM(OAM1_X(0x200+x + 1), OAM0_Y(0x100+y - 5), sBerserkIconSprites[berserkIconFrame], 0);

                break;

            case UNIT_STATUS_ATTACK:
            case UNIT_STATUS_DEFENSE:
            case UNIT_STATUS_CRIT:
            case UNIT_STATUS_AVOID:
                if (!displayRescueIcon) {
                    continue;
                }

                x = unit->xPos * 16 - gGameState.camera.x;
                y = unit->yPos * 16 - gGameState.camera.y;

                if (x < -16 || x > DISPLAY_WIDTH) {
                    break;
                }

                if (y < -16 || y > DISPLAY_HEIGHT) {
                    break;
                }

                CallARM_PushToSecondaryOAM(OAM1_X(0x200+x - 1), OAM0_Y(0x100+y - 5), sSprite_0859B968, 0);
                break;

            case UNIT_STATUS_SICK:
            case UNIT_STATUS_RECOVER:
                break;
        }

        if (!displayRescueIcon) {
            continue;
        }

        if (unit->state & US_RESCUING) {
            x = unit->xPos * 16 - gGameState.camera.x;
            y = unit->yPos * 16 - gGameState.camera.y;

            if (x < -16 || x > DISPLAY_WIDTH) {
                continue;
            }

            if (y < -16 || y > DISPLAY_HEIGHT) {
                continue;
            }

            CallARM_PushToSecondaryOAM(OAM1_X(0x200+x + 9), OAM0_Y(0x100+y + 7), gObject_8x8, (rescuePalLut[unit->rescueOtherUnit >> 6] & 0xf) * 0x1000 + 0x803);
        } else if ((UNIT_FACTION(unit) != FACTION_BLUE) && (UNIT_CATTRIBUTES(unit) & CA_BOSS)) {
            x = unit->xPos * 16 - gGameState.camera.x;
            y = unit->yPos * 16 - gGameState.camera.y;

            if (x < -16 || x > DISPLAY_WIDTH) {
                continue;
            }

            if (y < -16 || y > DISPLAY_HEIGHT) {
                continue;
            }

            CallARM_PushToSecondaryOAM(OAM1_X(0x200+x + 9), OAM0_Y(0x100+y + 7), gObject_8x8, 0x810);
        } else if (protectCharacterId == unit->pCharacterData->number) {
            x = unit->xPos * 16 - gGameState.camera.x;
            y = unit->yPos * 16 - gGameState.camera.y;

            if (x < -16 || x > DISPLAY_WIDTH) {
                continue;
            }

            if (y < -16 || y > DISPLAY_HEIGHT) {
                continue;
            }

            CallARM_PushToSecondaryOAM(OAM1_X(0x200+x + 9), OAM0_Y(0x100+y + 7), gObject_8x8, 0x811);
        }
    }

    return;
}*/