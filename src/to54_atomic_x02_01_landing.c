#include "global.h"

#pragma require_prototypes off

typedef struct Ov07CallbackWork {
    u8 pad00[8];
    s32 state;
    s32 unk0C;
    s32 unk10;
    u8 pad14[0x28 - 0x14];
    s32 done;
    u8 pad2C[4];
    void *sprite;
    s32 rotation;
    u8 pad38[0x48 - 0x38];
    u8 unk48[0x24];
    u8 unk6C[0x24];
    s32 unk90;
    void *unk94;
    u8 pad98[8];
    s32 unkA0;
    u8 padA4[0xB0 - 0xA4];
    void *unkB0;
    u8 padB4[4];
    s16 x;
    s16 y;
    s16 unkBC;
    s16 unkBE;
    s32 unkC0;
    s32 counter;
    s32 unkC8;
    s32 unkCC;
    void *unkD0;
    void *unkD4;
} Ov07CallbackWork;

typedef struct Ov07EffectArgs {
    s16 x;
    s16 y;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
} Ov07EffectArgs;

extern void ov07_022344C0(Ov07CallbackWork *, int);
extern void ManagedSprite_SetAnim(void *, int);
extern void ManagedSprite_SetAnimationFrame(void *, int);
extern void ManagedSprite_GetPositionXY(void *, s16 *, s16 *);
extern void *ov07_02232D20(Ov07EffectArgs *);
extern void ov07_02232D80(void *);
extern BOOL ov07_02232DF4(void *);
extern void ov07_02232E10(void *);
extern void ov07_022223F0(s32 *, s32, s32, int);
extern void ManagedSprite_SetAffineZRotation(void *, u16);
extern BOOL ov07_02222440(s32 *);
extern void ov07_02222338(void *, void *, s16, s16, s16, s16, u16, s32);
extern BOOL ov07_02232540(s32);
extern BOOL ov07_02232580(s32);
extern void ManagedSprite_OffsetAffineZRotation(void *, s32);
extern int LCRandom(void);
extern int ManagedSprite_GetPaletteOverrideOffset(void *);
extern void *ov07_02222F10(void *, void *, int, int, int, int, int, int, int, int, int);
extern BOOL ov07_022223CC(void *, void *, void *);
extern void ov07_022344B4(Ov07CallbackWork *, int);
extern BOOL ov07_02222EE8(void *);
extern void ov07_02222EF8(void *);
extern void PlaySE_SetPan(int, int);

