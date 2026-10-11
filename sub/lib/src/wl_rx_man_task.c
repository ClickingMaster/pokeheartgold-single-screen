#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define ROOT    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))

typedef struct {
    u16 ver : 2;
    u16 type : 2;
    u16 subtype : 4;
    u16 toDs : 1;
    u16 fromDs : 1;
    u16 moreFrag : 1;
    u16 retry : 1;
    u16 pwrMgt : 1;
    u16 moreData : 1;
    u16 wep : 1;
    u16 order : 1;
} FrameCtrl;

typedef struct {
    u16 ess : 1;
    u16 ibss : 1;
    u16 poll : 2;
    u16 wep : 1;
    u16 preamble : 1;
    u16 reserved : 10;
} CapInfo;

typedef struct {
    u8 *elements;
    u16 capability;
    u16 request;
    u16 length;
    u16 valid;
    u16 present;
    u16 x0e;
    u16 x10;
    u16 channel;
    u16 supported;
    u16 rates;
    u16 x18;
    u16 x1a;
    u8 *ssid;
    u8 *cf;
    u8 *tim;
    u8 *vendor;
} Elements;

extern int MatchMacAdrs(const void *, const void *);
extern int IsExistManFrame(const void *, u16);
extern u32 CAM_SearchAdd(const void *);
extern u32 CAM_Search(const void *);
extern void CAM_UpdateLifeTime(u32);
extern void CAM_SetRSSI(u16, u16);
extern u32 CAM_GetLastSeqCtrl(u32);
extern void CAM_SetLastSeqCtrl(u32, u32);
extern void CAM_SetPowerMgtMode(u16, u32);
extern u32 CAM_GetStaState(u32);
extern void CAM_SetStaState(u16, u32);
extern u32 CAM_GetAID(u32);
extern void CAM_SetCapaInfo(u32, u32);
extern void CAM_SetSupRate(u32, u32);
extern void CAM_SetAwake(u32);
extern void *MakeDeAuthFrame(const void *, u16, u32);
extern void *MakeReAssResFrame(u32, u32, const void *);
extern void TxManCtrlFrame(void *);
extern void MLME_IssueDisAssIndication(const void *, u32);
extern void __VENEER_MIi_CpuClear32(u32, void *, u32);
extern void ElementChecker(Elements *);
extern void RxAssReqFrame(void *);
extern void RxProbeReqFrame(void *);
extern void RxProbeResFrame(void *, void *);
extern void RxBeaconFrame(void *);
extern void RxDisAssFrame(void *);
extern void RxAuthFrame(void *);
extern void RxDeAuthFrame(void *);
extern void TxqPri(u32);
extern void ClearTimeOut(void);
extern void WSetAids(u16);
extern void MakePsPollFrame(u16);
extern void WSetMacAdrs1(void *, const void *);
extern void WSetStaState(u32);
extern void AddTask(u32, u32);
extern void ReleaseHeapBuf(void *, void *);
void RxManCtrlTask(void);

