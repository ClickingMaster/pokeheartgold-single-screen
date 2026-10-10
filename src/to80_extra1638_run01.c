#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct State {
    u8 pad[0x11c];
    u8 value, mode;
    u8 pad2[3];
    s8 selected;
    u8 pad3;
    u8 valid, ready, cancel;
} State;

extern State *ov13_0224F4BC;
extern u8 *ov13_02245D68[];
extern u8 ov13_02245208[];
extern u8 ov13_02244F58[];
int ov13_022416D4(int);
int ov13_02241694(int);
int ov13_02241714(int);
void ov13_02229594(int);
void ov13_0223ABB8(int);
void ov13_0222936C(int);
void ov13_02228C08(void);
void ov13_02228C38(void);
void ov13_02228EC0(void);

void ov13_02228EC0(void) {
    int selected;
    if (ov13_022416D4(32)) {
        ov13_02229594(0);
    }
    if (ov13_022416D4(64)) {
        ov13_02229594(1);
    }
    if (ov13_022416D4(16)) {
        ov13_02229594(2);
    }
    if (ov13_022416D4(128)) {
        ov13_02229594(3);
    }
    if (ov13_02241694(1)) {
        selected = ov13_0224F4BC->selected;
        if (selected < 47) {
            if (!ov13_0224F4BC->ready) {
                ov13_0223ABB8(9);
                return;
            }
            ov13_0224F4BC->value = ov13_02245D68[ov13_0224F4BC->mode][selected];
            if (ov13_0224F4BC->mode == 1) {
                ov13_0222936C(0);
            }
            return;
        }
        if (selected - 47 < 4) {
            switch (selected - 47) {
            case 0:
                ov13_02228C08();
                return;
            case 1:
                ov13_02228C38();
                return;
            case 2:
                if (!ov13_0224F4BC->ready) {
                    ov13_0223ABB8(9);
                    return;
                }
                break;
            case 3:
                if (!ov13_0224F4BC->valid) {
                    ov13_0223ABB8(9);
                    return;
                }
                break;
            default:
                break;
            }
            if (ov13_0224F4BC->mode == 1) {
                ov13_0222936C(0);
            }
            ov13_0224F4BC->value = ov13_02245208[selected - 47];
            return;
        }
        ov13_0224F4BC->value = ov13_02244F58[selected - 51];
    }
    if (ov13_022416D4(2)) {
        if (!ov13_0224F4BC->valid) {
            if (ov13_0224F4BC->cancel) {
                return;
            }
            ov13_0223ABB8(9);
            ov13_0224F4BC->cancel = 1;
            return;
        }
        ov13_0224F4BC->value = 128;
    } else if (ov13_02241714(2)) {
        ov13_0224F4BC->cancel = 0;
    }
    if (ov13_02241694(1024)) {
        ov13_02228C08();
    }
    if (ov13_02241694(2048)) {
        ov13_02228C38();
    }
}
