#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a)      (*(volatile u16 *)(a))
#define WL        (*(u8 **)0x0380fff4)
#define R16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define R32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)   (*(u8 **)((u8 *)(p) + (o)))
extern void ResetTxqPri(int);
extern void AddTask(int, int);
void SetParentTbttTxq(void);

void SetParentTbttTxq(void) {
    u8 *slots = WL + 0x42c;
    int pending = 0;
    ResetTxqPri(2);
    ResetTxqPri(1);
    ResetTxqPri(0);
    if (R16(slots, 0x28)) {
        if (R16(P(slots, 0x30), 0)) {
            pending = 1;
        } else {
            R16(slots, 0x28) = 0;
        }
    }
    if (R16(slots, 0x14)) {
        if (R16(P(slots, 0x1c), 0) == 0) {
            R16(slots, 0x14) = 0;
        } else {
            pending = 1;
        }
    }
    if (R16(slots, 0)) {
        if (R16(P(slots, 8), 0) == 0) {
            R16(slots, 0) = 0;
        } else {
            pending = 1;
        }
    }
    if (pending) {
        AddTask(0, 14);
    }
    AddTask(0, 20);
}
