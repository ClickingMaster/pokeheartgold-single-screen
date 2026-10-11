#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x1e298];
    void (*cb)(int);
    u32 unk;
    u8 active;
    u8 done;
} DwcWork;

DwcWork *ov13_0224F4F0;

extern void ov13_0223EAD4(int);
extern void ov13_0223EE58(u8 *, u8 *);
extern void WifiSetupTask_DestroyLater(int, int);
extern void WifiSetupHeap_FreeAndClear(void *);
extern u16 *ov13_0223EE9C(void);
void ov13_0222BCC0(int n);

// @fn ov13_0222BCC0
void ov13_0222BCC0(int n) {
    u8 event;
    u8 ok;

    ov13_0223EAD4(n);
    if (ov13_0224F4F0->active && !ov13_0224F4F0->done) {
        if (ov13_0224F4F0->cb != NULL) {
            ov13_0224F4F0->cb(0);
        }
        return;
    }
    ov13_0223EE58(&event, &ok);
    switch (event) {
    case 5:
        if (!ok) {
            return;
        }
        {
            u8 *p = (u8 *)ov13_0224F4F0 + 0x2280;
            u16 *src = ov13_0223EE9C();
            MIi_CpuCopy16(src, (u16 *)(p + 0x1c000), 0x16);
        }
        if (ov13_0224F4F0->cb == NULL) {
            ov13_0224F4F0->active = 1;
            return;
        }
        ov13_0224F4F0->cb(0);
        return;
    case 13:
        if (!ok) {
            return;
        }
        if (ov13_0224F4F0->cb == NULL) {
            ov13_0224F4F0->active = 1;
            return;
        }
        ov13_0224F4F0->cb(1);
        return;
    case 20:
    case 23:
        if (!ok) {
            return;
        }
        if (ov13_0224F4F0->cb == NULL) {
            ov13_0224F4F0->active = 1;
            return;
        }
        ov13_0224F4F0->cb(3);
        return;
    case 26:
    case 29:
        if (!ok) {
            return;
        }
        if (ov13_0224F4F0->cb == NULL) {
            ov13_0224F4F0->active = 1;
            return;
        }
        ov13_0224F4F0->cb(4);
        return;
    case 12:
        if (!ok) {
            return;
        }
        if (ov13_0224F4F0->cb == NULL) {
            ov13_0224F4F0->active = 1;
            return;
        }
        ov13_0224F4F0->cb(2);
        return;
    case 1: case 2: case 8: case 9:
        break;
    case 34:
        WifiSetupTask_DestroyLater(0, n);
        WifiSetupHeap_FreeAndClear(&ov13_0224F4F0);
        break;
    }
}
