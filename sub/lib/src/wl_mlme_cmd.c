#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// MLME command handlers (WlibCmdTbl_MLME). They check the parameters (5: invalid, 1: wrong state,
// 11: wrong mode, 14: bad flash checksum) and start the request; 0x80: the task sends the confirm later.

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

int MLME_ResetReqCmd(WlMlmeResetReq *req, WlCmdCfm *cfm) {
    cfm->length = 1;
    if (req->initCounter > 1) {
        return 5;
    }
    WStop();
    if (req->initCounter == 1) {
        WInitCounter();
    }
    return 0;
}

int MLME_PwrMgtReqCmd(WlMlmePwrMgtReq *req, WlCmdCfm *cfm) {
    cfm->length = 9;
    if (req->pwrMgtMode > 1) {
        return 5;
    }
    if (req->wakeUp > 1) {
        return 5;
    }
    if (req->receiveDtims > 1) {
        return 5;
    }
    WSetPowerMgtMode(req->pwrMgtMode);
    if (req->pwrMgtMode == 1) {
        if (req->wakeUp == 1) {
            WSetForcePowerState(0x8001);
        } else {
            WSetForcePowerState(0);
        }
        wlWorkPlain->bss.receiveDtims = req->receiveDtims;
    } else {
        WSetForcePowerState(0x8000);
        WSetPowerState(2);
    }
    return 0;
}

int MLME_ScanReqCmd(WlMlmeScanReq *req, WlMlmeScanCfm *cfm) {
    WlWork *w = wlWorkPlain;
    WlMlme *mlme = &w->mlme;
    u32 mode;
    u32 i;
    mlme->work.scan.cfmSpace = cfm->header.length - 3;
    cfm->header.length = 3;
    mode = w->param.mode;
    if (mode != 1 && mode != 3 && mode != 2) {
        return 11;
    }
    if (wlWorkPlain->bss.state < 32) {
        return 1;
    }
    if (req->ssidLength > 32) {
        return 5;
    }
    if (req->scanType > 1) {
        return 5;
    }
    if (!WL_ReadByte(req->channelList)) {
        return 5;
    }
    if (req->maxChannelTime > 1000) {
        return 5;
    }
    if (req->maxChannelTime < 10) {
        return 5;
    }
    if (req->knownBssidCount > 16) {
        return 5;
    }
    for (i = 0; i < 16; i++) {
        u32 channel = WL_ReadByte(req->channelList + i);
        if (!channel) {
            break;
        }
        if (!CheckEnableChannel(channel)) {
            return 5;
        }
    }
    WSetBssid(req->bssid);
    WSetSsid(req->ssidLength, req->ssid);
    mlme->req.scan = req;
    mlme->cfm.scan = cfm;
    mlme->state = 16;
    AddTask(2, 0);
    return 128;
}

int MLME_JoinReqCmd(WlMlmeJoinReq *req, WlMlmeJoinCfm *cfm) {
    WlWork *w = wlWorkPlain;
    WlMlme *mlme = &w->mlme;
    WlBss *bss = &w->bss;
    u32 mode;
    u32 basic, supported;
    cfm->header.length = 5;
    mode = w->param.mode;
    if (mode != 3 && mode != 2) {
        return 11;
    }
    if (bss->state < 32) {
        return 1;
    }
    WSetStaState(32);
    if (req->bssid[0] & 1) {
        return 5;
    }
    if (!req->ssidLength) {
        return 5;
    }
    if (req->ssidLength > 32) {
        return 5;
    }
    if (req->beaconPeriod < 10) {
        return 5;
    }
    if (req->beaconPeriod > 1000) {
        return 5;
    }
    if (req->dtimPeriod > 255) {
        return 5;
    }
    if (req->channel & 0xFFF0) {
        return 5;
    }
    if (!CheckEnableChannel(req->channel)) {
        return 5;
    }
    basic = req->rateSet.basic;
    if (basic & 0xFFFFF000) {
        return 5;
    }
    supported = req->rateSet.supported;
    if (supported & 0xFFFFF000) {
        return 5;
    }
    if (!basic) {
        return 5;
    }
    if (!(supported | basic)) {
        return 5;
    }
    if (req->timeOut > 2000) {
        return 5;
    }
    if (FLASH_VerifyCheckSum(NULL)) {
        return 14;
    }
    if (req->capaInfo & 32) {
        WSetPreambleType(1);
    } else {
        WSetPreambleType(0);
    }
    WSetBssid(req->bssid);
    WSetSsid(req->ssidLength, req->ssid);
    WSetBeaconPeriod(req->beaconPeriod);
    WSetChannel(req->channel, 0);
    WSetRateSet((const u16 *)&req->rateSet);
    mlme->req.join = req;
    mlme->cfm.join = cfm;
    mlme->state = 32;
    AddTask(2, 1);
    return 128;
}

