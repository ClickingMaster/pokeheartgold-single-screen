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
s32 ov07_0221BFD0(s32);                                                                    /* extern */
M2C_UNK ov07_0221C410(s32, void (*)(s32, void *), void *);                                 /* extern */
M2C_UNK ov07_0221C448(s32, s32);                                                           /* extern */
s32 ov07_0221C468(s32);                                                                    /* extern */
s32 ov07_0221C470(s32);                                                                    /* extern */
s32 ov07_0221C4A0(s32);                                                                    /* extern */

extern u32 ov07_0221C4A8(void *p, int i);                                                               /* extern */
s32 *ov07_0221C4E8(s32, s32);                                                              /* extern */
s32 ov07_0221C514(s32);                                                                    /* extern */
s32 ov07_0221C768(s32, s32);                                                               /* extern */
u16 ov07_0221E6C8(s32);                                                                    /* extern */
s32 ov07_0221FA48(s32, s32);                                                               /* extern */
s32 ov07_0221FA78(s32);                                                                    /* extern */
s32 ov07_0221FA80(s32, s32);                                                               /* extern */
s32 ov07_0221FA90(s32, s32);                                                               /* extern */
s32 ov07_0221FAA0(s32, s32);                                                               /* extern */
s32 ov07_0221FAB0(s32);                                                                    /* extern */
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
M2C_UNK ov07_02231E08(s32, s32, s32);                                                      /* extern */
M2C_UNK ov07_02231E44(s32, s32, s32, s32);                                                 /* extern */
M2C_UNK ov07_02231FA0(s32, M2C_UNK *);                                                     /* extern */
M2C_UNK ov07_02231FE4(s32, void *);                                                        /* extern */
M2C_UNK ov07_02232020(s32, s32, void *, void *);                                           /* extern */
void *ov07_022324D8(s32, s32);                                                             /* extern */
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

    temp_r0 = ov07_022324D8(arg0, 0x14);
    M2C_FIELD(temp_r0, s32 *, 4) = arg0;
    M2C_FIELD(temp_r0, s32 *, 8) = ov07_0221C514(arg0);
    M2C_FIELD(temp_r0, s32 **, 0xC) = ov07_0221C4E8(M2C_FIELD(temp_r0, s32 *, 4), 0);
    M2C_FIELD(temp_r0, s32 **, 0x10) = ov07_0221C4E8(M2C_FIELD(temp_r0, s32 *, 4), 1);
    M2C_FIELD((void *)0x04000000, s32 *, 0) = (s32)((0x04000000U >> 0xB) | (M2C_FIELD((void *)0x04000000, s32 *, 0) & 0xFFFF1FFF));
    M2C_FIELD((void *)0x04000000, u16 *, 0x4A) = (u16)(0x1F | (M2C_FIELD((void *)0x04000000, u16 *, 0x4A) & ~0x3F));
    M2C_FIELD((void *)0x04000000, u16 *, 0x4A) = (u16)(0x1200 | (M2C_FIELD((void *)0x04000000, u16 *, 0x4A) & 0xFFFFC0FF));
    temp_r7 = ov07_0221FA80(M2C_FIELD(temp_r0, s32 *, 4), ov07_0221C468(M2C_FIELD(temp_r0, s32 *, 4)));
    sp18 = ov07_0221FA90(M2C_FIELD(temp_r0, s32 *, 4), ov07_0221C468(M2C_FIELD(temp_r0, s32 *, 4)));
    temp_r6 = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*M2C_FIELD(temp_r0, s32 **, 0xC)), 1);
    temp_r0_2 = ov07_0221FA78(M2C_FIELD(temp_r0, s32 *, 4));
    sp1C = temp_r0_2;
    PaletteData_LoadNarc_CustomTint(temp_r0_2, sp18, temp_r7, ov07_0221BFD0(arg0), 2, 0x20, (s32)(u16)(temp_r6 * 0x10), 0x100, 0x100, 0x100);
    ManagedSprite_SetDrawFlag(M2C_FIELD(temp_r0, s32 **, 0xC), 0);
    ManagedSprite_SetDrawFlag(M2C_FIELD(temp_r0, s32 **, 0x10), 0);
    ov07_0221C410(M2C_FIELD(temp_r0, s32 *, 4), ov07_02228684, temp_r0);
}
