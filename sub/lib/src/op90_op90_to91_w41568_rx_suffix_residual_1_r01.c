#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#define ROOT (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define V(o) (*(volatile u16 *)(o))
typedef struct {
    u8 *elements;
    u16 x04;
    u16 x06;
    u16 length;
    u16 valid;
    u16 present;
    u16 x0e;
    u16 x10;
    u16 channel;
    u16 x14;
    u16 rates;
    u16 x18;
    u16 x1a;
    u8 *ssid;
    u8 *cf;
    u8 *tim;
    u8 *vendor;
} Elements;
typedef struct {
    u16 b0 : 6;
    u16 beaconInd : 1;
    u16 b7 : 9;
} CfgFlags;
extern u32 CAM_SearchAdd(const void *);
extern void CAM_SetRSSI(u16, u16);
extern void CAM_SetSupRate(u32, u32);
extern void CAM_UpdateLifeTime(u32);
extern void __VENEER_MIi_CpuClear32(u32, void *, u32);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
extern void ElementChecker(Elements *);
extern u8 WL_ReadByte(const void *);
extern void RxProbeResFrame(u8 *, Elements *);
extern void ClearTimeOut(void);
extern void WSetChannel(u16, u16);
extern u32 WSetActiveZoneTime(u16, u32);
extern int WSetDTIMPeriod(u16);
extern u32 WSetBeaconPeriod(u16);
extern void WSetMacAdrs1(void *, const void *);
extern void AddTask(u32, u32);
extern u64 __VENEER__ll_udiv(u64, u64);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern void TxPsPollFrame(void);
extern void WSetPowerState(u32);
extern int MLME_IssueBeaconRecvIndication(u8 *);
extern void UpdateApList(u32, const void *, const void *);
void RxBeaconFrame(u8 *frame);
// @fn RxBeaconFrame
void RxBeaconFrame(u8 *frame)
{
    u8 *wl;
    u8 *sta;
    u8 *mlme;
    u8 *cfg;
    u8 *rx;
    u32 aid;
    u32 len;
    u16 state;
    u64 ts;
    u64 t1;
    u64 t0;
    Elements info;
    u64 interval;
    u64 q;

    wl = ROOT;
    W(wl, 0x570)++;
    sta = wl + 0x344;
    mlme = wl + 0x404;
    cfg = wl + 0x31c;
    rx = wl + 0x17c;
    aid = CAM_SearchAdd(frame + 0x1e);
    H(frame, 2) = (u16)aid;
    if (aid == 0xff)
        return;
    CAM_SetRSSI(aid, H(frame, 0x12));
    len = H(frame, 6);
    if (len <= 0xc)
        return;
    __VENEER_MIi_CpuClear32(0, &info, sizeof(info));
    info.elements = frame + 0x38;
    info.length = len - 0xc;
    info.valid = 2;
    if (H(sta, 0x1e) == 0)
        info.valid |= 1;
    info.present = 0x38;
    info.x04 = H(frame, 8);
    info.x06 = H(frame, 0x36);
    ElementChecker(&info);
    if (info.cf && (H(frame, 0x16) & 0x8000)) {
        u8 *cf = info.cf;
        V(0x0480810c) = WL_ReadByte(cf + 6) + (WL_ReadByte(cf + 7) << 8);
    }
    state = H(mlme, 0);
    if (state == 0x13 && H(P(mlme, 0x18), 0x38) == 1) {
        if ((info.valid & 9) == 9)
            RxProbeResFrame(frame, &info);
        goto tail;
    }
    if (!(info.valid & 8))
        goto tail;
    if (state == 0x21) {
        ClearTimeOut();
        if ((info.valid & 0x30) != 0x30) {
            H(mlme, 4) = 0xc;
            H(mlme, 6) = 0xa;
        } else if (H(frame, 0x34) > 1000) {
            H(mlme, 4) = 0xc;
            H(mlme, 6) = 1;
        } else {
            H(mlme, 4) = 0;
            if ((info.present & 2) && !(info.valid & 2))
                WSetChannel(info.channel, 0);
            CAM_SetSupRate(aid, info.rates);
            if (H(sta, 0xc) == 2) {
                u8 *v = info.vendor;
                if (v) {
                    u8 lo = WL_ReadByte(v + 6);
                    WSetActiveZoneTime(lo + (WL_ReadByte(v + 7) << 8), 1);
                    lo = WL_ReadByte(info.vendor + 8);
                    *(u16 *)0x0380fff0 = lo + (WL_ReadByte(info.vendor + 9) << 8);
                } else {
                    WSetActiveZoneTime(0xffff, 1);
                    *(u16 *)0x0380fff0 = 0;
                }
            }
            WSetDTIMPeriod(WL_ReadByte(info.tim + 3));
            H(sta, 0x76) = WL_ReadByte(info.tim + 2);
            WSetBeaconPeriod(H(frame, 0x34));
            H(sta, 0x12) = 1;
            H(sta, 0x1a) = 1;
            if (H(sta, 0xc) == 2)
                V(0x04808048) = 3;
            V(0x04808038) |= 1;
        }
        WSetMacAdrs1(P(mlme, 0x1c) + 8, frame + 0x1e);
        H(mlme, 0) = 0x25;
        AddTask(2, 1);
    }
    switch (H(sta, 0xc)) {
    case 2:
        if (info.vendor) {
            u8 *v = info.vendor;
            u8 lo = WL_ReadByte(v + 6);
            WSetActiveZoneTime(lo + (WL_ReadByte(v + 7) << 8), 0);
            lo = WL_ReadByte(info.vendor + 8);
            *(u16 *)0x0380fff0 = lo + (WL_ReadByte(info.vendor + 9) << 8);
            H(sta, 0xa0) = WL_ReadByte(info.vendor + 1) - 8;
            if (H(sta, 0xa0)) {
                if ((u32)info.vendor & 1) {
                    __VENEER_MIi_CpuCopy16(info.vendor + 9, P(sta, 0x9c), H(sta, 0xa0) + 2);
                    H(sta, 0xa2) = 1;
                } else {
                    __VENEER_MIi_CpuCopy16(info.vendor + 0xa, P(sta, 0x9c), H(sta, 0xa0) + 1);
                    H(sta, 0xa2) = 0;
                }
            }
        }
    case 3:
        H(sta, 0x80) = 0;
        CAM_UpdateLifeTime(aid);
        ts = *(u64 *)(frame + 0x2c);
        interval = (u32)(H(sta, 0x6e) << 10);
        q = __VENEER__ll_udiv(ts, interval) + 1;
        ts = q * interval;
        {
            u16 *p = (u16 *)&ts;
            V(0x048080f6) = p[3];
            V(0x048080f4) = p[2];
            V(0x048080f2) = p[1];
            V(0x048080f0) = p[0] | 1;
        }
        if (H(sta, 0xc) == 2 && H(sta, 0x1a)) {
            u32 e;
            u32 d;
            ts -= interval;
            e = __VENEER_OS_DisableInterrupts();
            ((u16 *)&t1)[0] = V(0x048080f8);
            ((u16 *)&t1)[1] = V(0x048080fa);
            ((u16 *)&t1)[2] = V(0x048080fc);
            ((u16 *)&t1)[3] = V(0x048080fe);
            ((u16 *)&t0)[0] = V(0x048080f8);
            ((u16 *)&t0)[1] = V(0x048080fa);
            ((u16 *)&t0)[2] = V(0x048080fc);
            ((u16 *)&t0)[3] = V(0x048080fe);
            __VENEER_OS_RestoreInterrupts(e);
            if (((u16 *)&t1)[0] < ((u16 *)&t0)[0])
                d = (t1 - ts) >> 10;
            else
                d = (t0 - ts) >> 10;
            if (d < H(cfg, 0x20))
                V(0x04808134) = H(cfg, 0x20) - d;
            else
                V(0x04808134) = 0;
        }
        if (H(sta, 8) == 0x40 && info.tim && H(sta, 0xe) == 1) {
            u32 count = WL_ReadByte(info.tim + 2);
            u32 base; u32 bc; u32 off; u32 end;
            if (H(sta, 0x76) != count)
                H(sta, 0x76) = count;
            H(sta, 0x8e) = 0;
            if (count == 0 && (WL_ReadByte(info.tim + 4) & 1))
                H(sta, 0x8e) |= 1;
            bc = WL_ReadByte(info.tim + 4) & 0xfe;
            base = bc * 8;
            end = bc + WL_ReadByte(info.tim + 1) - 3;
            if (base <= H(sta, 0x6a) && H(sta, 0x6a) <= end * 8) {
                off = H(sta, 0x6a) - base;
                if (WL_ReadByte(info.tim + 5 + (off >> 3)) & (1 << (off & 7))) {
                    H(sta, 0x8e) |= 2;
                    TxPsPollFrame();
                }
            }
            if (H(rx, 0x20) == 0 && H(rx, 0x2c) == 0 && H(sta, 0x8e) == 0)
                WSetPowerState(1);
        }
        break;
    }
    if (((CfgFlags *)(cfg + 0x1e))->beaconInd)
        MLME_IssueBeaconRecvIndication(frame);
tail:
    if (info.ssid)
        UpdateApList(info.channel, frame, info.ssid);
}
