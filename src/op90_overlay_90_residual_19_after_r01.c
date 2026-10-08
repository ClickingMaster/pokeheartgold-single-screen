#pragma force_active on
#include "global.h"
typedef struct SysTask SysTask;
typedef struct Ov90Bg {
    void *bgConfig;
    u8 pad4[8];
} Ov90Bg;
typedef struct Ov90Obj {
    u8 data[0x38];
} Ov90Obj;
typedef struct Ov90Work {
    u16 timer;
    u16 heapId;
    u8 state;
    u8 x5;
    u8 done;
    u8 x7;
    u8 x8;
    u8 x9;
    u8 padA[2];
    u8 xC[8];
    u8 count;
    u8 x15;
    u8 x16;
    u8 x17;
    u32 *x18;
    u8 pad1C[0x30 - 0x1C];
    u32 x30;
    u8 x34[4];
    u8 x38[4];
    u32 x3C[4];
    Ov90Bg bg;
    u8 x58[0x10];
    u8 x68[0x38];
    u8 xA0[0x1E4 - 0xA0];
    u32 x1E4;
    u8 x1E8[0x208 - 0x1E8];
    u8 x208[0x258 - 0x208];
    u32 x258;
    Ov90Obj objs[4];
    u8 x33C[0x650 - 0x33C];
    u16 x650;
} Ov90Work;
BOOL BeginNormalPaletteFade(int, int, int, u16, int, int, int);
BOOL IsPaletteFadeFinished(void);
u32 ov90_0225A28C(u32);
void ov90_0225BAA0(Ov90Work *);
void ov90_02259464(void *, void *, int, int);
int ov90_02259538(void *, int);
void ov90_022594FC(void *, int);
void ov90_0225A088(void *, Ov90Bg *, int);
void ov90_02259BCC(Ov90Obj *, int, int, u32, Ov90Bg *, int, int, void *, u32, int);
int ov90_0225888C(void *, int);
int ov90_022588A4(void *, int);
void ov90_02259D50(Ov90Obj *, void *, u32, int, int, int);
void ov90_02259DAC(Ov90Obj *, void *, int, int, int, int);
void ov90_02259EE0(Ov90Obj *, int, s16);
void ov90_02259E8C(Ov90Obj *, Ov90Bg *);
void ov90_02259EA0(Ov90Obj *);
int ov90_0225B8F0(Ov90Work *);
void ov90_0225B6B0(void *, u32);
int ov90_0225B6C4(void *, Ov90Bg *);
void ov90_0225B954(Ov90Work *);
void ov90_02259250(void *, u32);
int ov90_0225B978(Ov90Work *);
void ov00_021E6A4C(void);
void sub_02037AC0(int);
int sub_02037B38(int);
void ov90_0225B9A8(Ov90Work *);
int ov90_0225B38C(void *, void *, void *, int, int);
u8 ov90_0225B538(void *);
void ov90_0225A108(void *);
void ov90_0225B2A8(Ov90Work *);
void ov90_02259170(void *);
void ScheduleSetBgPosText(void *, int, int, int);
void ov90_0225AE4C(SysTask *task, Ov90Work *work);
// @fn ov90_0225AE4C
void ov90_0225AE4C(SysTask *task, Ov90Work *work) {
    int i;
    switch (work->state) {
    case 0:
        if (work->x17 == 1) {
            work->x9 = FALSE;
        } else {
            if (work->x30 >= 10) {
                *work->x18 = ov90_0225A28C(*work->x18);
                work->x9 = TRUE;
            } else {
                work->x9 = FALSE;
            }
            if (work->x15 == 0) {
                ov90_0225BAA0(work);
            }
        }
        BeginNormalPaletteFade(3, 1, 1, 0xFFFF, 6, 1, work->heapId);
        work->state++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            work->state++;
            work->x650 = 4;
        }
        break;
    case 2:
        ov90_02259464(work->x68, work->x58, 1, 0);
        work->state++;
        break;
    case 3:
        if (ov90_02259538(work->x68, 0)) {
            work->timer = 0x66;
            work->state++;
        }
        break;
    case 4:
        if (work->timer != 0) {
            work->timer--;
            if (work->timer == 0) {
                ov90_022594FC(work->x68, 0);
                work->state++;
            }
        }
        break;
    case 5:
        ov90_0225A088(work->x33C, &work->bg, work->heapId);
        for (i = 0; i < work->count; i++) {
            ov90_02259BCC(&work->objs[i], work->count, work->x7, work->x258, &work->bg, i, work->x15, work->xA0, work->x1E4, work->heapId);
            ov90_02259D50(&work->objs[i], work->x58, work->x3C[i], 8, 0, ov90_022588A4(work->xC, ov90_0225888C(work->xC, i)));
            ov90_02259DAC(&work->objs[i], work->x58, work->x5, work->x34[i], work->x38[i], 8);
            ov90_02259E8C(&work->objs[i], &work->bg);
            ov90_02259EA0(&work->objs[i]);
            ov90_02259EE0(&work->objs[i], -8, i * 3);
        }
        work->state++;
        work->timer = 0x10;
        break;
    case 6:
        if (ov90_0225B8F0(work) == TRUE) {
            work->timer--;
            if (work->timer == 0) {
                ov90_0225B6B0(work->x208, work->x30);
                work->state++;
            }
        }
        break;
    case 7:
        if (ov90_0225B6C4(work->x208, &work->bg) == TRUE) {
            if (work->x9 == TRUE) {
                ov90_0225B954(work);
            }
            work->state++;
        }
        break;
    case 8:
        ov90_02259250(work->x58, work->x30);
        ov90_02259464(work->x68, work->x58, 7, 0);
        work->timer = 0x66;
        work->state++;
        break;
    case 9:
        if (ov90_02259538(work->x68, 0) == TRUE) {
            if (work->timer == 0) {
                if (ov90_0225B978(work) == TRUE) {
                    if (work->x17 == 1) {
                        work->timer = 0x66;
                        work->state = 12;
                    } else {
                        work->state++;
                    }
                }
            } else {
                work->timer--;
            }
        }
        break;
    case 10:
        ov90_02259464(work->x68, work->x58, work->x9 ? 8 : 9, 0);
        work->state++;
        break;
    case 11:
        if (ov90_02259538(work->x68, 0)) {
            work->timer = 0x66;
            if (work->x9) {
                work->state = 13;
            } else {
                work->state++;
            }
        }
        break;
    case 12:
        if (work->timer != 0) {
            work->timer--;
            if (work->timer == 0) {
                work->state = 14;
            }
        }
        break;
    case 13:
        if (work->timer != 0) {
            work->timer--;
        }
        if (work->timer == 0) {
            work->state = 14;
        }
        break;
    case 14:
        if (work->x16) {
            ov00_021E6A4C();
        }
        sub_02037AC0(0x82);
        work->state++;
        break;
    case 15:
        if (sub_02037B38(0x82)) {
            if (work->x17) {
                work->state = 18;
            } else {
                work->state++;
            }
        }
        break;
    case 16:
        BeginNormalPaletteFade(3, 0, 0, 0, 6, 1, work->heapId);
        work->state++;
        break;
    case 17:
        if (IsPaletteFadeFinished()) {
            work->state = 20;
        }
        break;
    case 18:
        ov90_0225B9A8(work);
        work->state++;
        break;
    case 19:
        if (ov90_0225B38C(work->x1E8, work->x68, work->x58, work->x7, work->heapId)) {
            work->x8 = ov90_0225B538(work->x1E8);
            work->state++;
        }
        break;
    case 20:
        ov90_0225A108(work->x33C);
        work->done = TRUE;
        break;
    }
    ov90_0225B2A8(work);
    ov90_02259170(work->xA0);
    ScheduleSetBgPosText(work->bg.bgConfig, 3, 4, 2);
    ScheduleSetBgPosText(work->bg.bgConfig, 5, 4, 2);
}
