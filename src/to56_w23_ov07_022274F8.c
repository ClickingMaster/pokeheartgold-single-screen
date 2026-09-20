#include "global.h"
#define M2C_UNK                 int
#define ROTATE_RIGHT(v, n)      (((u32)(v) >> (n)) | ((u32)(v) << (32 - (n))))
#define M2C_FIELD(x, type, off) (*(type)((u8 *)(x) + (off)))

void *Heap_Alloc(s32, s32);                                                                /* extern */
M2C_UNK Heap_Free(void *);                                                                 /* extern */
s32 ManagedSprite_GetDrawFlag(s32);                                                        /* extern */
M2C_UNK ManagedSprite_GetPositionXY(s32, void *, M2C_UNK *);                               /* extern */
M2C_UNK ManagedSprite_OffsetPositionXY(s32, s32, s16);                                     /* extern */
M2C_UNK ManagedSprite_SetAffineOverwriteMode(s32, s32);                                    /* extern */
M2C_UNK ManagedSprite_SetAffineScale(s32, s32, s32);                                       /* extern */
M2C_UNK ManagedSprite_SetDrawFlag(s32, s32);                                               /* extern */
M2C_UNK ManagedSprite_SetDrawPriority(s32 *, s32);                                         /* extern */
M2C_UNK ManagedSprite_SetOamMode(s32, s32);                                                /* extern */
M2C_UNK ManagedSprite_SetPositionXY(s32, s16, s16);                                        /* extern */
M2C_UNK ManagedSprite_SetPriority(s32 *, s32);                                             /* extern */
M2C_UNK ManagedSprite_TickFrame(s32);                                                      /* extern */
u16 ObjPlttTransfer_GetPaletteVramOffset(s32, s32);                                        /* extern */
M2C_UNK PaletteData_LoadNarc_CustomTint(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32); /* extern */
M2C_UNK Pokepic_AddAttr(s32, s32, s16);                                                    /* extern */
s16 Pokepic_GetAttr(s32, s32);                                                             /* extern */
s32 Pokepic_ResumePaletteFade(s32);                                                        /* extern */
M2C_UNK Pokepic_SetAttr(s32, s32, s16);                                                    /* extern */
M2C_UNK Pokepic_StartPaletteFade(s32, s32, s32, s32, s32);                                 /* extern */
M2C_UNK SpriteSystem_DrawSprites(s32);                                                     /* extern */
s32 SpriteSystem_NewSprite(s32, s32, void *);                                              /* extern */
M2C_UNK Sprite_DeleteAndFreeResources(s32);                                                /* extern */
s32 Sprite_GetPaletteProxy(s32);                                                           /* extern */
M2C_UNK Sprite_TickFrame(s32);                                                             /* extern */
s32 ov07_0221BFC0(s32);                                                                    /* extern */
s32 ov07_0221BFD0(s32);                                                                    /* extern */
M2C_UNK ov07_0221C3F4(s32, void (*)(s32, void *), void *, s32);                            /* extern */
M2C_UNK ov07_0221C410(s32, void (*)(s32, void *), void *);                                 /* extern */
M2C_UNK ov07_0221C448(s32, s32);                                                           /* extern */
s32 ov07_0221C468(s32);                                                                    /* extern */
s32 ov07_0221C470(s32);                                                                    /* extern */
s32 ov07_0221C4A8(s32, s32);                                                               /* extern */
s32 ov07_0221C4E8(s32, s32);                                                               /* extern */
s32 ov07_0221C514(s32);                                                                    /* extern */
M2C_UNK ov07_0221F9E8(void *, s32);                                                        /* extern */
s32 ov07_0221FA48(s32, s32);                                                               /* extern */
s32 ov07_0221FA78(s32);                                                                    /* extern */
s32 ov07_0221FA80(s32, s32);                                                               /* extern */
s32 ov07_0221FA90(s32, s32);                                                               /* extern */
s32 ov07_0221FAA0(s32, s32);                                                               /* extern */
s32 ov07_0221FAE8(s32);                                                                    /* extern */
M2C_UNK ov07_02222268(void *, s16, s16, s32, s32);                                         /* extern */
s32 ov07_022222B4(void *);                                                                 /* extern */
M2C_UNK ov07_02222508(void *, s16, s32, s16, s32);                                         /* extern */
s32 ov07_02222558(void *);                                                                 /* extern */
M2C_UNK ov07_02222590(void *, s32, s32, s32, s32, s32, s32);                               /* extern */
s32 ov07_0222260C(void *);                                                                 /* extern */
M2C_UNK ov07_02222644(void *, s32 *, s32 *);                                               /* extern */
s32 ov07_02222674(s16, s32, s32);                                                          /* extern */
M2C_UNK ov07_022227A8(void *, s32, s32, s32, s32);                                         /* extern */
M2C_UNK ov07_022227D8(void *);                                                             /* extern */
s32 ov07_0223192C(s32, s32);                                                               /* extern */
M2C_UNK ov07_02231A20(s32, s32, M2C_UNK *);                                                /* extern */
M2C_UNK ov07_02231E08(s32, s32, s32);                                                      /* extern */
void ov07_02227300(s32 arg0, void *arg1);                                                  /* static */
void ov07_02227630(s32 arg0, void *arg1);                                                  /* static */
void ov07_022278B0(s32 arg0, void *arg1);                                                  /* static */
void ov07_02227B58(s32 arg0, void *arg1);                                                  /* static */
void ov07_022274F8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ov07_02227824(s32 arg0);
void ov07_02227AEC(s32 arg0);
void ov07_02227D1C(s32 arg0);
void ov07_02227FA4(s32 arg0, void *arg1);
extern M2C_UNK ov07_022366CC;
extern M2C_UNK ov07_022366CD;
extern M2C_UNK ov07_022366CE;
extern M2C_UNK ov07_022366D2;
extern M2C_UNK ov07_02236732;
extern M2C_UNK ov07_02236734;
extern M2C_UNK ov07_02236736;

