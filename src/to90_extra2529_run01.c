#pragma force_active on
#include "global.h"
#pragma force_active on

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct State {
    u8 pad[0x40];
    int sub, state;
    u8 rest[0x1398];
    u8 sharing[0x820];
};

struct Global {
    void (*cb)(int, char *, ...);
    struct State *s;
};
extern struct Global ov13_0224F5C0;
extern char ov13_02247284[];
extern char ov13_02247264[];
extern void ov13_0223D724(int);
extern void ov13_0223D6A8(int);
extern int ov13_0223DC28(void);
extern int WM_StartDataSharing(void *, u16, u16, u16, int);
void ov13_0223DAFC(u16 *);

void ov13_0223DAFC(u16 *msg) {
    int result, state;
    if (msg[1]) {
        ov13_0223D724(msg[1]);
        ov13_0223D6A8(9);
        return;
    }
    state = msg[2];
    switch (state) {
    case 10:
        if (ov13_0224F5C0.s->state == 2) {
            if (ov13_0224F5C0.s->sub == 4) {
                if (ov13_0223DC28()) {
                    return;
                }
                if (ov13_0224F5C0.cb) {
                    ov13_0224F5C0.cb(0x8000000, ov13_02247284);
                }
                ov13_0223D6A8(9);
                return;
            } else if (ov13_0224F5C0.s->sub == 6) {
                return;
            }
        } else if (ov13_0224F5C0.s->state == 4) {
            result = WM_StartDataSharing(ov13_0224F5C0.s->sharing, 13, 7, 68, 1);
            if (result) {
                ov13_0223D724(result);
                ov13_0223D6A8(9);
                return;
            }
            ov13_0223D6A8(5);
            return;
        }
        ov13_0223D6A8(4);
        return;
    case 11:
        break;
    case 12:
    case 13:
    default:
        if (ov13_0224F5C0.cb) {
            ov13_0224F5C0.cb(0x8000000, ov13_02247264, state);
        }
        break;
    }
}
