#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 p[0x38];
    void *busy;
    u8 gap[5];
    u8 timer;
    u8 spare;
    u8 state;
    u8 spare2;
    u8 flag;
    u8 flag2;
} S;

extern struct {
    u32 p;
    u16 val;
    u16 pad;
    S *s;
} ov13_0224F54C;

extern u8 *ov13_02239FF0(void);
extern void ov13_0223A018(int);
extern void ov13_02233DCC(void);
void ov13_022337D8(int);
extern int ov13_02239564(void);
extern int ov13_02239550(void);
extern void WifiSetupCursor_Hide(void);
extern void WifiSetupSound_Play(int);
extern void ov13_02234CA4(void);
extern void ov13_022349DC(void);
extern void ov13_02234AA4(void);
extern void *WifiSetupTask_Create(int, void (*)(void), int, int);
void ov13_02233810(void);

void ov13_022337D8(int v) {
    u32 old = ov13_02239FF0()[0xf6];
    if (old == (u32)v) {
        return;
    }
    ov13_0224F54C.s->state = v;
    ov13_0223A018(v);
    ov13_02233DCC();
}

void ov13_02233810(void) {
    int rem;
    if (ov13_0224F54C.s->busy) {
        return;
    }
    if (ov13_0224F54C.s->timer) {
        ov13_0224F54C.s->timer--;
    }
    switch (ov13_02239564()) {
    case 0:
        goto end;
    case 1:
        ov13_0224F54C.s->flag = 1;
        return;
    case 2:
        if (ov13_0224F54C.s->timer) {
            return;
        }
        WifiSetupCursor_Hide();
        ov13_0224F54C.val = (145 * ov13_02239550()) / 55;
        ov13_02233DCC();
        ov13_0224F54C.s->timer = 4;
        return;
    case 3:
        ov13_0224F54C.s->flag = 0;
        ov13_0224F54C.val = (145 * ov13_02239550()) / 55;
        WifiSetupSound_Play(19);
        ov13_02233DCC();
        rem = FX_ModS32(ov13_0224F54C.val, 29);
        if (rem == 0) {
            ov13_02234CA4();
            return;
        }
        if (rem < 16) {
            ov13_0224F54C.s->busy = WifiSetupTask_Create(0, ov13_022349DC, 0, 120);
        } else {
            ov13_0224F54C.s->busy = WifiSetupTask_Create(0, ov13_02234AA4, 0, 120);
            return;
        }
        return;
    case 4:
        if (ov13_0224F54C.val == 0) {
            if (ov13_0224F54C.s->flag2) {
                return;
            }
            WifiSetupSound_Play(9);
            ov13_0224F54C.s->flag2 = 1;
            return;
        }
        WifiSetupSound_Play(19);
        ov13_0224F54C.s->busy = WifiSetupTask_Create(0, ov13_022349DC, 0, 120);
        return;
    case 6:
        if (ov13_0224F54C.val == 145) {
            if (ov13_0224F54C.s->flag2) {
                return;
            }
            WifiSetupSound_Play(9);
            ov13_0224F54C.s->flag2 = 1;
            return;
        }
        WifiSetupSound_Play(19);
        ov13_0224F54C.s->busy = WifiSetupTask_Create(0, ov13_02234AA4, 0, 120);
        return;
    case 5:
    case 7:
        ov13_0224F54C.s->flag2 = 0;
        return;
    default:
        goto end;
    }
end:
    return;
}
