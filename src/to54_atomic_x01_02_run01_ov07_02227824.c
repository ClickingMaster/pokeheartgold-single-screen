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
s32 SpriteSystem_NewSprite(s32, s32, M2C_UNK *);                                           /* extern */
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
M2C_UNK ov07_0221F9E8(M2C_UNK *, s32);                                                     /* extern */
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

void ov07_02227824(s32 arg0) {
    s32 temp_r0_2;
    s32 temp_r6;
    void *temp_r0;

    temp_r0 = Heap_Alloc(ov07_0221BFD0(arg0), 0x5C);
    M2C_FIELD(temp_r0, s8 *, 8) = 0;
    M2C_FIELD(temp_r0, s8 *, 0) = 0;
    M2C_FIELD(temp_r0, s32 *, 0xC) = arg0;
    temp_r0_2 = ov07_0221FA48(M2C_FIELD(temp_r0, s32 *, 0xC), ov07_0221C468(arg0));
    M2C_FIELD(temp_r0, s32 *, 0x10) = temp_r0_2;
    M2C_FIELD(temp_r0, s16 *, 2) = Pokepic_GetAttr(temp_r0_2, 1);
    M2C_FIELD(temp_r0, s32 *, 4) = ov07_0221FAA0(M2C_FIELD(temp_r0, s32 *, 0xC), ov07_0221C468(M2C_FIELD(temp_r0, s32 *, 0xC)));
    temp_r6 = ov07_0221C4A8(arg0, 0);
    ov07_02222508(temp_r0 + 0x14, 0x64, 0x64, (s16)temp_r6, ov07_0221C4A8(arg0, 2));
    ov07_022227A8(temp_r0 + 0x38, 2, 0, 0, 0xA);
    ov07_0221C410(M2C_FIELD(temp_r0, s32 *, 0xC), ov07_02227630, temp_r0);
}
