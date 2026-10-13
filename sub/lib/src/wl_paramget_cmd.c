#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// PARAMGET command handlers (WlibCmdTbl_PARAMGET / PARAMGET2): copy driver parameters into the confirm.

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Confirm: MAC address, retry limit, enabled channels, channel, mode, rate, WEP mode, WEP key id, beacon type,
// broadcast SSID response, beacon lost threshold, active zone time, SSID mask, preamble type, auth algorithm.
int PARAMGET_AllReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 33;
    WSetMacAdrs1(cfm->param, wlWork->param.macAddress);
    cfm->param[3] = wlWork->param.retryLimit;
    cfm->param[4] = wlWork->param.enableChannel;
    cfm->param[5] = wlWork->bss.channel;
    cfm->param[6] = wlWork->param.mode;
    cfm->param[7] = wlWork->param.rate;
    cfm->param[8] = wlWork->param.wepMode;
    cfm->param[9] = wlWork->param.wepKeyId;
    cfm->param[10] = wlWork->param.flags.bits.beaconType;
    cfm->param[11] = wlWork->param.flags.bits.bcSsidResponse;
    cfm->param[12] = wlWork->bss.beaconLostThreshold;
    cfm->param[13] = wlWork->param.activeZoneTime;
    __VENEER_MIi_CpuCopy16(wlWork->bss.ssidMask, cfm->param + 14, 32);
    cfm->param[30] = wlWork->param.flags.bits.preambleType;
    cfm->param[31] = wlWork->param.authAlgo;
    return 0;
}

int PARAMGET_MacAdrsReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 4;
    WSetMacAdrs1(cfm->param, wlWork->param.macAddress);
    return 0;
}

int PARAMGET_RetryReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.retryLimit;
    return 0;
}

int PARAMGET_EnableChannelReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 3;
    cfm->param[0] = wlWork->param.enableChannel;
    cfm->param[1] = wlWork->bss.channel;
    return 0;
}

int PARAMGET_ModeReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.mode;
    return 0;
}

int PARAMGET_RateReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.rate;
    return 0;
}

int PARAMGET_WepModeReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.wepMode;
    return 0;
}

int PARAMGET_WepKeyIdReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.wepKeyId;
    return 0;
}

int PARAMGET_BeaconTypeReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.flags.bits.beaconType;
    return 0;
}

int PARAMGET_ResBcSsidReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.flags.bits.bcSsidResponse;
    return 0;
}

int PARAMGET_BeaconLostThReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->bss.beaconLostThreshold;
    return 0;
}

int PARAMGET_ActiveZoneReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.activeZoneTime;
    return 0;
}

int PARAMGET_SSIDMaskReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    u16 *dst, *src;
    unsigned i;
    cfm->header.length = 17;
    dst = cfm->param;
    src = wlWork->bss.ssidMask;
    for (i = 0; i < 16; i++) {
        *dst++ = *src++;
    }
    return 0;
}

int PARAMGET_PreambleTypeReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.flags.bits.preambleType;
    return 0;
}

int PARAMGET_AuthAlgoReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.authAlgo;
    return 0;
}

// Confirm: CCA mode, ED threshold (BBP registers 0x13, 0x35) and BBP register 0x2E.
int PARAMGET_CCAModeEDThReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 4;
    cfm->param[0] = BBP_Read(0x13);
    cfm->param[1] = BBP_Read(0x35);
    cfm->param[2] = BBP_Read(0x2E);
    return 0;
}

int PARAMGET_MaxConnReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->param.maxSta - 1;
    return 0;
}

int PARAMGET_MainAntennaReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    u16 state;
    WlWork *w;
    cfm->header.length = 2;
    w = wlWork;
    state = w->bss.state;
    if (state < 16) {
        return 1;
    }
    cfm->param[0] = w->param.flags.bits.mainAntenna;
    return 0;
}

// Confirm: diversity, antenna in use.
int PARAMGET_DiversityReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    u16 state;
    WlWork *w;
    WlWork *w2;
    cfm->header.length = 3;
    w = wlWork;
    state = w->bss.state;
    if (state < 16) {
        return 1;
    }
    cfm->param[0] = w->param.flags.bits.diversity;
    w2 = wlWork;
    cfm->param[1] = w2->param.flags.bits.otherAntenna ^ w2->param.flags.bits.mainAntenna;
    return 0;
}

int PARAMGET_BcnSendRecvIndReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    u16 state;
    WlWork *w;
    cfm->header.length = 2;
    w = wlWork;
    state = w->bss.state;
    if (state < 16) {
        return 1;
    }
    cfm->param[0] = w->param.flags.bits.beaconIndicate;
    return 0;
}

int PARAMGET_NullKeyModeReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    u16 state;
    WlWork *w;
    cfm->header.length = 2;
    w = wlWork;
    state = w->bss.state;
    if (state < 16) {
        return 1;
    }
    cfm->param[0] = w->param.flags.bits.nullKeyMode;
    return 0;
}

int PARAMGET_BSSIDReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 4;
    WSetMacAdrs1(cfm->param, wlWork->bss.bssid);
    return 0;
}

// Confirm: SSID length, SSID (32 bytes).
int PARAMGET_SSIDReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    u16 *dst, *src;
    u32 i;
    cfm->header.length = 18;
    cfm->param[0] = wlWork->bss.ssidLength;
    dst = cfm->param + 1;
    src = (u16 *)wlWork->bss.ssid;
    i = 0;
    do {
        *dst++ = *src++;
        i += 2;
    } while (i < 32);
    return 0;
}

int PARAMGET_BeaconPeriodReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->bss.beaconPeriod;
    return 0;
}

int PARAMGET_DTIMPeriodReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->bss.dtimPeriod;
    return 0;
}

int PARAMGET_ListenIntervalReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    cfm->header.length = 2;
    cfm->param[0] = wlWork->bss.listenInterval;
    return 0;
}

// Confirm: game information length (bytes), game information; 4 when the confirm is too small.
int PARAMGET_GameInfoReqCmd(WlParamReq *req, WlParamCfm *cfm) {
    WlWork *w;
    u8 *src, *dst;
    u32 i;
    if (cfm->header.length > 1) {
        cfm->param[0] = wlWork->bss.gameInfoLength;
    }
    w = wlWork;
    if ((int)w->bss.gameInfoLength > ((int)cfm->header.length - 2) * 2) {
        return 4;
    }
    if (cfm->param[0]) {
        if (w->bss.gameInfoOdd & 1) {
            src = w->bss.gameInfo;
            dst = (u8 *)(cfm->param + 1);
            src++;
            for (i = 0; i < cfm->param[0]; i++) {
                WL_WriteByte(dst, WL_ReadByte(src));
                dst++;
                src++;
            }
        } else {
            __VENEER_MIi_CpuCopy16(w->bss.gameInfo, cfm->param + 1, cfm->param[0] + 1);
        }
    }
    cfm->header.length = ((int)cfm->param[0] + 1) / 2 + 2;
    return 0;
}

#pragma dont_inline reset
