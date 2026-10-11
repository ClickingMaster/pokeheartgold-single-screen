#pragma force_active on
#include "global.h"
#include "system.h"
#include "sound_playback.h"
#include "unk_0200FA24.h"

typedef struct {
    void *unk0;
    void *unk4;
} Ov47Arg;

typedef struct {
    int id;
    int unk4;
    int flag;
    u8 unkC[0x14];
} Ov47Record;

typedef struct {
    u8 unk0[0x60];
    u16 state;
    u16 nextState;
    u8 unk64[0x88 - 0x64];
    Ov47Record rec1;
    Ov47Record rec2;
    u8 unkC8[0x14];
    u8 unkDC[0x14];
} Ov47Work;

void *ov47_02258CEC(void *a0, int a1, int a2);
void ov47_022592B4(void *a0, void *a1);
void ov47_02259404(void *a0, void *a1, enum HeapID a2);
u32 ov47_02259430(void *a0, enum HeapID a1);
void *ov47_02259D40(Ov47Record *rec, void *msg);
void ov47_02259318(void *a0, void *a1);
void ov47_022593CC(void *a0);
int ov47_022593B4(void *a0);
void *ov45_0222A5C0(void *a0);
void ov45_0222AB0C(void *a0, u32 *a1);
int ov45_0222A53C(void *a0);
int ov45_0222AB28(void *a0, int a1);
void *sub_020318E8(void *a0);
u32 sub_020318F4(void *a0);
u32 sub_020318F8(void *a0);
void ov47_022599F0(Ov47Work *work, Ov47Record *rec, void *msg, void *a2, u32 *data, enum HeapID a5, u32 color1, u32 color2, int a8);
void ov47_02259B30(Ov47Work *work, void *a1, void *a2, int a3);
int ov47_02259B74(Ov47Work *work, void *a1);
void ov47_02259C3C(Ov47Work *work, void *msg, void *a2, enum HeapID a5);
BOOL ov47_02259578(Ov47Work *work, void *msg, void *a2, void *win, Ov47Arg *a4, enum HeapID a5);

// @fn ov47_02259578
BOOL ov47_02259578(Ov47Work *work, void *msg, void *a2, void *win, Ov47Arg *a4, enum HeapID a5) {
    u32 buf[2];
    u32 data[2];
    void *p;

    switch (work->state) {
    case 0:
        ov47_022592B4(win, ov47_02258CEC(msg, 0, work->rec1.flag == 0 ? 103 : 105));
        work->nextState = 1;
        work->state = 22;
        break;
    case 1:
        ov47_022592B4(win, ov47_02259D40(&work->rec1, msg));
        work->nextState = 2;
        work->state = 22;
        break;
    case 2:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            ov47_022592B4(win, ov47_02258CEC(msg, 0, 104));
            work->nextState = 3;
            work->state = 22;
        }
        break;
    case 3:
        ov47_022592B4(win, ov47_02258CEC(msg, 0, work->rec1.flag == 0 ? 107 : 108));
        work->nextState = 4;
        work->state = 22;
        break;
    case 4:
        ov45_0222AB0C(ov45_0222A5C0(a4->unk4), buf);
        ov47_022599F0(work, &work->rec1, msg, a2, buf, a5, 0x90A00, 0xD0E00, ov45_0222AB28(a4->unk4, ov45_0222A53C(a4->unk4)));
        ov47_02259B30(work, work->unkC8, a2, 1);
        PlaySE(1495);
        work->state = 5;
        break;
    case 5:
        if (ov47_02259B74(work, a2) == 1) {
            work->state = 6;
        }
        break;
    case 6:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            if (work->rec2.id != -1) {
                work->state = 7;
            } else {
                ov47_022592B4(win, ov47_02258CEC(msg, 0, 112));
                work->nextState = 20;
                work->state = 22;
            }
        }
        break;
    case 7:
        ov47_02259318(win, ov47_02258CEC(msg, 0, 111));
        ov47_02259404(win, a2, a5);
        work->state = 8;
        break;
    case 8:
        switch (ov47_02259430(win, a5)) {
        case 0:
            work->state = 9;
            break;
        case -2:
            work->state = 20;
            break;
        }
        break;
    case 9:
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, a5);
        work->state = 10;
        break;
    case 10:
        if (IsPaletteFadeFinished() == TRUE) {
            ov47_02259C3C(work, msg, a2, a5);
            work->state = 11;
        }
        break;
    case 11:
        BeginNormalPaletteFade(0, 1, 1, 0, 6, 1, a5);
        work->state = 12;
        break;
    case 12:
        if (IsPaletteFadeFinished() == TRUE) {
            work->state = 13;
        }
        break;
    case 13:
        ov47_022592B4(win, ov47_02258CEC(msg, 0, work->rec2.flag == 0 ? 113 : 115));
        work->nextState = 14;
        work->state = 22;
        break;
    case 14:
        ov47_022592B4(win, ov47_02259D40(&work->rec2, msg));
        work->nextState = 15;
        work->state = 22;
        break;
    case 15:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            ov47_022592B4(win, ov47_02258CEC(msg, 0, 114));
            work->nextState = 16;
            work->state = 22;
        }
        break;
    case 16:
        ov47_022592B4(win, ov47_02258CEC(msg, 0, work->rec2.flag == 0 ? 117 : 118));
        work->nextState = 17;
        work->state = 22;
        break;
    case 17:
        p = sub_020318E8(a4->unk0);
        data[0] = sub_020318F4(p);
        data[1] = sub_020318F8(p);
        ov47_022599F0(work, &work->rec2, msg, a2, data, a5, 0xB0C00, 0xF0E00, ov45_0222AB28(a4->unk4, ov45_0222A53C(a4->unk4)));
        ov47_02259B30(work, work->unkDC, a2, 2);
        PlaySE(1495);
        work->state = 18;
        break;
    case 18:
        if (ov47_02259B74(work, a2) == 1) {
            work->state = 19;
        }
        break;
    case 19:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            work->state = 20;
        }
        break;
    case 20:
        ov47_022592B4(win, ov47_02258CEC(msg, 0, 120));
        work->nextState = 21;
        work->state = 22;
        break;
    case 21:
        ov47_022593CC(win);
        return TRUE;
    case 22:
        if (ov47_022593B4(win) == 1) {
            work->state = work->nextState;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return FALSE;
}
