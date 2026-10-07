#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#define ROOT (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define V(o) (*(volatile u16 *)(o))
extern int MatchMacAdrs(const void *, const void *);
extern void SendMessageToWmDirect(void *, void *);
int RxMpFrame(u8 *frame);
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
extern u32 CAM_Search(const void *);
extern u32 CAM_GetStaState(u32);
extern void CAM_SetPowerMgtMode(u16, u32);
extern void CAM_UpdateLifeTime(u32);
extern u32 CAM_GetAID(u32);
extern int IsExistManFrame(const void *, u16);
extern void *MakeDeAuthFrame(const void *, u16, u32);
extern void TxManCtrlFrame(void *);
extern void WL_WriteByte(void *, u32);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
typedef struct {
    u16 : 12;
    u16 pwrMgt : 1;
    u16 : 3;
} FrameCtrl;
void RxKeyDataFrame(u8 *frame);
// @fn RxMpFrame
int RxMpFrame(u8 *frame)
{
    u8 *wl = ROOT;
    u8 *cfg = wl + 0x344;
    u8 *rx = wl + 0x4dc;
    u8 *msg;
    u16 ctrl, bits;
    u32 n;
    if (H(cfg, 8) != 0x40)
        return 1;
    if (!MatchMacAdrs(frame + 0x1e, cfg + 0x64) || !MatchMacAdrs(frame + 0x24, cfg + 0x82))
        return 1;
    if ((H(frame, 0x2e) & (u16)(1 << H(cfg, 0x6a))) == 0)
        H(rx, 6) = 0;
    else
        H(rx, 6) = 0x2000;
    ctrl = V(0x04808098);
    if (ctrl & 0x8000) {
        if (*(u16 *)(0x04804000 + (u16)(ctrl << 1) + 4) != 0)
            H(rx, 6) |= 0x4000;
    }
    CAM_UpdateLifeTime(H(cfg, 0x88));
    H(frame, 6) = H(frame, 0x10) - 0x1c;
    msg = frame - 0x10;
    H(msg, 0xc) = 0x182;
    H(msg, 0xe) = (u32)(H(frame, 6) + 0x31) >> 1;
    n = 0;
    bits = H(msg, 0x3e);
    while (bits) {
        if (bits & 1)
            n++;
        bits >>= 1;
    }
    H(msg, 0x18) |= H(rx, 6) | ((int)(V(0x04808098) & 0x8000) >> 4) | ((int)(V(0x04808094) & 0x8000) >> 3);
    H(msg, 0x1a) = H(msg, 0x1c) + ((n * (H(msg, 0x3c) + 10) + 0xfc) >> 4);
    SendMessageToWmDirect(ROOT + 0x188, msg);
    return 0;
}
// @fn RxKeyDataFrame
void RxKeyDataFrame(u8 *frame)
{
    u8 *kb;
    u8 *wl = ROOT;
    u8 *p = wl + 0x42c;
    u32 aid;
    u16 bit, m;
    u8 *dst;
    kb = P(p, 0x90);
    if (H(p, 0x3c) == 0)
        return;
    if (!MatchMacAdrs(frame + 0x18, wl + 0x3a8))
        return;
    if (H(frame, 0x10) - 0x18 > H(kb, 0x16) - 8)
        return;
    aid = CAM_Search(frame + 0x1e);
    if (aid == 0xff || (aid != 0 && CAM_GetStaState(aid) != 0x40)) {
        u16 *f;
        if (IsExistManFrame(frame + 0x1e, 0xc0))
            return;
        f = MakeDeAuthFrame(frame + 0x1e, 7, 0);
        if (f) {
            *f = 2;
            TxManCtrlFrame(f);
        }
        return;
    }
    if (aid == 0)
        return;
    CAM_SetPowerMgtMode((u16)aid, ((FrameCtrl *)(frame + 0x14))->pwrMgt);
    CAM_UpdateLifeTime(aid);
    bit = (u16)(1 << CAM_GetAID(aid));
    if (bit & H(p, 0x9a))
        return;
    if (!(bit & H(p, 0x98)))
        return;
    H(p, 0x9a) |= bit;
    H(kb, 0x10) &= ~bit;
    dst = kb + 0x1a;
    for (m = bit >> 1; m != 1; m >>= 1) {
        if (m & H(p, 0x98))
            dst += H(kb, 0x16);
    }
    H(dst, 0) = H(frame, 0x10) - 0x18;
    WL_WriteByte(dst + 3, H(frame, 0x12) & 0xff);
    WL_WriteByte(dst + 2, H(frame, 0xe) & 0xff);
    if (H(dst, 0))
        __VENEER_MIi_CpuCopy16(frame + 0x2c, dst + 8, H(dst, 0) + 1);
}
