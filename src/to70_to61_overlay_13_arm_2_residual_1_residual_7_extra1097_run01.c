#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int ov13_02241754(void *);
extern void ov13_02240E74(void *, void *, void *);
extern void ov13_02228E6C(int);
extern void ov13_0223ABB8(int);
extern u32 ov13_02245958[];
extern u32 ov13_02244FA4[];
extern u32 ov13_02244F68[];
extern u32 ov13_02244F9C[];
extern u32 ov13_02244F8C[];
extern u32 ov13_02244F7C[];
extern u32 ov13_02244F64[];

typedef struct {
    u8 p[0x11c];
    u8 mark, a;
    s8 selection;
    u8 q[3];
    u8 count, flag;
} Work;

extern Work *ov13_0224F4BC;
void ov13_02228C68(void);

void ov13_02228C68(void) {
    u32 result[2];
    int i;
    u32 *one, *two;
    if (!ov13_02241754(ov13_02245958)) {
        goto none;
    }
    one = ov13_02244FA4;
    for (i = 0; i < 47; i++, one++) {
        ov13_02240E74(one, ov13_02244F68, result);
        if (ov13_02241754(result)) {
            if (ov13_0224F4BC->selection != i) {
                goto none;
            }
            ov13_02228E6C(i);
            goto reset;
        }
    }
    two = ov13_02244F9C;
    one = ov13_02244F8C;
    for (i = 2; i < 4; i++, two++, one++) {
        ov13_02240E74(one, two, result);
        if (ov13_02241754(result)) {
            if (ov13_0224F4BC->selection != i + 47) {
                goto none;
            }
            ov13_02228E6C(i + 47);
            if (i != 3) {
                return;
            }
            ov13_0224F4BC->count++;
            if (ov13_0224F4BC->count < 40) {
                return;
            }
            if (ov13_0224F4BC->flag == 0) {
                ov13_0223ABB8(9);
                ov13_0224F4BC->selection = -1;
                return;
            }
            ov13_0224F4BC->mark = 128;
            ov13_0224F4BC->count -= 7;
            return;
        }
    }
    one = ov13_02244F7C;
    for (i = 0; i < 2; i++, one++) {
        ov13_02240E74(one, ov13_02244F64, result);
        if (ov13_02241754(result)) {
            if (ov13_0224F4BC->selection != i + 51) {
                goto none;
            }
            ov13_02228E6C(i + 51);
            goto reset;
        }
    }
none:
    ov13_02228E6C(-1);
reset:
    ov13_0224F4BC->count = 0;
}
