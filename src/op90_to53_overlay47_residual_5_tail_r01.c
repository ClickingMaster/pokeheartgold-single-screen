#pragma force_active on
#include "global.h"
#include "system.h"
#include "unk_02005D10.h"

typedef struct {
    u8 unk0[4];
    void *unk4;
} Ov47Arg;

typedef struct {
    u8 unk0[0x10];
    int cursor;
    u8 unk14[0x28 - 0x14];
    u16 state;
    u16 nextState;
    u8 unk2C[4];
    int unk30;
    int unk34;
} Ov47Work;

void *ov47_02258CEC(void *a0, int a1, int a2);
void ov47_022592B4(void *a0, void *a1);
void ov47_02259404(void *a0, void *a1, void *a2);
u32 ov47_02259430(void *a0, void *a1);
void ov47_0225916C(Ov47Work *work, void *msg, void *a2);
void *ov47_02259D40(void *a0, void *a1);
void ov47_02259318(void *a0, void *a1);
void ov47_022591F8(Ov47Work *work);
void ov47_0225921C(Ov47Work *work);
void ov45_0222A7DC(void *a0, int a1, int a2);
void ov45_0222F4D4(int a0);
void ov47_02259384(void *a0);
int ov45_0222F524(void);
void ov47_022593A0(void *a0);
void ov47_022593CC(void *a0);
int ov47_022593B4(void *a0);
BOOL ov47_02258F48(Ov47Work *work, void *msg, void *a2, void *win, Ov47Arg *a4, void *a5);

static inline void ShowMsgA(void *win, void *msg, int id) { ov47_022592B4(win, ov47_02258CEC(msg, 0, id)); }
static inline void ShowMsgB(void *msg, void *win, int id) { ov47_022592B4(win, ov47_02258CEC(msg, 0, id)); }
static inline void ShowMsgC(Ov47Work *work, void *win, void *msg, int id, int next) { ov47_022592B4(win, ov47_02258CEC(msg, 0, id)); work->nextState = next; work->state = 10; }
static inline void ShowMsgD(Ov47Work *work, void *msg, void *win, int id, int next) { ov47_022592B4(win, ov47_02258CEC(msg, 0, id)); work->nextState = next; work->state = 10; }

// @fn ov47_02258F48
BOOL ov47_02258F48(Ov47Work *work, void *msg, void *a2, void *win, Ov47Arg *a4, void *a5) {
    switch (work->state) {
    case 0:
        ov47_022592B4(win, ov47_02258CEC(msg, 0, 94));
        work->nextState = 1;
        work->state = 10;
        break;
    case 1:
        ov47_02259404(win, a2, a5);
        work->state = 2;
        break;
    case 2:
        switch (ov47_02259430(win, a5)) {
        case 0:
            work->state = 3;
            break;
        case -2:
            work->state = 9;
            break;
        }
        break;
    case 3:
        ov47_022592B4(win, ov47_02258CEC(msg, 0, work->unk34 == 0 ? 92 : 93));
        work->nextState = 4;
        work->state = 10;
        break;
    case 4:
        ov47_0225916C(work, msg, a2);
        ov47_02259318(win, ov47_02259D40(work->unk2C, msg));
        ov47_022591F8(work);
        ov47_0225921C(work);
        work->state = 5;
        break;
    case 5:
        if (gSystem.newKeys & PAD_BUTTON_A) {
            PlaySE(1500);
            work->state++;
            break;
        } else if (gSystem.newKeys & PAD_KEY_UP) {
            if (work->cursor - 1 >= 0) {
                PlaySE(1504);
                work->cursor--;
            }
        } else if (gSystem.newKeys & PAD_KEY_DOWN) {
            if (work->cursor + 1 < 3) {
                PlaySE(1504);
                work->cursor++;
            }
        }
        ov47_022591F8(work);
        break;
    case 6:
        ov45_0222A7DC(a4->unk4, work->unk30, work->cursor);
        ov45_0222F4D4(work->cursor);
        ov47_02259318(win, ov47_02258CEC(msg, 0, 96));
        ov47_02259384(win);
        PlaySE(1405);
        work->state = 7;
        break;
    case 7:
        if (ov45_0222F524() == 1) {
            break;
        }
        PlaySE(1508);
        ov47_022593A0(win);
        ov47_022592B4(win, ov47_02258CEC(msg, 0, 97));
        work->nextState = 8;
        work->state = 10;
        break;
    case 8:
        ov47_022592B4(win, ov47_02258CEC(msg, 0, work->unk34 == 0 ? 98 : 99));
        work->nextState = 9;
        work->state = 10;
        break;
    case 9:
        ov47_022593CC(win);
        return TRUE;
    case 10:
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
