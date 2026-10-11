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
s32 BattleAnimUtil_GetBattlerSideSign(s32, s32);                                                               /* extern */
M2C_UNK BattleAnimUtil_InitLinearMotion(void *, s16, s16, s32, s32);                                         /* extern */
s32 BattleAnimUtil_UpdateLinearMotion(void *);                                                                 /* extern */
M2C_UNK BattleAnimUtil_InitValueMotion(void *, s32, s32, s32);                                              /* extern */
s32 BattleAnimUtil_UpdateValueMotion(void *);                                                                 /* extern */
M2C_UNK BattleAnimUtil_InitScaleMotionXY(void *, s32, s32, s32, s32, s32, s32);                               /* extern */
s32 BattleAnimUtil_UpdateScaleMotionXY(void *);                                                                 /* extern */
M2C_UNK BattleAnimUtil_GetScaleFloat(void *, f32 *, s32 *);                                               /* extern */
M2C_UNK BattleAnimUtil_AlignSpriteToScale(s32 *, s16, s16, s32, s32);                                          /* extern */
M2C_UNK BattleAnimUtil_InitShake(void *, s16, s16, s16, s32);                                         /* extern */
s32 BattleAnimUtil_UpdateShake(void *);                                                                 /* extern */
s32 BattleAnimUtil_UpdateShakePokepic(void *, s16, s16, s32);                                                  /* extern */
M2C_UNK BattleAnimUtil_StartBlendAlphaMotion(void *, s32, s32, s32, s32, s32);                                    /* extern */
M2C_UNK BattleAnimUtil_IsBlendAlphaMotionDone(void *);                                                             /* extern */
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

void ov07_02228750(s32 arg0) {
    s32 sp1C;
    s32 sp18;
    s32 temp_r0_2;
    s32 temp_r6;
    s32 temp_r7;
    void *temp_r0;

    temp_r0 = BattleAnimUtil_AllocWork(arg0, 0x14);
    M2C_FIELD(temp_r0, s32 *, 4) = arg0;
    M2C_FIELD(temp_r0, s32 *, 8) = BattleAnimSystem_GetBattlerSpriteManager(arg0);
    M2C_FIELD(temp_r0, s32 **, 0xC) = BattleAnimSystem_GetBattlerSprite(M2C_FIELD(temp_r0, s32 *, 4), 0);
    M2C_FIELD(temp_r0, s32 **, 0x10) = BattleAnimSystem_GetBattlerSprite(M2C_FIELD(temp_r0, s32 *, 4), 1);
    M2C_FIELD((void *)0x04000000, s32 *, 0) = (s32)((0x04000000U >> 0xB) | (M2C_FIELD((void *)0x04000000, s32 *, 0) & 0xFFFF1FFF));
    M2C_FIELD((void *)0x04000000, u16 *, 0x4A) = (u16)(0x1F | (M2C_FIELD((void *)0x04000000, u16 *, 0x4A) & ~0x3F));
    M2C_FIELD((void *)0x04000000, u16 *, 0x4A) = (u16)(0x1200 | (M2C_FIELD((void *)0x04000000, u16 *, 0x4A) & 0xFFFFC0FF));
    temp_r7 = ov07_0221FA80(M2C_FIELD(temp_r0, s32 *, 4), BattleAnimSystem_GetAttacker(M2C_FIELD(temp_r0, s32 *, 4)));
    sp18 = ov07_0221FA90(M2C_FIELD(temp_r0, s32 *, 4), BattleAnimSystem_GetAttacker(M2C_FIELD(temp_r0, s32 *, 4)));
    temp_r6 = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*M2C_FIELD(temp_r0, s32 **, 0xC)), 1);
    temp_r0_2 = BattleAnimSystem_GetPaletteData(M2C_FIELD(temp_r0, s32 *, 4));
    sp1C = temp_r0_2;
    PaletteData_LoadNarc_CustomTint(temp_r0_2, sp18, temp_r7, BattleAnimSystem_GetHeapID(arg0), 2, 0x20, (s32)(u16)(temp_r6 * 0x10), 0x100, 0x100, 0x100);
    ManagedSprite_SetDrawFlag(M2C_FIELD(temp_r0, s32 **, 0xC), 0);
    ManagedSprite_SetDrawFlag(M2C_FIELD(temp_r0, s32 **, 0x10), 0);
    BattleAnimSystem_CreateTaskDefaultPriority(M2C_FIELD(temp_r0, s32 *, 4), ov07_02228684, temp_r0);
}
