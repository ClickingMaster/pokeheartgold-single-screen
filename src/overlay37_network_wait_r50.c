#include "global.h"

#include "overlay_37_helpers_internal.h"

#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))



extern void Comm_SendCommand(u32, const void *, u32);
extern void MI_CpuFill8(void *, u8, u32);

u32 ov37_021E6B64(Ov37Work *work, u32 state);

u32 ov37_021E6B64(Ov37Work *work, u32 state) {
    u8 packet[4];
    Ov37Work *w = work;

    w->flags &= ~0x38;
    if (w->player != Comm_CountConnected() || w->player != ov37_021E75C4()) {
        w->networkCounter = 0;
        ov37_021E7844(work, 9);
        ov37_021E68AC(work);
        return state;
    }
    w->networkCounter++;
    if (w->networkCounter > 30) {
        MI_CpuFill8(packet, 0, 4);
        packet[2] = 1;
        packet[0] = (u8)Comm_GetLocalNetId();
        Comm_SendCommand(0x7E, packet, 4);
        w->networkCounter = 0;
        ov37_021E7844(work, 10);
    }
    ov37_021E68AC(work);
    return state;
}
