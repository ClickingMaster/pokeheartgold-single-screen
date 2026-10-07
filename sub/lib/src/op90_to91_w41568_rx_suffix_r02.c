#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#define ROOT (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define V(o) (*(volatile u16 *)(o))
extern int MatchMacAdrs(const void *, const void *);
extern void SendMessageToWmDirect(void *, void *);
int RxMpAckFrame(u8 *frame);
// @fn RxMpAckFrame
int RxMpAckFrame(u8 *frame)
{
    u8 *wl = ROOT;
    u8 *cfg = wl + 0x344;
    u8 *msg;
    if (H(wl, 0x34c) != 0x40)
        return 1;
    if (!MatchMacAdrs(frame + 0x1e, cfg + 0x64) || !MatchMacAdrs(frame + 0x24, cfg + 0x82))
        return 1;
    H(frame, 6) = H(frame, 0x10) - 0x1c;
    msg = frame - 0x10;
    H(msg, 0xc) = 0x185;
    H(msg, 0xe) = 0x18;
    H(msg, 0x18) |= H(ROOT, 0x4e2) | ((int)(V(0x04808098) & 0x8000) >> 4) | ((int)(V(0x04808094) & 0x8000) >> 3);
    SendMessageToWmDirect(ROOT + 0x188, msg);
    return 0;
}
