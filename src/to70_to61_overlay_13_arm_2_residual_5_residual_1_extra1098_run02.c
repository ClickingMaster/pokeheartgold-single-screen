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
extern void WifiSetupSound_Play(int), ov13_02230F54(void), ov13_0223959C(void), WifiSetupButtonBar_Lock(void), ov13_0223BE6C(int, int, int, int, int), WifiSetupApp_SetStateFunc(void (*)(void)), ov13_02230F84(void), ov13_02230520(void);
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

void ov13_02230414(void) {
    int result, index;
    State *s;
    if (ov13_0224F538.state->task || ov13_0224F538.state->blocked) {
        return;
    }
    result = WifiSetupButtonBar_GetSelected();
    switch (result) {
    case 0:
        WifiSetupSound_Play(7);
        break;
    case 1:
        if (ov13_0224F538.selected == 4) {
            ov13_0224F538.state->busy = 1;
            WifiSetupSound_Play(6);
            ov13_02230F54();
        } else {
            index = FX_DivS32(ov13_0224F538.position, 28) + ov13_0224F538.selected;
            s = ov13_0224F538.state;
            if (s->items[index].kind == 2) {
                WifiSetupSound_Play(9);
                ov13_0223959C();
                WifiSetupButtonBar_Lock();
                ov13_0223BE6C(14, 1, 1, -1, 0);
                WifiSetupApp_SetStateFunc(ov13_02230F84);
                return;
            }
            s->busy = 1;
            ov13_0224F538.state->target = index;
            WifiSetupSound_Play(6);
        }
        break;
    default:
        return;
    }
    WifiSetupApp_SetStateFunc(ov13_02230520);
}
