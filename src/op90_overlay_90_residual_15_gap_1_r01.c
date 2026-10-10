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
    u8 state;
    u8 timer;
    u16 heapId;
    u16 x4;
    u8 pad6;
    u8 x7;
    u8 x8[8];
    u8 count;
    u8 x11;
    u8 x12;
    u8 pad13[5];
    u8 x18[4];
    u8 x1C[4];
    u32 x20[4];
    Ov90Bg bg;
    u8 x3C[0x10];
    u8 x4C[0x38];
    u8 x84[0x1C8 - 0x84];
    u32 x1C8;
    u8 x1CC[0x200 - 0x1CC];
    u32 x200;
    Ov90Obj objs[4];
    u8 x2E4[4];
} Ov90Work;
extern u8 ov90_0225C1E8[];
extern u8 _0225C1E0[];
extern u8 ov90_0225C1E4[];
BOOL BeginNormalPaletteFade(int, int, int, u16, int, int, int);
BOOL IsPaletteFadeFinished(void);
void ov90_0225A350(void *, Ov90Bg *, int);
int ov90_0225A428(void *, Ov90Bg *);
void ov90_02259464(void *, void *, int, int);
int ov90_02259538(void *, int);
void ov90_022594FC(void *, int);
void ov90_0225A3E0(void *, Ov90Bg *);
int ov90_0225A544(void *, Ov90Bg *);
void ov90_0225A088(void *, Ov90Bg *, int);
int ov90_0225A258(void *, void *);
void ov90_02259BCC(Ov90Obj *, int, int, u32, Ov90Bg *, int, int, void *, u32, int);
int ov90_0225888C(void *, int);
int ov90_022588A4(void *, int);
void ov90_02259D50(Ov90Obj *, void *, u32, int, int, int);
void ov90_02259DAC(Ov90Obj *, void *, int, int, int, int);
void ov90_02259EE0(Ov90Obj *, int, s16);
void ov90_02259E8C(Ov90Obj *, Ov90Bg *);
void ov90_02259EA0(Ov90Obj *);
int ov90_02259B68(Ov90Work *);
void sub_02037AC0(int);
int sub_02037B38(int);
void WifiConnection_StopVoiceChat(void);
void ov90_02259F44(Ov90Obj *, s16);
u32 ov90_02259B38(void *, int);
void sub_02037030(int, void *, int);
void ov90_0225A108(void *);
void ov90_02259170(void *);
void ScheduleSetBgPosText(void *, int, int, int);
void ov90_02259794(SysTask *task, Ov90Work *work);
// @fn ov90_02259794
void ov90_02259794(SysTask *task, Ov90Work *work) {
    u32 msg;
    int i;
    int v;
    switch (work->state) {
    case 0:
        BeginNormalPaletteFade(3, 1, 1, 0xFFFF, 6, 1, work->heapId);
        ov90_0225A350(work->x1CC, &work->bg, work->heapId);
        work->state++;
        break;
    case 1:
        ov90_0225A428(work->x1CC, &work->bg);
        if (IsPaletteFadeFinished() == TRUE) {
            work->state++;
        }
        break;
    case 2:
        if (ov90_0225A428(work->x1CC, &work->bg) == TRUE) {
            work->state++;
        }
        break;
    case 3:
        ov90_02259464(work->x4C, work->x3C, 0, 0);
        work->state++;
        break;
    case 4:
        if (ov90_02259538(work->x4C, 0)) {
            work->state++;
            work->timer = 0x40;
        }
        break;
    case 5:
        if (work->timer != 0) {
            work->timer--;
            if (work->timer == 0) {
                ov90_022594FC(work->x4C, 0);
                ov90_0225A3E0(work->x1CC, &work->bg);
                work->state++;
            }
        }
        break;
    case 6:
        if (ov90_0225A544(work->x1CC, &work->bg) == TRUE) {
            work->state++;
        }
        break;
    case 7:
        ov90_0225A088(work->x2E4, &work->bg, work->heapId);
        v = ov90_0225A258(work->x8, work->x18);
        for (i = 0; i < work->count; i++) {
            ov90_02259BCC(&work->objs[i], work->count, work->x4, work->x200, &work->bg, i, work->x11, work->x84, work->x1C8, work->heapId);
            ov90_02259D50(&work->objs[i], work->x3C, work->x20[i], 8, 0, ov90_022588A4(work->x8, ov90_0225888C(work->x8, i)));
            ov90_02259DAC(&work->objs[i], work->x3C, v, work->x18[i], work->x1C[i], 8);
            ov90_02259EE0(&work->objs[i], -8, i * 3);
        }
        for (i = 0; i < work->count; i++) {
            ov90_02259E8C(&work->objs[i], &work->bg);
            ov90_02259EA0(&work->objs[i]);
        }
        work->timer = 0x5C;
        work->state++;
        break;
    case 8:
        if (ov90_02259B68(work) == TRUE) {
            work->state++;
        }
        break;
    case 9:
        work->timer--;
        if (work->timer == 0) {
            work->state++;
            sub_02037AC0(0x81);
            if (work->x12) {
                WifiConnection_StopVoiceChat();
            }
        }
        break;
    case 10:
        if (sub_02037B38(0x81)) {
            work->state++;
        }
        break;
    case 11:
        for (i = 0; i < work->count; i++) {
            ov90_02259F44(&work->objs[i], i * 3);
        }
        work->state++;
        work->timer = 8;
        break;
    case 12:
        work->timer--;
        ov90_02259B68(work);
        if (work->timer == 0) {
            work->state++;
        }
        break;
    case 13:
        BeginNormalPaletteFade(3, 0, 1, 0, 6, 1, work->heapId);
        ov90_02259B68(work);
        if (work->x11 == 0) {
            switch (work->x4) {
            case 0:
                msg = ov90_02259B38(ov90_0225C1E8, 3);
                break;
            case 1:
                msg = ov90_02259B38(_0225C1E0, 2);
                break;
            case 2:
                msg = ov90_02259B38(ov90_0225C1E4, 3);
                break;
            }
            sub_02037030(0x1A, &msg, 4);
        }
        work->state++;
        break;
    case 14:
        ov90_02259B68(work);
        if (IsPaletteFadeFinished() == TRUE) {
            ov90_0225A108(work->x2E4);
            work->state++;
        }
        break;
    case 15:
        if (work->x7 == 1) {
            work->state++;
        }
        break;
    case 16:
        break;
    }
    ov90_02259170(work->x84);
    ScheduleSetBgPosText(work->bg.bgConfig, 3, 4, 2);
    ScheduleSetBgPosText(work->bg.bgConfig, 5, 4, 2);
}