static inline void RxReAssReqFrame(u8 *buf) {
    u8 *wl = ROOT;
    u32 len = H(buf, 0x16);
    u16 aid;
    u32 reason;
    void *f;
    Elements info;
    if (len <= 0xa) {
        return;
    }
    if (H(wl, 0x350) != 1) {
        return;
    }
    if (IsExistManFrame(buf + 0x2e, 0x30)) {
        return;
    }
    aid = H(buf, 0x12);
    if (CAM_GetStaState(aid) < 0x30) {
        if (IsExistManFrame(buf + 0x2e, 0xc0)) {
            return;
        }
        f = MakeDeAuthFrame(buf + 0x2e, 6, 1);
        if (f) {
            TxManCtrlFrame(f);
        }
        return;
    }
    if (CAM_GetStaState(aid) == 0x40) {
        CAM_SetStaState(aid, 0x30);
        MLME_IssueDisAssIndication(buf + 0x2e, 1);
    } else if (CAM_GetAID(aid)) {
        return;
    }
    __VENEER_MIi_CpuClear32(0, &info, sizeof(info));
    info.elements = buf + 0x46;
    info.length = len - 0xa;
    info.present = 0x800;
    ElementChecker(&info);
    {
        u16 cap = H(buf, 0x3c);
        u16 wep;
        if ((cap & 0xffc2) || ((wep = H(wl, 0x334)) == 0 && ((CapInfo *)(buf + 0x3c))->wep == 1) || (wep != 0 && ((CapInfo *)(buf + 0x3c))->wep == 0)) {
            reason = 10;
        } else {
            CAM_SetCapaInfo(aid, cap);
            if (!(info.valid & 1)) {
                reason = 1;
            } else if (!(info.valid & 4)) {
                reason = 0x12;
            } else {
                CAM_SetSupRate(aid, info.rates);
                reason = 0;
            }
        }
    }
    f = MakeReAssResFrame(aid, reason, info.ssid);
    if (f) {
        TxManCtrlFrame(f);
    }
}

static inline void RxPsPollFrame(u8 *buf) {
    u8 *rx;
    u32 aid = H(buf, 0x12);
    rx = ROOT + 0x17c;
    if (CAM_GetStaState(aid) != 0x40) {
        return;
    }
    CAM_SetAwake(aid);
    if (H(rx, 0x2c)) {
        TxqPri(1);
    }
    if (H(rx, 0x20)) {
        TxqPri(0);
    }
}

static inline void RxAssResFrame(u8 *buf) {
    u8 *wl = ROOT;
    u8 *sta = wl + 0x344;
    u8 *mlme = wl + 0x404;
    switch (H(sta, 0xc)) {
    case 2:
    case 3:
        break;
    default:
        return;
    }
    if (H(mlme, 0) != 0x51) {
        return;
    }
    if (!MatchMacAdrs(P(mlme, 0x18) + 0x10, buf + 0x2e)) {
        return;
    }
    ClearTimeOut();
    if (H(buf, 0x3e) == 0) {
        WSetAids(H(buf, 0x40) & 0xfff);
        MakePsPollFrame(H(sta, 0x6a));
        WSetMacAdrs1(sta + 0x82, buf + 0x2e);
        H(sta, 0x88) = CAM_Search(buf + 0x2e);
        CAM_SetStaState(H(sta, 0x88), 0x40);
    }
    if (H(buf, 0x3e) == 0) {
        H(P(mlme, 0x1c), 4) = 0;
        H(P(mlme, 0x1c), 6) = 0;
        WSetStaState(0x40);
    } else {
        H(P(mlme, 0x1c), 4) = 0xc;
        H(P(mlme, 0x1c), 6) = H(buf, 0x3e);
    }
    H(P(mlme, 0x1c), 8) = H(sta, 0x6a);
    H(mlme, 0) = 0x53;
    AddTask(2, 3);
}

static inline void RxReAssResFrame(u8 *buf) {
    u8 *wl = ROOT;
    u8 *sta = wl + 0x344;
    u8 *mlme = wl + 0x404;
    switch (H(sta, 0xc)) {
    case 2:
    case 3:
        break;
    default:
        return;
    }
    if (H(mlme, 0) != 0x61) {
        return;
    }
    if (!MatchMacAdrs(P(mlme, 0x18) + 0x10, buf + 0x2e)) {
        return;
    }
    ClearTimeOut();
    if (H(buf, 0x3e) == 0) {
        WSetAids(H(buf, 0x40) & 0xfff);
        MakePsPollFrame(H(sta, 0x6a));
        WSetMacAdrs1(sta + 0x82, buf + 0x2e);
        H(sta, 0x88) = CAM_Search(buf + 0x2e);
        CAM_SetStaState(H(sta, 0x88), 0x40);
        WSetStaState(0x40);
    }
    if (H(buf, 0x3e) == 0) {
        H(P(mlme, 0x1c), 4) = 0;
        H(P(mlme, 0x1c), 6) = 0;
        WSetStaState(0x40);
    } else {
        H(P(mlme, 0x1c), 4) = 0xc;
        H(P(mlme, 0x1c), 6) = H(buf, 0x3e);
    }
    H(P(mlme, 0x1c), 8) = H(sta, 0x6a);
    H(mlme, 0) = 0x63;
    AddTask(2, 4);
}

