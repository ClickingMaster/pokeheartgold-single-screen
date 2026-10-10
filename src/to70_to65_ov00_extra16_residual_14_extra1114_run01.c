#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 p[13];
    u8 count;
    u8 q[0xf4 - 14];
    int children[43];
    int state;
    u8 r[8];
    u8 retries;
    u8 s[5];
    u16 delay;
    u8 t[0x24];
    u32 mask;
    u8 u[12];
    u64 tick;
    u8 v[0xe0];
    u8 ids[256];
} State;

extern State *ov00_021F955C(void);
extern int ov00_021EE568(void);
extern int ov00_021F75F4(int, int);
extern int ov00_021F7CEC(int, int);
extern int ov00_021F7D50(int);
extern int ov00_021F7158(u32);
extern void ov00_021F956C(int);
extern void ov00_021F70B8(int);
extern void ov00_021F6EC4(int);
int ov00_021F7674(int, int, int), ov00_021F7898(void);

int ov00_021F7674(int connection, int kind, int arg) {
    int i;
    if (ov00_021EE568() != 6) {
        return 1;
    }
    switch (kind) {
    case 13:
        if (ov00_021F955C()->state != 8) {
            ov00_021F956C(8);
            ov00_021F70B8(arg);
        }
        if (!ov00_021F75F4(connection, 14)) {
            return 0;
        }
        break;
    case 14:
        if (ov00_021F955C()->state == 14) {
            u64 now = OS_GetTick();
            if ((((now - ov00_021F955C()->tick) << 6) / 33514 / 2) > 300) {
                u64 delay = (((now - ov00_021F955C()->tick) << 6) / 33514 / 2) - 300;
                if (delay > (u64)(int)ov00_021F955C()->delay) {
                    ov00_021F955C()->delay = delay;
                }
            }
            i = ov00_021F7CEC(connection, 0);
            if (i != 255) {
                State *s = ov00_021F955C();
                s->mask |= 1 << i;
            }
            i = ov00_021F7D50(1);
            if (i == ov00_021F955C()->mask) {
                for (i = 1; i <= ov00_021F955C()->count; i++) {
                    if (!ov00_021F75F4(ov00_021F955C()->children[i], 15)) {
                        return 0;
                    }
                }
                ov00_021F956C(15);
            }
        } else if (!ov00_021F75F4(connection, 15)) {
            return 0;
        }
        break;
    case 15:
        if (ov00_021F955C()->state == 8) {
            ov00_021F6EC4(2);
        }
        break;
    }
    return 1;
}

int ov00_021F7898(void) {
    u64 elapsed;
    int i;
    if (ov00_021F955C()->state == 8 || ov00_021F955C()->state == 14 || ov00_021F955C()->state == 15) {
        u64 now = OS_GetTick();
        elapsed = ((now - ov00_021F955C()->tick) << 6) / 33514;
    } else {
        return 1;
    }
    switch (ov00_021F955C()->state) {
    case 8:
        if (elapsed > 6000) {
            if (!ov00_021F75F4(ov00_021F955C()->children[0], 14)) {
                return 0;
            }
        }
        break;
    case 14:
        if (elapsed > 6000) {
            ++ov00_021F955C()->retries;
            if (ov00_021F955C()->retries > 5) {
                if (!ov00_021F7158(ov00_021F955C()->mask)) {
                    return 0;
                }
                if (ov00_021F955C()->count) {
                    ov00_021F955C()->retries = 0;
                    ov00_021F955C()->tick = OS_GetTick();
                } else {
                    ov00_021F6EC4(2);
                }
            } else {
                for (i = 1; i <= ov00_021F955C()->count; i++) {
                    State *s = ov00_021F955C();
                    if (!(s->mask & (1 << ov00_021F955C()->ids[i]))) {
                        if (!ov00_021F75F4(ov00_021F955C()->children[i], 13)) {
                            return 0;
                        }
                    }
                }
            }
        }
        break;
    case 15:
        if (elapsed > (u64)(int)ov00_021F955C()->delay) {
            ov00_021F6EC4(2);
        }
        break;
    }
    return 1;
}
