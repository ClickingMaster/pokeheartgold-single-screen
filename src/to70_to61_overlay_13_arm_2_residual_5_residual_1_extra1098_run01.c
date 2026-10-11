#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Row {
    u8 p[0x28];
    u8 kind, pad;
} Row;

typedef struct State {
    Row *items;
    u8 p4[0x34];
    void *task;
    u8 p3c[4];
    u16 max;
    u8 p42[0xe];
    s8 selected;
    u8 count, target, rows, busy, delay, p56, blocked, warn, repeat;
} State;

typedef struct Global {
    u8 selected, pad;
    u16 position;
    State *state;
} Global;

extern Global ov13_0224F538;
extern void ov13_0223ABB8(int), ov13_02230F54(void), ov13_0223959C(void), WifiSetupButtonBar_Lock(void), ov13_0223BE6C(int, int, int, int, int), ov13_0222B8E8(void (*)(void)), ov13_02230F84(void), ov13_02230520(void);
extern int WifiSetupButtonBar_GetSelected(void);
extern int FX_DivS32(int, int);
void ov13_02230414(void);

typedef struct Box {
    u32 a, b;
} Box;

extern Box ov13_02245958;
extern Box ov13_022453FC[];
extern int WifiSetupInput_IsTouchPressed(Box *);
extern int WifiSetupInput_IsTouchReleased(Box *);
extern int WifiSetupInput_IsKeyPressed(int);
extern int WifiSetupInput_IsKeyRepeated(int);
extern int WifiSetupInput_IsKeyReleased(int);
extern void WifiSetupButtonBar_SelectIfNone(int);
extern void ov13_02230CC4(void);
extern void ov13_02230ED0(void);
extern void ov13_02230E5C(void);
extern void ov13_02230D10(int);
void ov13_0222FF50(void);

void ov13_0222FF50(void) {
    int i;
    Box *box;
    if (ov13_0224F538.state->task || ov13_0224F538.state->blocked) {
        return;
    }
    if (WifiSetupInput_IsTouchPressed(&ov13_02245958)) {
        ov13_0224F538.state->selected = -1;
        for (i = 0, box = ov13_022453FC; (u32)i < 5; i++, box++) {
            if (WifiSetupInput_IsTouchPressed(box)) {
                if (i < 4) {
                    ov13_0224F538.state->selected = i;
                    break;
                }
                WifiSetupButtonBar_SelectIfNone(1);
                ov13_0224F538.selected = i;
                ov13_02230CC4();
                return;
            }
        }
    }
    if (WifiSetupInput_IsTouchReleased(&ov13_02245958)) {
        Box *pos;
        int j;
        for (j = 0, pos = ov13_022453FC; j < 4; j++, pos++) {
            if (WifiSetupInput_IsTouchReleased(pos)) {
                if (ov13_0224F538.state->selected != j) {
                    break;
                }
                if (j >= ov13_0224F538.state->count) {
                    ov13_0223ABB8(9);
                    break;
                }
                WifiSetupButtonBar_SelectIfNone(1);
                ov13_0224F538.selected = j;
                ov13_02230CC4();
                return;
            }
        }
    }
    if (WifiSetupInput_IsKeyPressed(1)) {
        WifiSetupButtonBar_SelectIfNone(1);
        ov13_0223959C();
        return;
    }
    if (WifiSetupInput_IsKeyPressed(2)) {
        WifiSetupButtonBar_SelectIfNone(0);
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x200)) {
        ov13_02230ED0();
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x200)) {
        ov13_0224F538.state->repeat = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x100)) {
        ov13_02230E5C();
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x100)) {
        ov13_0224F538.state->repeat = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x40)) {
        ov13_02230D10(1);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x40)) {
        ov13_0224F538.state->repeat = 0;
        return;
    }
    if (WifiSetupInput_IsKeyRepeated(0x80)) {
        ov13_02230D10(3);
        return;
    }
    if (WifiSetupInput_IsKeyReleased(0x80)) {
        ov13_0224F538.state->repeat = 0;
    }
}
