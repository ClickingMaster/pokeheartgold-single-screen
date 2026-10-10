#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    void *handle;
    u8 pad[12];
    void *object;
    u8 left, mode, right, flag17;
    u16 pad2, flag1a;
    int ready, count;
    u8 pad3[0x17c];
    int status;
    u8 pad4[5], busy, pad5[0x56];
    int remote, current, next;
} State;

extern State *ov00_021F955C(void);
extern int ov00_022005B0(void *, int);
extern int ov00_021F7BA0(int);
extern int ov00_021F5B30(void);
extern void ov00_02210CBC(void *);
extern void ov00_021F60AC(int);
extern u8 ov00_0221A928[];
int ov00_021F5624(int, int, int, int, int);
#pragma opt_lifetimes off
#pragma force_active on

int ov00_021F5624(int player, int a, int b, int mode, int flag) {
    int result;
    switch (ov00_021F955C()->mode) {
    case 1:
        if (!ov00_022005B0(ov00_021F955C()->handle, player)) {
            result = 255;
            break;
        }
        if (!ov00_021F7BA0(player)) {
            result = 3;
            break;
        }
    case 0:
        if (mode != ov00_021F955C()->mode || ov00_021F955C()->busy || ov00_021F955C()->left == ov00_021F955C()->right || (ov00_021F955C()->flag17 && ov00_021F955C()->count == ov00_021F955C()->remote)) {
            result = 3;
            if (ov00_021F955C()->mode == 0 && *(int *)((u8 *)ov00_021F955C()->object + 0xb4) == 0 && ov00_021F955C()->flag17 && ov00_021F955C()->count == ov00_021F955C()->remote) {
                ov00_02210CBC(ov00_021F955C()->object);
            }
            break;
        }
        if (ov00_021F955C()->status == 3 || ov00_021F955C()->status == 4) {
            if (ov00_021F955C()->ready || ov00_021F955C()->flag1a) {
                if (a || b) {
                    goto accept;
                }
            }
        }
        result = 4;
        break;
accept: {
    if (ov00_021F955C()->current) {
        if (player == ov00_021F955C()->current) {
            if (flag || (ov00_021F955C()->remote < player && ov00_021F955C()->next != player)) {
                result = 2;
            } else {
                result = 255;
            }
        } else {
            if (flag || (ov00_021F955C()->remote < player && ov00_021F955C()->next == 0)) {
                ov00_021F60AC(ov00_021F955C()->current);
                if (ov00_021F5B30()) {
                    return 255;
                }
                result = 2;
            } else {
                result = 3;
            }
        }
    } else {
        result = 2;
    }
} break;
    case 2:
        if (!ov00_022005B0(ov00_021F955C()->handle, player)) {
            result = 255;
            break;
        }
        if (mode != 3 || ov00_021F955C()->left == ov00_021F955C()->right) {
            result = 3;
            break;
        }
        if (ov00_0221A928[0x1c] == 1 && ov00_0221A928[0x1d] == 1) {
            result = 19;
            break;
        }
        if (ov00_021F955C()->status == 10 && (ov00_021F955C()->ready || ov00_021F955C()->flag1a)) {
            if (a || b) {
                goto ready2;
            }
        }
        result = 4;
        break;
ready2:
        result = 2;
        break;
    }
    return result;
}
