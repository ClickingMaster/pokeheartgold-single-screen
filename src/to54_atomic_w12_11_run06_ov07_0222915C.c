#include "global.h"
#define M2C_UNK                 int
#define M2C_FIELD(x, type, off) (*(type)((u8 *)(x) + (off)))

M2C_UNK BgSetPosTextAndCommit(s32, s32, s32, s16);                                         /* extern */
M2C_UNK Heap_Free(void *);                                                                 /* extern */
M2C_UNK ManagedSprite_GetPositionXY(s32 *, void *, M2C_UNK *);                             /* extern */
M2C_UNK ManagedSprite_SetAffineOverwriteMode(s32 *, s32);                                  /* extern */
M2C_UNK ManagedSprite_SetAffineScale(s32 *, f32, s32);                                     /* extern */
M2C_UNK ManagedSprite_SetDrawFlag(s32 *, s32);                                             /* extern */
M2C_UNK ManagedSprite_SetDrawPriority(s32 *, s32);                                         /* extern */
M2C_UNK ManagedSprite_SetOamMode(s32 *, s32);                                              /* extern */
M2C_UNK ManagedSprite_SetPositionXY(s32 *, s16, s16);                                      /* extern */
M2C_UNK ManagedSprite_TickFrame(s32 *);                                                    /* extern */
s32 ObjPlttTransfer_GetPaletteVramOffset(s32, s32);                                        /* extern */
M2C_UNK PaletteData_BeginPaletteFade(s32, s32, u16, s32, s32, s32, s32);                   /* extern */
M2C_UNK PaletteData_BlendPalettes(s32, s32, s32, s32, s32);                                /* extern */
s32 PaletteData_GetSelectedBuffersBitmask();                                               /* extern */
M2C_UNK PaletteData_LoadNarc_CustomTint(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32); /* extern */
s16 Pokepic_GetAttr(s32, s32);                                                             /* extern */
M2C_UNK Pokepic_SetAttr(s32, s32, s32);                                                    /* extern */
M2C_UNK SpriteSystem_DrawSprites(s32);                                                     /* extern */
s32 Sprite_GetPaletteProxy(s32);                                                           /* extern */
s32 BattleAnimSystem_GetHeapID(s32);                                                                    /* extern */
M2C_UNK BattleAnimSystem_CreateTaskDefaultPriority(s32, void (*)(s32, void *), void *);                                 /* extern */
M2C_UNK BattleAnimSystem_DestroyTask(s32, s32);                                                           /* extern */
s32 BattleAnimSystem_GetAttacker(s32);                                                                    /* extern */
s32 BattleAnimSystem_GetDefender(s32);                                                                    /* extern */
s32 BattleAnimSystem_GetBgConfig(s32);                                                                    /* extern */