int MLME_AuthReqCmd(WlMlmePeerReq *req, WlMlmeAuthCfm *cfm) {
    WlWork *w = wlWorkPlain;
    WlMlme *mlme = &w->mlme;
    WlBss *bss = &w->bss;
    u32 mode;
    cfm->header.length = 6;
    mode = w->param.mode;
    if (mode != 3 && mode != 2) {
        return 11;
    }
    if (bss->state < 32) {
        return 1;
    }
    if (req->peerMacAdrs[0] & 1) {
        return 5;
    }
    if (req->param > 1) {
        return 5;
    }
    if (req->timeOut > 2000) {
        return 5;
    }
    if (req->timeOut < 10) {
        return 5;
    }
    WSetStaState(32);
    mlme->req.peer = req;
    mlme->cfm.auth = cfm;
    mlme->state = 48;
    cfm->algorithm = mlme->req.peer->param;
    WSetMacAdrs1(cfm->peerMacAdrs, mlme->req.peer->peerMacAdrs);
    MLME_AuthTask();
    return 128;
}

int MLME_DeAuthReqCmd(WlMlmePeerReq *req, WlMlmeDeAuthCfm *cfm) {
    WlWork *w = wlWorkPlain;
    WlMlme *mlme = &w->mlme;
    u32 mode;
    WlWork *current;
    WlTxHeapFrame *frame;
    cfm->header.length = 4;
    current = wlWorkPlain;
    mode = current->param.mode;
    if (mode != 3 && mode != 2 && mode != 1) {
        return 11;
    }
    if (current->bss.state < 48) {
        return 1;
    }
    if ((u16)(mode + 0xFFFE) <= 1 && (req->peerMacAdrs[0] & 1)) {
        return 5;
    }
    WSetMacAdrs1(cfm->peerMacAdrs, req->peerMacAdrs);
    frame = MakeDeAuthFrame(cfm->peerMacAdrs, req->param, 0);
    if (!frame) {
        return 8;
    }
    mlme->req.peer = req;
    mlme->cfm.deauth = cfm;
    mlme->work.frame = (u8 *)frame;
    mlme->state = 65;
    if (req->peerMacAdrs[0] & 1) {
        frame->time = wlWorkPlain->bss.intervalCount;
        CAM_AddBcFrame(&wlWorkPlain->heap.bufMan[0], (WlHeapBuf *)((u8 *)frame - 16));
        w = wlWorkPlain;
        if (!(w->cam.powerMgtMap & ~w->cam.notAssocMap)) {
            TxqPri(2);
        }
    } else {
        DeleteTxFrameByAdrs(req->peerMacAdrs);
        TxManCtrlFrame(frame);
    }
    return 128;
}

int MLME_AssReqCmd(WlMlmePeerReq *req, WlCmdCfm *cfm) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlMlme *mlme = &w->mlme;
    u32 mode;
    cfm->length = 3;
    mode = w->param.mode;
    if (mode != 3 && mode != 2) {
        return 11;
    }
    if (bss->state < 48) {
        return 1;
    }
    if (req->peerMacAdrs[0] & 1) {
        return 5;
    }
    if (!req->param) {
        return 5;
    }
    if (req->param > 255) {
        return 5;
    }
    if (req->timeOut > 2000) {
        return 5;
    }
    if (req->timeOut < 10) {
        return 5;
    }
    WSetStaState(48);
    WClearAids();
    bss->listenInterval = req->param;
    bss->listenCount = req->param;
    mlme->req.peer = req;
    mlme->cfm.header = cfm;
    mlme->state = 80;
    MLME_AssTask();
    return 128;
}

int MLME_ReAssReqCmd(WlMlmePeerReq *req, WlCmdCfm *cfm) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlMlme *mlme = &w->mlme;
    u32 mode;
    cfm->length = 3;
    mode = w->param.mode;
    if (mode != 3 && mode != 2) {
        return 11;
    }
    if (bss->state < 48) {
        return 1;
    }
    if (req->peerMacAdrs[0] & 1) {
        return 5;
    }
    if (req->param < 1) {
        return 5;
    }
    if (req->param > 255) {
        return 5;
    }
    if (req->timeOut > 2000) {
        return 5;
    }
    if (req->timeOut < 10) {
        return 5;
    }
    bss->listenInterval = req->param;
    bss->listenCount = req->param;
    mlme->req.peer = req;
    mlme->cfm.header = cfm;
    mlme->state = 96;
    MLME_ReAssTask();
    return 128;
}

