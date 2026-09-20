#include "global.h"

#include "to56_r3_w06_private.h"
void ov119_0225FA2C(s32, void *);
typedef struct Vec3Raw {
    s32 x;
    s32 y;
    s32 z;
} Vec3Raw;
struct _m2c_stack_ov119_0225FA2C {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ s32 sp18;   /* inferred */
    /* 0x1C */ void *sp1C; /* inferred */
    /* 0x20 */ s32 sp20;   /* inferred */
    /* 0x24 */ void *sp24; /* inferred */
    /* 0x28 */ s16 sp28;   /* inferred */
    /* 0x2A */ char pad2A[2];
    /* 0x2C */ M2C_UNK sp2C; /* inferred */
    /* 0x2C */ char pad2C[4];
    /* 0x30 */ s32 sp30; /* inferred */
    /* 0x34 */ char pad34[4];
    /* 0x38 */ M2C_UNK sp38; /* inferred */
    /* 0x38 */ char pad38[0x20];
}; /* size = 0x58 */

M2C_UNK AddWindowParameterized(s32, s32, s32, s32, s32, s32, s32, s32, s32); /* extern */
s32 AllocWindows(s32, s32);                                                  /* extern */
M2C_UNK BG_ClearCharDataRange(s32, s32, s32, s32);                           /* extern */
M2C_UNK BG_LoadPlttData(s32, s16 *, s32, s32);                               /* extern */
M2C_UNK BgClearTilemapBufferAndCommit(s32, s32);                             /* extern */
s32 Camera_GetDistance(s32);                                                 /* extern */
M2C_UNK Camera_SetDistance(s32, s32);                                        /* extern */
M2C_UNK ClearWindowTilemapAndCopyToVram(s32);                                /* extern */
M2C_UNK FillWindowPixelBuffer(s32, s32);                                     /* extern */
M2C_UNK GfGfx_EngineATogglePlanes(s32, s32);                                 /* extern */
void *Heap_Alloc(s32, s32);                                                  /* extern */
M2C_UNK RemoveWindow(s32);                                                   /* extern */
M2C_UNK ScheduleWindowCopyToVram(s32);                                       /* extern */
M2C_UNK SpriteList_RenderAndAnimateSprites(s32);                             /* extern */
M2C_UNK Sprite_Delete(s32);                                                  /* extern */
void *Sprite_GetMatrixPtr(s32);                                              /* extern */
M2C_UNK Sprite_SetAffineOverwriteMode(s32, s32);                             /* extern */
M2C_UNK Sprite_SetAffineZRotation(s32, u16);                                 /* extern */
M2C_UNK Sprite_SetDrawFlag(s32, s32);                                        /* extern */
M2C_UNK Sprite_SetMatrix(s32, void *);                                       /* extern */
M2C_UNK WindowArray_Delete(s32, s32);                                        /* extern */
M2C_UNK ov01_021EFCDC(void *, s32);                                          /* extern */
M2C_UNK ov01_021EFCF8(s32, s32, s32, void *, s32);                           /* extern */
M2C_UNK ov01_021EFE34(void *, s32, s32, s32);                                /* extern */
M2C_UNK ov01_021EFE44(void *);                                               /* extern */
M2C_UNK ov01_021EFEC8(void *, s32, s32, s32, s32);                           /* extern */
M2C_UNK ov01_021EFF28(void *);                                               /* extern */
M2C_UNK ov01_021F05C4(void *, s32, s32);                                     /* extern */
M2C_UNK ov01_021F05F4(void *);                                               /* extern */
M2C_UNK ov01_021F0614(s32, void *, void *, s32, s32, s32, s32, s32, s32);    /* extern */
M2C_UNK ov01_021F06EC(void *, void *);                                       /* extern */
s32 ov01_021F0718(void *, void *, s32, s32, s32, s32);                       /* extern */
M2C_UNK ov01_021F074C(void *, s32, s32, s32);                                /* extern */
s32 ov01_021F0768(s32);                                                      /* extern */
M2C_UNK ov01_021F0780(s32);                                                  /* extern */
M2C_UNK ov01_021F0788(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);     /* extern */
s32 ov01_021F07E0(s32);                                                      /* extern */
M2C_UNK ov01_021F12B4(void *, s32);                                          /* extern */
M2C_UNK ov01_021F12D0(void *);                                               /* extern */
M2C_UNK ov01_021F12E8(void *, s32, s32, s32, s32, s32, s32, s32, s32);       /* extern */
M2C_UNK ov01_021F1310(void *, s32);                                          /* extern */
M2C_UNK sub_0200FC20(s32);                                                   /* extern */

