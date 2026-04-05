void Decompress(const void* src, void* dst);
#define OBJ_VRAM0      (void *)(VRAM + 0x10000)
extern u16 gAPMISSAnim[];
extern u8 gGfxBreakAnim[];


enum {
    BM_BGCHR_BANIM_IFBACK = 1,
    BM_BGCHR_BANIM_UNK160 = 0x160,
    BM_BGCHR_BANIM_UNK200 = 0x200,

    BM_OBJCHR_BANIM_EFFECT = 384,
    BM_OBJCHR_BANIM_EFFECT2 = 448,
};


struct BattleForecastProc {
    /* 00 */ PROC_HEADER;

    /* 2C */ int unk_2C;
    /* 30 */ s8 x;
    /* 31 */ s8 y;
    /* 32 */ u8 frameKind;
    /* 33 */ s8 ready;
    /* 34 */ s8 needContentUpdate;
    /* 35 */ s8 side; // -1 is left, +1 is right
    /* 36 */ s8 unk_36;
    /* 38 */ struct TextHandle unitNameTextA;
    /* 40 */ struct TextHandle unitNameTextB;
    /* 48 */ struct TextHandle itemNameText;
    /* 50 */ s8 hitCountA;
    /* 51 */ s8 hitCountB;
    /* 52 */ s8 isEffectiveA;
    /* 53 */ s8 isEffectiveB;
};

void PutBattleForecastWeaponTriangleArrows(struct BattleForecastProc* proc);
void UpdateStatArrowSprites(int x, int y, u8 isDown);