BOOL ov07_022335B4(Ov07CallbackWork *work) {
    switch (work->state) {
    case 0:
        ov07_02222338(work->unk48, work->unk6C, work->x, work->unkBC, work->y, work->unkBE, work->unkC0, work->unkC8 << 12);
        work->counter = 0;
        work->state++;
        work->unkD4 = NULL;
        if (ov07_02232540(work->unk90) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(work->sprite, (LCRandom() % 20 + 10) << 13);
        }
        break;
    case 1:
        if (ov07_02232540(work->unk90) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(work->sprite, 2 << 12);
            if (work->counter > work->unkC0 / 2 + 10) {
                ManagedSprite_OffsetAffineZRotation(work->sprite, 2 << 12);
            }
            if (ov07_02232580(work->unk90) == TRUE && work->counter == work->unkC0 / 2 + 10) {
                work->unkD4 = ov07_02222F10(work->unkB0, work->unk94, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(work->sprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            }
        }
        work->counter++;
        if (work->unk90 < 6 || work->unk90 > 8 || work->counter <= work->unkC0 / 2 || work->counter >= work->unkC0 / 2 + 5) {
            if (ov07_022223CC(work->unk48, work->unk6C, work->sprite) == FALSE) {
                ov07_022344B4(work, 0);
                work->state++;
            }
        }
        break;
    case 2:
        if (ov07_02232580(work->unk90) == TRUE && work->unkD4 != NULL) {
            if (ov07_02222EE8(work->unkD4) == TRUE) {
                break;
            }
            ov07_02222EF8(work->unkD4);
            work->done = TRUE;
            return FALSE;
        }
        if (work->unk90 >= 15) {
            work->done = TRUE;
            return FALSE;
        }
        if (work->unkA0 == 0x401) {
            PlaySE_SetPan(0x7E8, 0x75);
        }
        work->state++;
        break;
    case 3:
        ManagedSprite_SetAnim(work->sprite, 1);
        ov07_022344C0(work, 1);
        work->counter = 0;
        work->state++;
        break;
    default:
        if (work->counter == 5) {
            if (work->unk90 != 12 && work->unk90 != 13) {
                work->unkD4 = ov07_02222F10(work->unkB0, work->unk94, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(work->sprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            }
            work->done = TRUE;
        }
        work->counter++;
        if (work->counter > 15) {
            if (work->unk90 != 12 && work->unk90 != 13) {
                if (ov07_02222EE8(work->unkD4) == TRUE) {
                    break;
                }
                ov07_02222EF8(work->unkD4);
            }
            return FALSE;
        }
        break;
    }
    return TRUE;
}

BOOL ov07_02233874(Ov07CallbackWork *work) {
    switch (work->state) {
    case 0:
        ov07_02222338(work->unk48, work->unk6C, work->x, work->unkBC, work->y, work->unkBE, work->unkC0, work->unkC8 << 12);
        work->counter = 0;
        work->state++;
        work->unkD4 = NULL;
        if (ov07_02232540(work->unk90) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(work->sprite, (LCRandom() % 20 + 10) << 13);
        }
        break;
    case 1:
        if (ov07_02232540(work->unk90) == TRUE) {
            ManagedSprite_OffsetAffineZRotation(work->sprite, 2 << 12);
            if (work->counter > work->unkC0 / 2 + 10) {
                ManagedSprite_OffsetAffineZRotation(work->sprite, 2 << 12);
            }
            if (ov07_02232580(work->unk90) == TRUE && work->counter == work->unkC0 / 2 + 10) {
                work->unkD4 = ov07_02222F10(work->unkB0, work->unk94, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(work->sprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            }
        }
        work->counter++;
        if (work->unk90 < 6 || work->unk90 > 8 || work->counter <= work->unkC0 / 2 || work->counter >= work->unkC0 / 2 + 5) {
            if (ov07_022223CC(work->unk48, work->unk6C, work->sprite) == FALSE) {
                ov07_022344B4(work, 0);
                work->state++;
            }
        }
        break;
    case 2:
        if (ov07_02232580(work->unk90) == TRUE && work->unkD4 != NULL) {
            if (ov07_02222EE8(work->unkD4) == TRUE) {
                break;
            }
            ov07_02222EF8(work->unkD4);
            work->done = TRUE;
            return FALSE;
        }
        if (work->unk90 >= 15) {
            work->done = TRUE;
            return FALSE;
        }
        work->state++;
        break;
    case 3:
        ManagedSprite_SetAnim(work->sprite, 1);
        ov07_022344C0(work, 0);
        work->counter = 0;
        work->state++;
        break;
    default:
        if (work->counter == 5) {
            ManagedSprite_SetAnimationFrame(work->sprite, 2);
            work->unkD4 = ov07_02222F10(work->unkB0, work->unk94, 2, (u16)(ManagedSprite_GetPaletteOverrideOffset(work->sprite) << 4), 16, -2, 2, 0, 14, 0xFFFF, 1002);
            work->done = TRUE;
        }
        work->counter++;
        if (work->counter > 15) {
            if (ov07_02222EE8(work->unkD4) != TRUE) {
                ov07_02222EF8(work->unkD4);
                return FALSE;
            }
        }
        break;
    }
    return TRUE;
}

BOOL ov07_02233B04(Ov07CallbackWork *work) {
    Ov07EffectArgs args;

    switch (work->state) {
    case 0:
        ov07_022344C0(work, 0);
        ManagedSprite_SetAnim(work->sprite, 1);
        work->counter++;
        if (work->counter >= 2) {
            ManagedSprite_SetAnimationFrame(work->sprite, 0);
            work->counter = 0;
            work->state++;
        }
        break;
    case 1:
        work->counter++;
        if (work->counter >= 2) {
            ManagedSprite_SetAnimationFrame(work->sprite, 2);
            work->counter = 0;
            work->state++;
        }
        break;
    case 2:
        args.unk04 = work->unkA0;
        args.unk08 = (s32)work->unk94;
        args.unk0C = 0xFF;
        args.unk10 = 1;
        ManagedSprite_GetPositionXY(work->sprite, &args.x, &args.y);
        work->unkD0 = ov07_02232D20(&args);
        work->state++;
        break;
    case 3:
        ov07_02232D80(work->unkD0);
        work->state++;
        break;
    case 4:
        work->counter++;
        if (work->counter >= 6) {
            ManagedSprite_SetAnimationFrame(work->sprite, 2);
            work->counter = 0;
            work->state++;
        }
        break;
    case 5:
        work->counter++;
        if (work->counter >= 6) {
            ManagedSprite_SetAnimationFrame(work->sprite, 1);
            work->counter = 0;
            work->state++;
        }
        break;
    case 6:
        work->counter++;
        if (work->counter >= 6) {
            ManagedSprite_SetAnimationFrame(work->sprite, 0);
            work->counter = 0;
            work->state++;
        }
        break;
    case 7:
        if (!ov07_02232DF4(work->unkD0)) {
            ov07_02232E10(work->unkD0);
            work->state++;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL ov07_02233C98(Ov07CallbackWork *work) {
    switch (work->state) {
    case 0:
        if (work->unk10 == 0) {
            ov07_022223F0(&work->rotation, -0x1FFF, 0x1FFF, 10);
        } else {
            ov07_022223F0(&work->rotation, 0x1FFF, -0x1FFF, 10);
        }
        work->unk10 ^= 1;
        work->state++;
        break;
    case 1:
        ManagedSprite_SetAffineZRotation(work->sprite, work->rotation);
        if (!ov07_02222440(&work->rotation)) {
            if (work->unk0C >= 1) {
                work->state++;
            } else {
                work->unk0C++;
                work->state--;
            }
        }
        break;
    default:
        work->counter = 0;
        return FALSE;
    }
    return TRUE;
}
