#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x38];
    void *busy;
    u32 pad3c;
    u8 state, gap[4], flag, pad46, repeat;
} Work;

extern struct {
    u8 selected, pad[3];
    u16 position, pad6;
    Work *work;
} ov13_0224F54C;

extern int ov13_02233568(void);
extern int WifiSetupInput_IsKeyPressed(int);
extern int WifiSetupInput_IsKeyRepeated(int);
extern int WifiSetupInput_IsKeyReleased(int);
extern void ov13_022335EC(int);
extern void ov13_0223ABB8(int);
extern void ov13_0222B8E8(void *);
extern void ov13_02233A6C(void);
extern void ov13_02234AA4(void);
extern void ov13_022349DC(void);
extern void ov13_02234D84(int);
extern void *WifiSetupTask_Create(int, void *, int, int);
void ov13_022332E8(void);

void ov13_022332E8(void) {
    if (ov13_0224F54C.work->busy || ov13_0224F54C.work->flag) {
        return;
    }
    if (ov13_02233568()) {
        return;
    }
    if (WifiSetupInput_IsKeyPressed(1)) {
        ov13_022335EC(ov13_0224F54C.selected);
        return;
    }
    if (WifiSetupInput_IsKeyPressed(2)) {
        ov13_0223ABB8(7);
        ov13_0224F54C.work->state = 13;
        ov13_0222B8E8(ov13_02233A6C);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(512)) {
        if (ov13_0224F54C.position == 145) {
            if (ov13_0224F54C.work->repeat) {
                return;
            }
            ov13_0223ABB8(9);
            ov13_0224F54C.work->repeat = 1;
            return;
        }
        ov13_0223ABB8(19);
        ov13_0224F54C.work->busy = WifiSetupTask_Create(0, ov13_02234AA4, 0, 120);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(512)) {
        ov13_0224F54C.work->repeat = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(256)) {
        if (ov13_0224F54C.position == 0) {
            if (ov13_0224F54C.work->repeat) {
                return;
            }
            ov13_0223ABB8(9);
            ov13_0224F54C.work->repeat = 1;
            return;
        }
        ov13_0223ABB8(19);
        ov13_0224F54C.work->busy = WifiSetupTask_Create(0, ov13_022349DC, 0, 120);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(256)) {
        ov13_0224F54C.work->repeat = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(64)) {
        ov13_02234D84(1);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(64)) {
        ov13_0224F54C.work->repeat = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(128)) {
        ov13_02234D84(3);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(128)) {
        ov13_0224F54C.work->repeat = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(32)) {
        ov13_02234D84(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(16)) {
        ov13_02234D84(2);
    }
}
