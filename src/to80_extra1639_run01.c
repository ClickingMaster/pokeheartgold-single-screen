#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pad[0x120];
    s8 previous, current;
} State;

extern State *ov13_0224F4BC;
extern const s8 ov13_02245134[][4];
extern void ov13_022294B8(void);
extern void ov13_0223ABB8(int);
void ov13_02229594(int);

void ov13_02229594(int input) {
    int old = ov13_0224F4BC->current;
    ov13_0224F4BC->current = ov13_02245134[old][input];
    if (ov13_0224F4BC->current == 46 && input == 3) {
        ov13_0224F4BC->previous = old;
    } else if (ov13_0224F4BC->current == 51 && (input == 1 || input == 3)) {
        ov13_0224F4BC->previous = old;
    } else if (ov13_0224F4BC->current == 52 && (input == 1 || input == 3)) {
        if (old != 46) {
            ov13_0224F4BC->previous = old;
        }
    } else if (ov13_0224F4BC->current == -1) {
        if (ov13_0224F4BC->previous == 35 || ov13_0224F4BC->previous == 50) {
            ov13_0224F4BC->current = 35;
        } else {
            ov13_0224F4BC->current = 34;
        }
    } else if (ov13_0224F4BC->current == -2) {
        switch (ov13_0224F4BC->previous) {
        case 0:
        case 49:
            ov13_0224F4BC->current = 49;
            break;
        case 1:
        case 36:
            ov13_0224F4BC->current = 36;
            break;
        case 3:
        case 38:
            ov13_0224F4BC->current = 38;
            break;
        case 4:
        case 39:
            ov13_0224F4BC->current = 39;
            break;
        case 5:
        case 40:
            ov13_0224F4BC->current = 40;
            break;
        default:
            ov13_0224F4BC->current = 37;
            break;
        }
    } else if (ov13_0224F4BC->current == -3) {
        switch (ov13_0224F4BC->previous) {
        case 6:
        case 41:
            ov13_0224F4BC->current = 41;
            break;
        case 7:
        case 42:
            ov13_0224F4BC->current = 42;
            break;
        case 9:
        case 44:
            ov13_0224F4BC->current = 44;
            break;
        case 10:
        case 45:
            ov13_0224F4BC->current = 45;
            break;
        case 11:
        case 34:
        case 35:
        case 50:
            ov13_0224F4BC->current = 46;
            break;
        default:
            ov13_0224F4BC->current = 43;
            break;
        }
    } else if (ov13_0224F4BC->current == -4) {
        switch (ov13_0224F4BC->previous) {
        case 0:
        case 49:
            ov13_0224F4BC->current = 0;
            break;
        case 1:
        case 36:
            ov13_0224F4BC->current = 1;
            break;
        case 3:
        case 38:
            ov13_0224F4BC->current = 3;
            break;
        case 4:
        case 39:
            ov13_0224F4BC->current = 4;
            break;
        case 5:
        case 40:
            ov13_0224F4BC->current = 5;
            break;
        default:
            ov13_0224F4BC->current = 2;
            break;
        }
    } else if (ov13_0224F4BC->current == -5) {
        switch (ov13_0224F4BC->previous) {
        case 6:
        case 41:
            ov13_0224F4BC->current = 6;
            break;
        case 7:
        case 42:
            ov13_0224F4BC->current = 7;
            break;
        case 9:
        case 44:
            ov13_0224F4BC->current = 9;
            break;
        case 10:
        case 45:
            ov13_0224F4BC->current = 10;
            break;
        case 11:
        case 34:
            ov13_0224F4BC->current = 11;
            break;
        case 35:
        case 50:
            ov13_0224F4BC->current = 50;
            break;
        default:
            ov13_0224F4BC->current = 8;
            break;
        }
    }
    ov13_022294B8();
    ov13_0223ABB8(8);
}