void ov119_0225FA2C(s32 arg0, void *arg1) {
    Vec3Raw sp38;
    Vec3Raw sp2C;
    s16 sp28;
    void *sp24;
    s32 sp20;
    void *sp1C;
    s32 sp18;
    s32 *temp_r1;
    s32 temp_r0_3;
    s32 temp_r0_5;
    s32 var_r6;
    s32 var_r6_3;
    u32 temp_r0;
    void *temp_r0_2;
    void *temp_r0_4;
    void *var_r4;
    void *var_r5;
    void *var_r5_2;
    void *var_r5_3;
    void *var_r6_2;
    void *var_r7;

    sp18 = arg0;
    temp_r0 = M2C_FIELD(arg1, u32 *, 0);
    sp1C = arg1;
    var_r4 = M2C_FIELD(arg1, void **, 0xC);
    switch (temp_r0) { /* irregular */
    case 0:
        temp_r0_2 = Heap_Alloc(4, 0x240);
        M2C_FIELD(sp1C, void **, 0xC) = temp_r0_2;
        memset(temp_r0_2, 0, 0x240);
        var_r4 = M2C_FIELD(sp1C, void **, 0xC);
        M2C_FIELD(var_r4, s32 *, 0x220) = (s32)M2C_FIELD(M2C_FIELD(sp1C, void **, 0x10), s32 *, 0x24);
        ov01_021F12B4(var_r4 + 0x210, 4);
        M2C_FIELD(var_r4, s32 *, 0x23C) = 0xE;
        ov01_021F05C4(var_r4, 3, 1);
        ov01_021F0614(M2C_FIELD(sp1C, s32 *, 0x20), var_r4, var_r4 + 0x13C, 0, 1, 4, 6, 5, 0x927C0);
        var_r6 = 0;
        var_r5 = var_r4;
        do {
            M2C_FIELD(var_r5, s32 *, 0x170) = ov01_021F0718(var_r4, var_r4 + 0x13C, 0, 0, 0, 0);
            Sprite_SetDrawFlag(M2C_FIELD(var_r5, s32 *, 0x170), 0);
            Sprite_SetAffineOverwriteMode(M2C_FIELD(var_r5, s32 *, 0x170), 2);
            M2C_FIELD(var_r5, s32 *, 0x1F4) = ov01_021F0768(4);
            var_r6 += 1;
            var_r5 += 4;
        } while (var_r6 < 3);
        GfGfx_EngineATogglePlanes(0x10, 1);
        M2C_FIELD(var_r4, s32 *, 0x20C) = AllocWindows(4, 1);
        AddWindowParameterized(M2C_FIELD(M2C_FIELD(sp1C, void **, 0x10), s32 *, 8), M2C_FIELD(var_r4, s32 *, 0x20C), 3, 0, 0, 0x20, 0x20, 0, 0);
        sp28 = 0;
        BG_LoadPlttData(3, &sp28, 2, 0x1E);
        FillWindowPixelBuffer(M2C_FIELD(var_r4, s32 *, 0x20C), 0);
        ScheduleWindowCopyToVram(M2C_FIELD(var_r4, s32 *, 0x20C));
        M2C_FIELD(sp1C, s32 *, 0) = (s32)(M2C_FIELD(sp1C, s32 *, 0) + 1);
    default:
        goto block_29;
    case 1:
        ov01_021EFCF8(1, 0x10, -0x10, sp1C + 4, 2);
        M2C_FIELD(sp1C, s32 *, 0) = (s32)(M2C_FIELD(sp1C, s32 *, 0) + 1);
        goto block_29;
    case 2:
        M2C_FIELD(var_r4, s32 *, 0x23C) = (s32)(M2C_FIELD(var_r4, s32 *, 0x23C) - 1);
        if (M2C_FIELD(var_r4, s32 *, 0x23C) == 0) {
            ov01_021F12E8(var_r4 + 0x210, 0, 0xBF, 0x2AA, 0xC000, 0x320, 0x04000010, 0, 4);
            M2C_FIELD(var_r4, s32 *, 0x21C) = 1;
        }
        if (M2C_FIELD(sp1C, s32 *, 4) != 0) {
            M2C_FIELD(var_r4, s32 *, 0x23C) = 6;
            M2C_FIELD(sp1C, s32 *, 0) = (s32)(M2C_FIELD(sp1C, s32 *, 0) + 1);
        }
        goto block_29;
    case 3:
        M2C_FIELD(var_r4, s32 *, 0x23C) = (s32)(M2C_FIELD(var_r4, s32 *, 0x23C) - 1);
        if ((s32)M2C_FIELD(var_r4, s32 *, 0x23C) >= 0) {

        } else {
            temp_r0_5 = Camera_GetDistance(M2C_FIELD(var_r4, s32 *, 0x220));
            ov01_021EFEC8(var_r4 + 0x224, temp_r0_5, temp_r0_5 - 0x1F4000, 0xFFFF6000, 0x10);
            ov01_021EFE34(var_r4 + 0x1B8, 0, 0xFFFF, 6);
            ov01_021EFE34(var_r4 + 0x17C, 0xE7, -0x20, 6);
            ov01_021F0788(M2C_FIELD(var_r4, s32 *, 0x1F4), 0x2B, 0x2B, 0x138, 0, 6, M2C_FIELD(var_r4, s32 *, 0x20C), 0x56, 0x40, 0xF);
            ov01_021F074C(&sp38, 0x2B000, 0xE7000, 0);
            Sprite_SetMatrix(M2C_FIELD(var_r4, s32 *, 0x170), &sp38);
            Sprite_SetDrawFlag(M2C_FIELD(var_r4, s32 *, 0x170), 1);
            M2C_FIELD(var_r4, s32 *, 0x200) = 1;
            M2C_FIELD(sp1C, s32 *, 0) = (s32)(M2C_FIELD(sp1C, s32 *, 0) + 1);
            M2C_FIELD(var_r4, s32 *, 0x23C) = 4;
        }
        goto block_29;
    case 4:
        M2C_FIELD(var_r4, s32 *, 0x23C) = (s32)(M2C_FIELD(var_r4, s32 *, 0x23C) - 1);
        if ((s32)M2C_FIELD(var_r4, s32 *, 0x23C) < 0) {
            ov01_021EFE34(var_r4 + 0x190, 0xE7, -0x20, 6);
            ov01_021EFE34(var_r4 + 0x1CC, 0, 0xFFFF0001, 6);
            ov01_021F0788(M2C_FIELD(var_r4, s32 *, 0x1F8), 0xD7, 0xD7, 0x138, 0, 6, M2C_FIELD(var_r4, s32 *, 0x20C), 0x56, 0x40, 0xF);
            ov01_021F074C(&sp38, 0xD7000, 0xE7000, 1);
            Sprite_SetMatrix(M2C_FIELD(var_r4, s32 *, 0x174), &sp38);
            Sprite_SetDrawFlag(M2C_FIELD(var_r4, s32 *, 0x174), 1);
            M2C_FIELD(var_r4, s32 *, 0x204) = 1;
            M2C_FIELD(sp1C, s32 *, 0) = (s32)(M2C_FIELD(sp1C, s32 *, 0) + 1);
            M2C_FIELD(var_r4, s32 *, 0x23C) = 2;
        }
        goto block_29;
    case 5:
        M2C_FIELD(var_r4, s32 *, 0x23C) = (s32)(M2C_FIELD(var_r4, s32 *, 0x23C) - 1);
        if ((s32)M2C_FIELD(var_r4, s32 *, 0x23C) >= 0) {

        } else {
            ov01_021EFE34(var_r4 + 0x1A4, 0xE7, -0x20, 6);
            ov01_021EFE34(var_r4 + 0x1E0, 0, 0xFFFF, 6);
            ov01_021F0788(M2C_FIELD(var_r4, s32 *, 0x1FC), 0x81, 0x81, 0x138, 0, 6, M2C_FIELD(var_r4, s32 *, 0x20C), 0x56, 0x40, 0xF);
            ov01_021F074C(&sp38, 0x81000, 0xE7000, 2);
            Sprite_SetMatrix(M2C_FIELD(var_r4, s32 *, 0x178), &sp38);
            Sprite_SetDrawFlag(M2C_FIELD(var_r4, s32 *, 0x178), 1);
            M2C_FIELD(var_r4, s32 *, 0x208) = 1;
            M2C_FIELD(sp1C, s32 *, 0) = (s32)(M2C_FIELD(sp1C, s32 *, 0) + 1);
        }
        goto block_29;
    case 6:
        ov01_021EFF28(var_r4 + 0x224);
        Camera_SetDistance(M2C_FIELD(var_r4, s32 *, 0x224), M2C_FIELD(var_r4, s32 *, 0x220));
        if ((M2C_FIELD(var_r4, s32 *, 0x200) == 0) && (M2C_FIELD(var_r4, s32 *, 0x204) == 0) && (M2C_FIELD(var_r4, s32 *, 0x208) == 0)) {
            M2C_FIELD(sp1C, s32 *, 0) = (s32)(M2C_FIELD(sp1C, s32 *, 0) + 1);
        }
        goto block_29;
    case 7:
        sub_0200FC20(0);
        temp_r1 = M2C_FIELD(sp1C, s32 **, 0x14);
        if (temp_r1 != NULL) {
            *temp_r1 = 1;
        }
        ov01_021F12D0(var_r4 + 0x210);
        var_r6_3 = 0;
        M2C_FIELD(var_r4, s32 *, 0x21C) = 0;
        var_r5_3 = var_r4;
        do {
            Sprite_Delete(M2C_FIELD(var_r5_3, s32 *, 0x170));
            ov01_021F0780(M2C_FIELD(var_r5_3, s32 *, 0x1F4));
            var_r6_3 += 1;
            var_r5_3 += 4;
        } while (var_r6_3 < 3);
        ov01_021F06EC(var_r4, var_r4 + 0x13C);
        ov01_021F05F4(var_r4);
        ClearWindowTilemapAndCopyToVram(M2C_FIELD(var_r4, s32 *, 0x20C));
        RemoveWindow(M2C_FIELD(var_r4, s32 *, 0x20C));
        WindowArray_Delete(M2C_FIELD(var_r4, s32 *, 0x20C), 1);
        BG_ClearCharDataRange(3, 0x20, 0, 4);
        BgClearTilemapBufferAndCommit(M2C_FIELD(M2C_FIELD(sp1C, void **, 0x10), s32 *, 8), 3);
        ov01_021EFCDC(sp1C, sp18);
        return;
    }
block_29:
    if (M2C_FIELD(var_r4, s32 *, 0x21C) == 1) {
        ov01_021F1310(var_r4 + 0x210, 2);
    }
    sp20 = 0;
    var_r5_2 = var_r4;
    sp24 = var_r4 + 0x17C;
    var_r7 = var_r4 + 0x1B8;
    var_r6_2 = var_r4;
    do {
        if (M2C_FIELD(var_r5_2, s32 *, 0x200) == 1) {
            if (ov01_021F07E0(M2C_FIELD(var_r5_2, s32 *, 0x1F4)) != 0) {
                M2C_FIELD(var_r5_2, s32 *, 0x200) = 0;
            }
            ov01_021EFE44(sp24);
            ov01_021EFE44(var_r7);
            Sprite_SetAffineZRotation(M2C_FIELD(var_r5_2, s32 *, 0x170), (u16)M2C_FIELD(var_r6_2, s32 *, 0x1B8));
            temp_r0_4 = Sprite_GetMatrixPtr(M2C_FIELD(var_r5_2, s32 *, 0x170));
            sp2C = *(Vec3Raw *)temp_r0_4;
            sp2C.y = M2C_FIELD(var_r6_2, s32 *, 0x17C) << 0xC;
            Sprite_SetMatrix(M2C_FIELD(var_r5_2, s32 *, 0x170), &sp2C);
        }
        var_r5_2 += 4;
        sp24 += 0x14;
        var_r7 += 0x14;
        temp_r0_3 = sp20 + 1;
        var_r6_2 += 0x14;
        sp20 = temp_r0_3;
    } while (temp_r0_3 < 3);
    ScheduleWindowCopyToVram(M2C_FIELD(var_r4, s32 *, 0x20C));
    if (M2C_FIELD(sp1C, s32 *, 0) != 7) {
        SpriteList_RenderAndAnimateSprites(M2C_FIELD(var_r4, s32 *, 0));
    }
    return;
}