void ov07_022274F8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 sp8[0x34];
    M2C_UNK sp4;
    s32 sp0;
    s32 temp_r0_2;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r7;
    void *temp_r0;
    void *var_r5;
    void *var_r5_2;

    sp0 = arg0;
    temp_r0 = Heap_Alloc(ov07_0221BFD0(arg0), 0x44);
    if (temp_r0 == NULL) {
        GF_AssertFail();
    }
    M2C_FIELD(temp_r0, s8 *, 4) = 0;
    M2C_FIELD(temp_r0, s8 *, 0) = 0;
    M2C_FIELD(temp_r0, s32 *, 0xC) = arg1;
    M2C_FIELD(temp_r0, s32 *, 0x10) = arg2;
    M2C_FIELD(temp_r0, s32 *, 8) = sp0;
    ov07_0221F9E8(&sp8, sp0);
    ov07_02231E08(M2C_FIELD(temp_r0, s32 *, 8), -1, -1);
    M2C_FIELD(temp_r0, u8 *, 5) = 0xFU;
    M2C_FIELD(temp_r0, u8 *, 6) = 0U;
    *(s16 *)0x04000052 = M2C_FIELD(temp_r0, u8 *, 5) | (M2C_FIELD(temp_r0, u8 *, 6) << 8);
    M2C_FIELD(temp_r0, s32 *, 0x14) = arg3;
    if (ov07_0223192C(sp0, ov07_0221C470(sp0)) == 3) {
        ov07_02231A20(0, ov07_0221BFC0(sp0), &sp4);
    } else {
        ov07_02231A20(1, ov07_0221BFC0(sp0), &sp4);
    }
    var_r6 = 1;
    if (ov07_0221C4A8(M2C_FIELD(temp_r0, s32 *, 8), 0) > 1) {
        var_r5 = temp_r0 + 4;
        do {
            temp_r0_2 = SpriteSystem_NewSprite(M2C_FIELD(temp_r0, s32 *, 0xC), M2C_FIELD(temp_r0, s32 *, 0x10), &sp8);
            M2C_FIELD(var_r5, s32 *, 0x14) = temp_r0_2;
            ManagedSprite_SetPositionXY(temp_r0_2, M2C_FIELD(&sp4, s16 *, 0), M2C_FIELD(&sp4, s16 *, 2));
            var_r5 += 4;
            var_r6 += 1;
        } while (var_r6 < ov07_0221C4A8(M2C_FIELD(temp_r0, s32 *, 8), 0));
    }
    ManagedSprite_SetPositionXY(M2C_FIELD(temp_r0, s32 *, 0x14), M2C_FIELD(&sp4, s16 *, 0), M2C_FIELD(&sp4, s16 *, 2));
    var_r6_2 = 0;
    if (ov07_0221C4A8(M2C_FIELD(temp_r0, s32 *, 8), 0) > 0) {
        var_r5_2 = temp_r0;
        var_r7 = 0;
        do {
            M2C_FIELD(temp_r0 + var_r6_2, s8 *, 1) = 0;
            ManagedSprite_SetAffineOverwriteMode(M2C_FIELD(var_r5_2, s32 *, 0x14), 2);
            ManagedSprite_OffsetPositionXY(M2C_FIELD(var_r5_2, s32 *, 0x14), 0, (s16)(0x20 - var_r7));
            ManagedSprite_SetOamMode(M2C_FIELD(var_r5_2, s32 *, 0x14), 1);
            var_r5_2 += 4;
            var_r7 += 4;
            var_r6_2 += 1;
        } while (var_r6_2 < ov07_0221C4A8(M2C_FIELD(temp_r0, s32 *, 8), 0));
    }
    ov07_0221C3F4(sp0, ov07_02227300, temp_r0, 0x1000);
}
