#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
struct State {
    void (*log)(int, char *, ...);
    u8 *work;
};
extern struct State ov13_0224F5C0;
extern char ov13_022473BC[], ov13_022473E0[];
void ov13_0223D6A8(u32);
void ov13_0223E698(void);
int ov13_0223DD64(void), ov13_0223DDB4(void), ov13_0223DC64(void), ov13_0223DC94(void);
void ov13_0223E6B4(void);
#define W ov13_0224F5C0.work
#define U32(o) (*(u32 *)(W + (o)))
// @fn ov13_0223E6B4
void ov13_0223E6B4(void) {
    if (U32(0x40) == 1) {
        if (ov13_0224F5C0.log != NULL) {
            ov13_0224F5C0.log(0x8000000, ov13_022473BC);
        }
        return;
    }
    if (ov13_0224F5C0.log != NULL) {
        ov13_0224F5C0.log(0x8000000, ov13_022473E0, U32(0x40));
    }
    if (!(U32(0x40) == 6 || U32(0x40) == 5 || U32(0x40) == 4)) {
        ov13_0223D6A8(3);
        ov13_0223E698();
    } else {
        ov13_0223D6A8(3);
        switch (U32(0x44)) {
        case 3:
            if (!ov13_0223DD64()) {
                ov13_0223E698();
            }
            break;
        case 1:
        case 5:
            if (!ov13_0223DDB4()) {
                ov13_0223E698();
            }
            break;
        case 2:
            if (!ov13_0223DC64()) {
                ov13_0223E698();
            }
            break;
        case 0:
        case 4:
            if (!ov13_0223DC94()) {
                ov13_0223E698();
            }
            break;
        }
    }
}
