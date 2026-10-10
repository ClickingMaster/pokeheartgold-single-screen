#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
u16 CAM_GetStaState(u32), CAM_GetAID(u32);
void CAM_SetStaState(u32, u32), CAM_SetCapaInfo(u32, u16), CAM_SetSupRate(u32, u16);
int IsExistManFrame(const void *, u16);
u8 *MakeDeAuthFrame(const void *, u16, u32), *MakeAssResFrame(u32, u32, const void *);
void TxManCtrlFrame(void *), ElementChecker(void *), __VENEER_MIi_CpuClear32(u32, u32 *, u32);
int MLME_IssueDisAssIndication(const void *, int);

typedef struct {
    u8 *elements;
    u16 capability, request, length, valid, present, reserved0, reserved1, channel, supported, rates, unknown, total;
    u8 *ssid, *cf, *tim, *vendor;
} Elements40765;

typedef struct {
    u16 ess : 1, ibss : 1, poll : 2, wep : 1, preamble : 1, reserved : 10;
} Cap40765;

typedef char Elements40765Size[(sizeof(Elements40765) == 44) ? 1 : -1];
void RxAssReqFrame(u8 *);
#pragma opt_common_subs off

void RxAssReqFrame(u8 *frame) {
    u8 *w = BASE;
    u8 *s = w + 0x31c;
    u32 length = H(frame, 6), station;
    u32 reason;
    u8 *reply;
    Elements40765 info;
    if (length <= 4 || H(w, 0x350) != 1) {
        return;
    }
    if (IsExistManFrame(frame + 0x1e, 0x10)) {
        return;
    }
    station = H(frame, 2);
    if (!station || CAM_GetStaState(station) < 48) {
        if (IsExistManFrame(frame + 0x1e, 0xc0)) {
            return;
        }
        reply = MakeDeAuthFrame(frame + 0x1e, 6, 1);
        if (reply) {
            TxManCtrlFrame(reply);
        }
        return;
    }
    if (CAM_GetStaState(station) == 64) {
        CAM_SetStaState(station, 48);
        MLME_IssueDisAssIndication(frame + 0x1e, 1);
    } else if (CAM_GetAID(station)) {
        return;
    }
    __VENEER_MIi_CpuClear32(0, (u32 *)&info, 44);
    info.elements = frame + 0x30;
    info.length = length - 4;
    ElementChecker(&info);
    {
        u16 cap = H(frame, 0x2c), wep;
        if ((cap & 0xffc2) || !((Cap40765 *)(frame + 0x2c))->ess || ((wep = H(s, 0x18)) == 0 && ((Cap40765 *)(frame + 0x2c))->wep == 1) || (wep != 0 && ((Cap40765 *)(frame + 0x2c))->wep == 0) || ((((u32)H(s, 0x1e) << 29) >> 31) == 1 && !((Cap40765 *)(frame + 0x2c))->preamble)) {
            reason = 10;
        } else {
            CAM_SetCapaInfo(station, cap);
            if (!(info.valid & 1)) {
                reason = 1;
            } else if (!(info.valid & 4)) {
                reason = 18;
            } else {
                CAM_SetSupRate(station, info.rates);
                reason = 0;
            }
        }
    }
    reply = MakeAssResFrame(station, reason, info.ssid);
    if (reply) {
        TxManCtrlFrame(reply);
    }
}
