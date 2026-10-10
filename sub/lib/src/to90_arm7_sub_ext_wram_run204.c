#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a)      (*(volatile u16 *)(a))
#define WL        (*(u8 **)0x0380fff4)
#define R16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define R32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)   (*(u8 **)((u8 *)(p) + (o)))
extern void WlIntrRxEndTask(void);
extern void WDisableTmpttPowerSave(void);
extern void SendMessageToWmDirect(void *, void *);
void WlIntrMpEndTask(void);

void WlIntrMpEndTask(void) {
    u8 *slots = WL + 0x42c;
    if (R16(slots, 0x3c)) {
        u8 *out = P(slots, 0x90);
        if (R16(out, 0x10) != R16(P(slots, 0x44), 2)) {
            WlIntrRxEndTask();
        }
        {
            u32 retries = R16(P(slots, 0x44), 4) & 255;
            if (retries) {
                R32(WL, 0x5a0) += retries;
            } else {
                R32(WL, 0x5a0)
                ++;
            }
        }
        R16(slots, 0x40)
        ++;
        out = P(slots, 0x90);
        R16(out, 0x12) = R16(out, 0x10) ^ R16(P(slots, 0x44), 2);
        R16(slots, 0xa0) = R16(P(slots, 0x90), 0x10);
        R16(slots, 0x3c) = 0;
        if (R16(WL, 0x3ea)) {
            WDisableTmpttPowerSave();
        }
        SendMessageToWmDirect(WL + 0x188, P(slots, 0x90));
    }
}
