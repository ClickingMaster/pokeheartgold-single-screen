#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {
    u8 pad0[4];
    u16 userGameInfoLength; /* 0x04 */
    u8 pad6[2];
    u32 ggid;              /* 0x08 */
    u16 tgid;              /* 0x0c */
    u8 padE[0x14 - 0xe];
    u16 entryFlag;         /* 0x14 */
    u8 pad16[2];
    u16 beaconPeriod;      /* 0x18 */
    u8 pad1a[0x32 - 0x1a];
    u16 channel;           /* 0x32 */
    u16 parentMaxSize;     /* 0x34 */
    u16 childMaxSize;      /* 0x36 */
} WmspParentParam;
typedef struct {
    u16 state;             /* 0x00 */
    u8 pad2[0x30 - 2];
    u16 x30;               /* 0x30 */
    u16 x32;               /* 0x32 */
    u8 pad34[0x86 - 0x34];
    u16 x86;               /* 0x86 */
    u8 pad88[0xc2 - 0x88];
    u16 xc2;               /* 0xc2 */
    u8 padc4[2];
    u16 pwrMgtMode;        /* 0xc6 */
    u32 xc8;               /* 0xc8 */
    u8 padcc[0xe6 - 0xcc];
    u16 xe6;               /* 0xe6 */
    WmspParentParam pparam; /* 0xe8 */
    u8 pad120[0x182 - 0x120];
    u16 childBitmap;       /* 0x182 */
    u8 pad184[0x188 - 0x184];
    u16 aid;               /* 0x188 */
    u8 pad18a[0x1ee - 0x18a];
    u16 x1ee;              /* 0x1ee */
    u8 pad1f0[0x1f6 - 0x1f0];
    u16 allowedChannel;    /* 0x1f6 */
} WmspStatus;
typedef struct { u8 pad[0x1550]; WmspStatus *status; } WmspWork;
extern WmspWork wmspW;
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *);
extern void __VENEER_MIi_CpuClear16(u16 data, void *dst, u32 size);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern int WMSP_SetAllParams(u16 cmd, u16 *buf);
extern u16 *WMSP_WL_DevClass1(u16 *buf);
extern u16 *WMSP_WL_MlmePowerManagement(u16 *buf, u16 pwrMgtMode, u16 wakeUp, u16 recieveDtims);
extern u16 *WMSP_WL_MlmeStart(u16 *buf, u16 ssidLength, u16 *ssid, u16 beaconPeriod, u16 dtimPeriod, u16 channel, u16 basicRateSet, u16 supportRateSet, u16 gameInfoLength, void *gameInfo);
extern void WMSP_CopyParentParam(void *dst, WmspParentParam *src);
extern void WMSP_SetParentMaxSize(u16);
extern void WMSP_SetChildMaxSize(u16);
extern void WmspError_060033F4(u16 code, u16 result);
void WMSP_StartParent(void *msg);
#pragma push
#pragma opt_propagation off
// @fn WMSP_StartParent
void WMSP_StartParent(void *msg)
{
    struct { u16 ssid[16]; u16 gameInfo[0x40]; u16 buf[256]; } loc;
    WmspStatus *st = wmspW.status;
    u16 *buf = loc.buf;
    WmspParentParam *pp;
    u16 *r;
    u16 *res;
    u32 pwr;
    u16 pm;
    u32 e;
    u16 len;
    if (st->state != 2 || (st->xc8 & 1)) {
        res = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        res[0] = 8; res[1] = 3; res[4] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(res);
        return;
    }
    pwr = ((u32 *)msg)[1];
    if (!(st->allowedChannel & ((1 << st->pparam.channel) >> 1))) {
        res = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        res[0] = 8; res[1] = 6; res[4] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(res);
        return;
    }
    st->xe6 = 1;
    st->aid = 0;
    e = __VENEER_OS_DisableInterrupts();
    st->childBitmap = 0;
    st->x86 = 0;
    __VENEER_OS_RestoreInterrupts(e);
    st->x1ee = 1;
    if (!WMSP_SetAllParams(8, buf))
        return;
    r = WMSP_WL_DevClass1(buf);
    if (r[2] != 0) {
        WmspError_060033F4(0x303, r[2]);
        return;
    }
    pm = (pwr != 0) ? 1 : 0;
    r = WMSP_WL_MlmePowerManagement(buf, pm, 0, 1);
    if (r[2] != 0) {
        WmspError_060033F4(1, r[2]);
        return;
    }
    st->pwrMgtMode = pm;
    pp = &st->pparam;
    __VENEER_MIi_CpuClear16(0, loc.gameInfo, sizeof(loc.gameInfo));
    WMSP_CopyParentParam(loc.gameInfo, pp);
    __VENEER_MIi_CpuClear16(0, loc.ssid, sizeof(loc.ssid));
    loc.ssid[0] = (u16)pp->ggid;
    loc.ssid[1] = (u16)(pp->ggid >> 16);
    loc.ssid[2] = pp->tgid;
    loc.ssid[3] = 0;
    len = 0x20;
    r = WMSP_WL_MlmeStart(buf, len, loc.ssid, pp->beaconPeriod, 2, pp->channel, 3, 3, (u16)(pp->userGameInfoLength + 0x10), loc.gameInfo);
    if (r[2] != 0) {
        WmspError_060033F4(9, r[2]);
        return;
    }
    WMSP_SetParentMaxSize((u16)(pp->parentMaxSize + (pp->entryFlag ? 0x2a : 0)));
    WMSP_SetChildMaxSize((u16)(pp->childMaxSize + (pp->entryFlag ? 6 : 0)));
    res = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    st->state = 7;
    res[0] = 8; res[1] = 0; res[4] = 0;
    res[0x16] = st->x30;
    res[0x17] = st->x32;
    __VENEER_WMSP_ReturnResult2Wm9(res);
    st->xc2 = 1;
}
#pragma pop