extern u32 BattleAnimSystem_GetArg(void *p, int i);                                                               /* extern */
s32 *BattleAnimSystem_GetBattlerSprite(s32, s32);                                                              /* extern */
s32 BattleAnimSystem_GetBattlerSpriteManager(s32);                                                                    /* extern */
s32 BattleAnimSystem_GetMoveParam(s32, s32);                                                               /* extern */
u16 BattleAnimSystem_GetBattleBgPaletteMask(s32);                                                                    /* extern */
s32 BattleAnimSystem_GetBattlerPokepic(s32, s32);                                                               /* extern */
s32 BattleAnimSystem_GetPaletteData(s32);                                                                    /* extern */
s32 ov07_0221FA80(s32, s32);                                                               /* extern */
s32 ov07_0221FA90(s32, s32);                                                               /* extern */
s32 ov07_0221FAA0(s32, s32);                                                               /* extern */
s32 BattleAnimSystem_IsDoubleBattle(s32);                                                                    /* extern */
s32 ov07_0221FAF8(s32, s32);                                                               /* extern */
s32 ov07_0221FB78(s32, s32);                                                               /* extern */
s32 ov07_02222004(s32, s32);                                                               /* extern */
s32 ov07_0222202C(s32, s32);                                                               /* extern */
M2C_UNK ov07_02222268(void *, s16, s16, s32, s32);                                         /* extern */
s32 ov07_022222B4(void *);                                                                 /* extern */
M2C_UNK ov07_022223F0(void *, s32, s32, s32);                                              /* extern */
s32 ov07_02222440(void *);                                                                 /* extern */
M2C_UNK ov07_02222590(void *, s32, s32, s32, s32, s32, s32);                               /* extern */
s32 ov07_0222260C(void *);                                                                 /* extern */
M2C_UNK ov07_02222644(void *, f32 *, s32 *);                                               /* extern */
M2C_UNK ov07_022226FC(s32 *, s16, s16, s32, s32);                                          /* extern */
M2C_UNK ov07_022227A8(void *, s16, s16, s16, s32);                                         /* extern */
s32 ov07_022227D8(void *);                                                                 /* extern */
s32 ov07_0222283C(void *, s16, s16, s32);                                                  /* extern */
M2C_UNK ov07_02222AC4(void *, s32, s32, s32, s32, s32);                                    /* extern */
M2C_UNK ov07_02222AF4(void *);                                                             /* extern */
M2C_UNK BattleAnimUtil_SetBlendWithBattleBg(s32, s32, s32);                                                      /* extern */
M2C_UNK BattleAnimUtil_SetBlendWithMask(s32, s32, s32, s32);                                                 /* extern */
M2C_UNK BattleAnimUtil_GetPicPosition(s32, M2C_UNK *);                                                     /* extern */
M2C_UNK BattleAnimUtil_InitCommonWork(s32, void *);                                                        /* extern */
M2C_UNK BattleAnimUtil_CollectPicsByFlags(s32, s32, void *, void *);                                           /* extern */
void *BattleAnimUtil_AllocWork(s32, s32);                                                             /* extern */
void ov07_02228684(s32 arg0, void *arg1);                                                  /* static */
void ov07_02228834(s32 arg0, void *arg1);                                                  /* static */
void ov07_02228B38(s32 arg0, void *arg1);                                                  /* static */
void ov07_02228D08(s32 arg0, void *arg1);                                                  /* static */
void ov07_02228E70(s32 arg0, void *arg1);                                                  /* static */
void ov07_02228F14(s32 arg0, void *arg1);                                                  /* static */
void ov07_02229024(s32 arg0, void *arg1);                                                  /* static */
void ov07_02228750(s32 arg0);
void ov07_02228A50(s32 arg0);
void ov07_02228C8C(s32 arg0);
void ov07_02228D64(s32 arg0);
void ov07_02228F88(s32 arg0);
void ov07_022290BC(s32 arg0);
void ov07_0222915C(s32 arg0, void *arg1);
extern u8 ov07_022366C8;
extern M2C_UNK ov07_02236722;
extern M2C_UNK ov07_0223674C;
extern M2C_UNK ov07_02236750;

void ov07_0222915C(s32 arg0, void *arg1) {
    u8 temp_r0;

    temp_r0 = M2C_FIELD(arg1, u8 *, 0);
    switch (temp_r0) { /* irregular */
    case 0:
        ov07_022227A8(arg1 + 0x40, 0x14, 0, 0, 0xA);
        ov07_022227A8(arg1 + 0x64, 2, 0, 0, 0xA);
        M2C_FIELD(arg1, u8 *, 0) = (u8)(M2C_FIELD(arg1, u8 *, 0) + 1);
        return;
    case 1:
        if (ov07_022227D8(arg1 + 0x40) == 0) {
            M2C_FIELD(arg1, u8 *, 0) = (u8)(M2C_FIELD(arg1, u8 *, 0) + 1);
        } else {
            ManagedSprite_SetPositionXY(M2C_FIELD(arg1, s32 **, 0x38), (s16)(M2C_FIELD(arg1, s16 *, 0x2C) + M2C_FIELD(arg1, s16 *, 0x40)), (s16)(M2C_FIELD(arg1, s16 *, 0x2E) + M2C_FIELD(arg1, s16 *, 0x42)));
            ManagedSprite_SetPositionXY(M2C_FIELD(arg1, s32 **, 0x3C), (s16)(M2C_FIELD(arg1, s16 *, 0x40) - M2C_FIELD(arg1, s16 *, 0x2C)), (s16)(M2C_FIELD(arg1, s16 *, 0x2E) + M2C_FIELD(arg1, s16 *, 0x42)));
            ManagedSprite_TickFrame(M2C_FIELD(arg1, s32 **, 0x38));
            ManagedSprite_TickFrame(M2C_FIELD(arg1, s32 **, 0x3C));
        }
        SpriteSystem_DrawSprites(M2C_FIELD(arg1, s32 *, 0xC));
        return;
    default:
        BattleAnimSystem_DestroyTask(M2C_FIELD(arg1, s32 *, 4), arg0);
        Heap_Free(arg1);
        return;
    }
}