void RxManCtrlTask(void) {
    u8 *wl = ROOT;
    u8 *rx = wl + 0x17c;
    u16 mode;
    u8 *buf;
    buf = P(rx, 0x60);
    mode = H(wl, 0x350);
    u32 aid, type, subtype;
    u16 seq;

    if (buf == (u8 *)0xffffffff) {
        return;
    }
    if (H(buf, 0x28) & 1) {
        W(wl, 0x568)
        ++;
    } else {
        W(wl, 0x564)
        ++;
    }
    W(wl, 0x560) += (int)(H(buf, 0x18) & 0xf0) / 16 - 1;
    type = ((FrameCtrl *)(buf + 0x24))->type;
    subtype = ((FrameCtrl *)(buf + 0x24))->subtype;
    aid = CAM_SearchAdd(buf + 0x2e);
    H(buf, 0x12) = aid;
    if (aid == 0xff) {
        H(buf, 0x12) = 0;
        if (mode == 1) {
            switch (type) {
            case 0:
                switch (subtype) {
                case 0xb:
                    RxAuthFrame(buf + 0x10);
                    break;
                case 4:
                    RxProbeReqFrame(buf + 0x10);
                    break;
                case 0:
                    RxAssReqFrame(buf + 0x10);
                    break;
                }
                break;
            }
        }
        goto end;
    }
    CAM_UpdateLifeTime(aid);
    CAM_SetRSSI(aid, H(buf, 0x22) & 0xff);
    if (type == 0) {
        seq = H(buf, 0x3a);
        if (seq == CAM_GetLastSeqCtrl(aid)) {
            W(wl, 0x578)
            ++;
            goto end;
        }
        CAM_SetLastSeqCtrl(aid, seq);
    }
    switch (mode) {
    case 1:
        CAM_SetPowerMgtMode(aid, ((FrameCtrl *)(buf + 0x24))->pwrMgt);
        if (type == 0) {
            switch (subtype) {
            case 8:
                RxBeaconFrame(buf + 0x10);
                break;
            case 0:
                RxAssReqFrame(buf + 0x10);
                break;
            case 2:
                RxReAssReqFrame(buf);
                break;
            case 4:
                RxProbeReqFrame(buf + 0x10);
                break;
            case 5:
                RxProbeResFrame(buf + 0x10, NULL);
                break;
            case 10:
                RxDisAssFrame(buf + 0x10);
                break;
            case 11:
                RxAuthFrame(buf + 0x10);
                break;
            case 12:
                RxDeAuthFrame(buf + 0x10);
                break;
            }
        } else if (type == 1) {
            switch (subtype) {
            case 10:
                RxPsPollFrame(buf);
                break;
            }
        }
        break;
    case 2:
    case 3:
        if (type == 0) {
            switch (subtype) {
            case 8:
                RxBeaconFrame(buf + 0x10);
                break;
            case 1:
                RxAssResFrame(buf);
                break;
            case 3:
                RxReAssResFrame(buf);
                break;
            case 5:
                RxProbeResFrame(buf + 0x10, NULL);
                break;
            case 10:
                RxDisAssFrame(buf + 0x10);
                break;
            case 11:
                RxAuthFrame(buf + 0x10);
                break;
            case 12:
                RxDeAuthFrame(buf + 0x10);
                break;
            }
        }
        break;
    }
end:
    ReleaseHeapBuf(rx + 0x60, buf);
    if (H(rx, 0x68)) {
        AddTask(1, 7);
    }
}
