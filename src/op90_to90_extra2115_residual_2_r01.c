#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 pad0[0x60];
    u8 b60;
    s8 selected;
    u8 pad62[3];
    u8 holdCount;
    u8 b66;
} MenuWork;
extern MenuWork *ov13_0224F4C0;
extern u8 ov13_02245958[], ov13_0224527A[], ov13_02245228[], ov13_02245250[], ov13_02245230[], ov13_02245258[], ov13_02245234[];
int ov13_02241754(const void *rect);
void ov13_02240E74(const void *entry, const void *base, void *out);
void ov13_0222A914(int sel);
void ov13_0223ABB8(int se);
void ov13_0222A724(void);
// @fn ov13_0222A724
void ov13_0222A724(void) {
    u8 rect[8];
    int i;
    u8 *entry;
    if (!ov13_02241754(ov13_02245958)) {
        goto cancel;
    }
    entry = ov13_0224527A;
    for (i = 0; i < 10; i++, entry += 4) {
        ov13_02240E74(entry, ov13_02245228, rect);
        if (ov13_02241754(rect)) {
            if (ov13_0224F4C0->selected != i) {
                goto cancel;
            }
            ov13_0222A914(i);
            goto done;
        }
    }
    entry = ov13_02245250;
    for (i = 0; i < 2; i++, entry += 4) {
        ov13_02240E74(entry, ov13_02245230, rect);
        if (ov13_02241754(rect)) {
            if (ov13_0224F4C0->selected != i + 10) {
                goto cancel;
            }
            ov13_0222A914(i + 10);
            if (i != 0) {
                goto done;
            }
            ov13_0224F4C0->holdCount++;
            if (ov13_0224F4C0->holdCount < 40) {
                return;
            }
            if (ov13_0224F4C0->b66 == 0) {
                ov13_0223ABB8(9);
                ov13_0224F4C0->selected = -1;
                return;
            }
            ov13_0224F4C0->b60 = 0x10;
            ov13_0224F4C0->holdCount -= 7;
            return;
        }
    }
    entry = ov13_02245258;
    for (i = 0; i < 2; i++, entry += 4) {
        ov13_02240E74(entry, ov13_02245234, rect);
        if (ov13_02241754(rect)) {
            if (ov13_0224F4C0->selected != i + 12) {
                goto cancel;
            }
            ov13_0222A914(i + 12);
            goto done;
        }
    }
cancel:
    ov13_0222A914(-1);
done:
    ov13_0224F4C0->holdCount = 0;
}
