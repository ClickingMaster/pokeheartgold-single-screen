#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x38];
    void *task;
    u8 pad2[0x15];
    u8 count;
    u8 pad3[7];
    u8 flag;
} Work;

typedef struct {
    u8 state, pad;
    u16 value;
    Work *work;
} State;

extern State ov13_0224F538;
extern void WifiSetupSound_Play(int);
extern void ov13_02230B14(void);
extern void *WifiSetupTask_Create(int, void (*)(void), int, int);
extern void ov13_02230ED0(void);
extern void ov13_022307AC(void);
extern void ov13_02239578(int);
extern void ov13_02230CC4(void);
void ov13_02230D10(int);

void ov13_02230D10(int input) {
    int changed = 1;
    int state = ov13_0224F538.state;
    switch (state) {
    case 0:
        if (input == 1) {
            if (ov13_0224F538.value == 0) {
                ov13_0224F538.state = 4;
            } else {
                WifiSetupSound_Play(19);
                ov13_0224F538.work->task = WifiSetupTask_Create(0, ov13_02230B14, 0, 120);
                return;
            }
        } else {
            if (ov13_0224F538.work->count > 1) {
                ov13_0224F538.state = state + 1;
            } else {
                changed = 0;
            }
        }
        break;
    case 1:
    case 2:
        if (input == 1) {
            ov13_0224F538.state = state - 1;
        } else {
            if (ov13_0224F538.work->count > state + 1) {
                ov13_0224F538.state = state + 1;
            } else {
                changed = 0;
            }
        }
        break;
    case 3:
        if (input == 1) {
            ov13_0224F538.state = state - 1;
        } else {
            ov13_02230ED0();
            return;
        }
        break;
    case 4:
        if (input == 1) {
            changed = 0;
        } else {
            ov13_0224F538.value = 0;
            ov13_0224F538.state = 0;
            ov13_022307AC();
            ov13_02239578(0);
        }
        break;
    }
    if (!changed) {
        if (ov13_0224F538.work->flag) {
            return;
        }
        WifiSetupSound_Play(9);
        ov13_0224F538.work->flag = 1;
        return;
    }
    WifiSetupSound_Play(8);
    ov13_02230CC4();
}