int MLME_DisAssReqCmd(WlMlmePeerReq *req, WlCmdCfm *cfm) {
    WlWork *w = wlWorkPlain;
    WlMlme *mlme = &w->mlme;
    u32 mode;
    WlTxHeapFrame *frame;
    cfm->length = 1;
    mode = wlWorkPlain->param.mode;
    if (!mode) {
        return 11;
    }
    if (mode != 1 && (req->peerMacAdrs[0] & 1)) {
        return 5;
    }
    if (wlWorkPlain->bss.state != 64) {
        return 1;
    }
    frame = MakeDisAssFrame(req->peerMacAdrs, req->param);
    if (!frame) {
        return 8;
    }
    mlme->req.peer = req;
    mlme->cfm.header = cfm;
    mlme->work.frame = (u8 *)frame;
    mlme->state = 113;
    if (req->peerMacAdrs[0] & 1) {
        frame->time = wlWorkPlain->bss.intervalCount;
        CAM_AddBcFrame(&wlWorkPlain->heap.bufMan[0], (WlHeapBuf *)((u8 *)frame - 16));
        w = wlWorkPlain;
        if (!(w->cam.powerMgtMap & ~w->cam.notAssocMap)) {
            TxqPri(2);
        }
    } else {
        DeleteTxFrameByAdrs(req->peerMacAdrs);
        TxManCtrlFrame(frame);
    }
    return 128;
}

int MLME_StartReqCmd(WlMlmeStartReq *req, WlCmdCfm *cfm) {
    WlMlmeStartReq *r = req;
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlParam *param = &w->param;
    cfm->length = 1;
    if (param->mode != 1 && param->mode != 0) {
        return 11;
    }
    if (bss->state != 32) {
        return 1;
    }
    if (r->ssidLength > 32) {
        return 5;
    }
    if (!r->ssidLength) {
        return 5;
    }
    if (r->beaconPeriod < 10) {
        return 5;
    }
    if (r->beaconPeriod > 1000) {
        return 5;
    }
    if (!r->dtimPeriod) {
        return 5;
    }
    if (r->dtimPeriod > 255) {
        return 5;
    }
    if (r->channel & 0xFFF0) {
        return 5;
    }
    if (!CheckEnableChannel(r->channel)) {
        return 5;
    }
    if (!r->rateSet.basic) {
        return 5;
    }
    if (r->rateSet.basic & 0xFFFFF000) {
        return 5;
    }
    if (!r->rateSet.supported) {
        return 5;
    }
    if (r->rateSet.supported & 0xFFFFF000) {
        return 5;
    }
    if (r->gameInfoLength > 128) {
        return 5;
    }
    if (FLASH_VerifyCheckSum(NULL)) {
        return 14;
    }
    if (!param->mode) {
        WSetBssid((const u16 *)BC_ADRS);
    } else {
        WSetBssid(param->macAddress);
    }
    WSetSsid(r->ssidLength, r->ssid);
    WSetBeaconPeriod(r->beaconPeriod);
    WSetDTIMPeriod(r->dtimPeriod);
    WSetChannel(r->channel, 0);
    WSetRateSet((const u16 *)&r->rateSet);
    WInitGameInfo(r->gameInfoLength, req->gameInfo);
    bss->gameInfoUpdated = 0;
    WStart();
    return 0;
}

int MLME_MeasChanReqCmd(WlMlmeMeasChanReq *req, WlMlmeMeasChanCfm *cfm) {
    WlWork *w = wlWorkPlain;
    WlMlme *mlme = &w->mlme;
    u32 i;
    cfm->header.length = 18;
    if (wlWorkPlain->bss.state != 32) {
        return 1;
    }
    if (req->ccaMode > 3) {
        return 5;
    }
    if (req->edThreshold > 63) {
        return 5;
    }
    if (!req->measureTime) {
        return 5;
    }
    if (req->measureTime > 1000) {
        return 5;
    }
    for (i = 0; i < 16; i++) {
        u32 channel = WL_ReadByte(req->channelList + i);
        if (!channel) {
            break;
        }
        if (!CheckEnableChannel(channel)) {
            return 5;
        }
    }
    if (!i) {
        return 5;
    }
    mlme->req.meas = req;
    mlme->cfm.meas = cfm;
    mlme->state = 128;
    cfm->header.result = 128;
    MLME_MeasChannelTask();
    return 128;
}

#pragma dont_inline reset
