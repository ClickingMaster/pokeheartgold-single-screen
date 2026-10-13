#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// PARAMSET command handlers (WlibCmdTbl_PARAMSET / PARAMSET2): set one driver parameter through its WSet*
// function (1: not allowed in the current state, 5: invalid, 11: wrong mode).

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

int PARAMSET_AllReqCmd(WlParamSetAllReq *req, WlCmdCfm *cfm) {
    int r;
    cfm->length = 1;
    if (wlWorkPlain->bss.state != 16) {
        return 1;
    }
    r = WSetMacAdrs(req->macAdrs);
    r |= WSetRetryLimit(req->retryLimit);
    r |= WSetEnableChannel(req->enableChannel);
    r |= WSetMode(req->mode);
    r |= WSetRate(req->rate);
    r |= WSetWepMode(req->wepMode);
    r |= WSetWepKeyId(req->wepKeyId);
    r |= WSetWepKey(req->wepKey);
    r |= WSetBeaconType(req->beaconType);
    r |= WSetBcSsidResponse(req->bcSsidResponse);
    r |= WSetBeaconLostThreshold(req->beaconLostThreshold);
    r |= WSetActiveZoneTime(req->activeZoneTime, 0);
    r |= WSetSsidMask(req->ssidMask);
    r |= WSetPreambleType(req->preambleType);
    r |= WSetAuthAlgo(req->authAlgo);
    return r;
}

int PARAMSET_MacAdrsReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    int r = 1;
    cfm->length = 1;
    if (wlWorkPlain->bss.state == 16) {
        r = WSetMacAdrs(req->param);
    }
    return r;
}

u32 PARAMSET_RetryReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetRetryLimit(req->param[0]);
}

u32 PARAMSET_EnableChannelReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    u32 result = 1;
    u16 state;
    cfm->length = 1;
    state = wlWork->bss.state;
    if (state == 16) {
        result = WSetEnableChannel(req->param[0]);
    }
    return result;
}

u32 PARAMSET_ModeReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    WlWork *w = wlWork;
    u16 state = w->bss.state;
    if (state > 32) {
        return 1;
    }
    if (state == 32 && w->bss.joined != 0) {
        return 1;
    }
    return WSetMode(req->param[0]);
}

u32 PARAMSET_RateReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetRate(req->param[0]);
}

u32 PARAMSET_WepModeReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetWepMode(req->param[0]);
}

u32 PARAMSET_WepKeyIdReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetWepKeyId(req->param[0]);
}

u32 PARAMSET_WepKeyReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetWepKey((const u8 *)req->param);
}

u32 PARAMSET_BeaconTypeReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    u32 result = 1;
    u16 state;
    cfm->length = 1;
    state = wlWork->bss.state;
    if (state <= 32) {
        result = WSetBeaconType(req->param[0]);
    }
    return result;
}

u32 PARAMSET_ResBcSsidReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetBcSsidResponse(req->param[0]);
}

u32 PARAMSET_BeaconLostThReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetBeaconLostThreshold(req->param[0]);
}

u32 PARAMSET_ActiveZoneReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetActiveZoneTime(req->param[0], 0);
}

u32 PARAMSET_SSIDMaskReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetSsidMask(req->param);
}

u32 PARAMSET_PreambleTypeReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetPreambleType(req->param[0]);
}

u32 PARAMSET_AuthAlgoReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetAuthAlgo(req->param[0]);
}

// param: CCA mode, ED threshold, BBP register 46 value (0..63).
u32 PARAMSET_CCAModeEDThReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    u32 r;
    if (req->param[2] > 63) {
        return 5;
    }
    r = WSetCCA_ED(req->param[0], req->param[1]);
    if (r == 0) {
        BBP_Write(46, req->param[2]);
    }
    return r;
}

// param: station (0xFFFF: all), its life time, the frame life time (0: unchanged).
u32 PARAMSET_LifeTimeReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    WlStation *table = wlWork->param.camTable;
    u32 i;
    u16 index;
    cfm->length = 1;
    index = req->param[0];
    if (index >= wlWork->param.maxSta && index != 65535) {
        return 5;
    }
    if (req->param[2] > 63 && req->param[2] != 65535) {
        return 5;
    }
    if (index == 65535) {
        for (i = 1; i < wlWork->param.maxSta; i++) {
            WlStation *sta = &table[i];
            sta->lifeTime = req->param[1];
            if (sta->timer) {
                sta->timer = req->param[1];
            }
        }
    } else if (index) {
        u16 *timers = &table->timer;
        table[index].lifeTime = req->param[1];
        if (*(u16 *)((u8 *)timers + req->param[0] * sizeof(WlStation))) {
            *(u16 *)((u8 *)timers + req->param[0] * sizeof(WlStation)) = req->param[1];
        }
    }
    if (req->param[2]) {
        WSetFrameLifeTime(req->param[2]);
    }
    return 0;
}

// param: the number of child stations (the broadcast entry is added).
u32 PARAMSET_MaxConnReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    WlWork *w = wlWork;
    if (w->bss.state > 32) {
        return 1;
    }
    req->param[0]++;
    w = wlWork;
    if (req->param[0] > w->param.camTableSize) {
        return 5;
    }
    w->param.maxSta = req->param[0];
    return 0;
}

u32 PARAMSET_MainAntennaReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    u32 result = 1;
    u16 state;
    cfm->length = 1;
    state = wlWork->bss.state;
    if (state >= 16) {
        result = WSetMainAntenna(req->param[0]);
    }
    return result;
}

u32 PARAMSET_DiversityReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    if (wlWork->bss.state < 16) {
        return 1;
    }
    return WSetDiversity(req->param[0], req->param[1]);
}

u32 PARAMSET_BcnSendRecvIndReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    u32 result = 1;
    u16 state;
    cfm->length = 1;
    state = wlWork->bss.state;
    if (state >= 16) {
        result = WSetBeaconSendRecvIndicate(req->param[0]);
    }
    return result;
}

u32 PARAMSET_NullKeyModeReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    u32 result = 1;
    u16 state;
    cfm->length = 1;
    state = wlWork->bss.state;
    if (state >= 16) {
        result = WSetNullKeyMode(req->param[0]);
    }
    return result;
}

u32 PARAMSET_BSSIDReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetBssid(req->param);
}

// param: SSID length, SSID.
u32 PARAMSET_SSIDReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    return WSetSsid(req->param[0], (const u8 *)(req->param + 1));
}

u32 PARAMSET_BeaconPeriodReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    if (wlWork->param.mode != 1) {
        return 11;
    }
    return WSetBeaconPeriod(req->param[0]);
}

int PARAMSET_DTIMPeriodReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    if (wlWork->param.mode != 1) {
        return 11;
    }
    return WSetDTIMPeriod(req->param[0]);
}

int PARAMSET_ListenIntervalReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    int mode;
    cfm->length = 1;
    mode = wlWork->param.mode;
    if (mode != 2 && mode != 3) {
        return 11;
    }
    return WSetListenInterval(req->param[0]);
}

// param: game information length (bytes), game information.
int PARAMSET_GameInfoReqCmd(WlParamReq *req, WlCmdCfm *cfm) {
    int length;
    cfm->length = 1;
    if (wlWork->param.mode != 1) {
        return 11;
    }
    length = req->param[0];
    if (req->header.length < ((length + 1) / 2 + 1)) {
        return 4;
    }
    return WSetGameInfo(length, (const u8 *)(req->param + 1));
}

#pragma dont_inline reset
