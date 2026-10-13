#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#pragma push
#pragma opt_propagation off

// request[1]: power save mode. Starts the parent (MlmeStart) with the parameters of
// WMSP_SetParentParam; state 7 on success.
void WMSP_StartParent(u32 *request) {
    struct {
        u16 ssid[16];
        u16 gameInfo[0x40];
        u16 buf[256];
    } loc;

    WmspStatus *st = wmspW.status;
    u16 *buf = loc.buf;
    WmspParentParam *pp;
    u16 *r;
    u16 *res;
    u32 pwr;
    u16 pm;
    u32 e;
    u16 len;
    if (st->state != 2 || (st->miscFlags & 1)) {
        res = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        res[0] = 8;
        res[1] = 3;
        res[4] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(res);
        return;
    }
    pwr = request[1];
    if (!(st->allowedChannel & ((1 << st->parentParam.channel) >> 1))) {
        res = __VENEER_WMSP_GetBuffer4Callback2Wm9();
        res[0] = 8;
        res[1] = 6;
        res[4] = 0;
        __VENEER_WMSP_ReturnResult2Wm9(res);
        return;
    }
    st->mode = 1;
    st->aid = 0;
    e = __VENEER_OS_DisableInterrupts();
    st->childBitmap = 0;
    st->readyBitmap = 0;
    __VENEER_OS_RestoreInterrupts((OSIntrMode)e);
    st->preamble = 1;
    if (!WMSP_SetAllParams(8, (u8 *)buf)) {
        return;
    }
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
    pp = &st->parentParam;
    __VENEER_MIi_CpuClear16(0, loc.gameInfo, sizeof(loc.gameInfo));
    WMSP_CopyParentParam((u8 *)loc.gameInfo, (u8 *)pp);
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
    WMSP_SetParentMaxSize((u16)(pp->parentMaxSize + (pp->KS_Flag ? 0x2a : 0)));
    WMSP_SetChildMaxSize((u16)(pp->childMaxSize + (pp->KS_Flag ? 6 : 0)));
    res = __VENEER_WMSP_GetBuffer4Callback2Wm9();
    st->state = 7;
    res[0] = 8;
    res[1] = 0;
    res[4] = 0;
    res[0x16] = st->parentSize;
    res[0x17] = st->childSize;
    __VENEER_WMSP_ReturnResult2Wm9(res);
    st->beaconIndicateFlag = 1;
}

#pragma pop
