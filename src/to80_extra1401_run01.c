#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0xa90];
    u8 state, pad91[7];
    u32 counter, timer;
    u8 gap[12];
    u8 flag;
} Work;

extern Work *ov13_0224F5C8;
extern void ov13_0223EA58(void);
extern void ov13_0223EEAC(void);
extern void ov13_0223EF54(void);
extern void ov13_0223EF78(void);
extern void ov13_0223F130(void);
extern void ov13_0223F15C(void);
extern void ov13_0223F338(void);
extern void ov13_0223F350(void);
extern void ov13_0223E7BC(void);
extern void ov13_0223E6B4(void);
extern void MB_End(void);
extern int ov13_0223DF28(void);
void ov13_0223EAD4(void);

void ov13_0223EAD4(void) {
    switch (ov13_0224F5C8->state) {
    case 1:
        if (ov13_0224F5C8->flag == 1) {
            ov13_0224F5C8->flag = 0;
            ov13_0223EA58();
            return;
        }
        if (ov13_0224F5C8->flag == 2) {
            ov13_0224F5C8->flag = 0;
            ov13_0224F5C8->state = 34;
        }
        return;
    case 2:
        ov13_0223EEAC();
        return;
    case 3:
        ov13_0223EF54();
        ov13_0224F5C8->state = 4;
        return;
    case 4:
        ov13_0224F5C8->counter++;
        ov13_0223EF78();
        return;
    case 5:
    case 6:
        ov13_0223EF78();
        return;
    case 7:
        ov13_0223F130();
        return;
    case 8:
    case 9:
    case 10:
        ov13_0223F15C();
        return;
    case 11:
        ov13_0223F338();
        return;
    case 16:
        ov13_0224F5C8->timer = 0;
        ov13_0224F5C8->state = 17;
        return;
    case 17:
        if (ov13_0224F5C8->timer++ > 30) {
            ov13_0223F350();
        }
        return;
    case 18:
        MB_End();
        ov13_0224F5C8->timer = 0;
        ov13_0224F5C8->state = 22;
        return;
    case 19:
        if (ov13_0224F5C8->timer++ <= 30) {
            return;
        }
        if (ov13_0223DF28() == 1) {
            ov13_0223E7BC();
            ov13_0224F5C8->state = 20;
            return;
        }
        return;
    case 21:
        ov13_0223E6B4();
        ov13_0224F5C8->timer = 0;
        ov13_0224F5C8->state = 22;
        return;
    case 22:
        if (ov13_0224F5C8->timer++ <= 30) {
            return;
        }
        if (ov13_0223DF28() == 1) {
            ov13_0223E7BC();
            ov13_0224F5C8->state = 23;
            return;
        }
        return;
    case 24:
        ov13_0223E6B4();
        ov13_0224F5C8->timer = 0;
        ov13_0224F5C8->state = 25;
        return;
    case 25:
        if (ov13_0224F5C8->timer++ <= 30) {
            return;
        }
        if (ov13_0223DF28() == 1) {
            ov13_0223E7BC();
            ov13_0224F5C8->state = 26;
            return;
        }
        return;
    case 27:
        ov13_0223E6B4();
        ov13_0224F5C8->timer = 0;
        ov13_0224F5C8->state = 28;
        return;
    case 28:
        if (ov13_0224F5C8->timer++ <= 30) {
            return;
        }
        if (ov13_0223DF28() == 1) {
            ov13_0223E7BC();
            ov13_0224F5C8->state = 29;
            return;
        }
        if (ov13_0224F5C8->timer % 30 == 1 && ov13_0224F5C8->timer > 55) {
            ov13_0223E6B4();
        }
        return;
    case 32:
        ov13_0223E6B4();
        ov13_0224F5C8->timer = 0;
        ov13_0224F5C8->state = 33;
        return;
    case 33:
        if (ov13_0224F5C8->timer++ <= 30) {
            return;
        }
        if (ov13_0223DF28() == 1) {
            ov13_0223E7BC();
            ov13_0224F5C8->state = 34;
            return;
        }
        return;
    case 30:
        ov13_0223E6B4();
        return;
    case 0:
    case 12:
    case 13:
    case 14:
    case 15:
    case 20:
    case 23:
    case 26:
    case 29:
    case 31:
    case 34:
    default:
        goto done;
    }
done:
    return;
}
