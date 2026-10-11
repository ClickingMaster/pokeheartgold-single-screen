#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// MLME tasks (0: scan, 1: join, 2: auth, 3: ass, 4: reass, 5: measure channel) and their SetupTimeOut
// handlers. Each task steps through WlMlme.state and sends the confirm with IssueMlmeConfirm at the end.

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Scan: per channel of the list, (active scan) sends a probe request and listens for maxChannelTime.
void MLME_ScanTask(void) {
    WlMlme *p;
    WlBss *bss;
    int schedule = 0;
    WlWork *w = wlWorkPlain;
    p = &w->mlme;
    bss = &w->bss;
    switch (p->state) {
    case 16:
        WSetStaState(32);
        bss->mode = 2;
        p->cfm.scan->bssCount = 0;
        p->cfm.scan->knownBssidFound = 0;
        p->work.scan.channelIndex = 0;
        p->work.scan.unk_08 = 0;
        if (p->req.scan->scanType == 0) {
            p->work.scan.step = ((int)p->req.scan->maxChannelTime + 3) / 4;
            if (p->work.scan.step < 10) {
                p->work.scan.step = 10;
            }
        } else {
            p->work.scan.step = p->req.scan->maxChannelTime;
        }
        p->cfm.header->result = 0;
    case 17: {
        u16 channel = WL_ReadByte(p->req.scan->channelList + p->work.scan.channelIndex);
        if (!channel) {
            p->state = 21;
            schedule = 1;
            break;
        }
        p->work.scan.channelIndex++;
        p->work.scan.elapsed = 0;
        if (FLASH_VerifyCheckSum(NULL)) {
            p->cfm.header->result = 14;
            p->state = 21;
            schedule = 1;
            break;
        }
        if (p->state == 16) {
            WSetChannel(channel, 0);
            WStart();
        } else {
            WSetChannel(channel, 0);
        }
        p->state = 18;
    }
    case 18:
    case 19: {
        WlTxHeapFrame *frame;
        p->state = 19;
        if (!p->req.scan->scanType) {
            frame = MakeProbeReqFrame(p->req.scan->bssid);
            if (!frame) {
                p->cfm.header->result = 8;
                p->state = 21;
                schedule = 1;
                break;
            }
            TxManCtrlFrame(frame);
        }
        SetupTimeOut(p->work.scan.step, (OSAlarmHandler)MLME_ScanTimeOut);
        break;
    }
    case 20:
        break;
    case 21:
        p->state = 0;
        WStop();
        bss->mode = wlWorkPlain->param.mode;
        IssueMlmeConfirm();
        break;
    }
    if (schedule) {
        AddTask(2, 0);
    }
}

void MLME_ScanTimeOut(void) {
    WlMlme *p = &wlWorkPlain->mlme;
    p->work.scan.elapsed += p->work.scan.step;
    if (p->work.scan.elapsed >= p->req.scan->maxChannelTime) {
        if (p->work.scan.channelIndex < 16) {
            p->state = 17;
        } else {
            p->state = 21;
        }
    }
    AddTask(2, 0);
}

// Join: starts the MAC on the BSS and waits for its beacon (the RX side ends the join).
void MLME_JoinTask(void) {
    WlMlme *p = &wlWorkPlain->mlme;
    switch (p->state) {
    case 32:
        WStart();
        p->work.join.result = 0;
        p->work.join.reason = 0;
        p->state = 33;
        SetupTimeOut(p->req.join->timeOut, (OSAlarmHandler)MLME_JoinTimeOut);
        break;
    case 37:
        p->cfm.join->header.result = p->work.join.result;
        p->cfm.join->reason = p->work.join.reason;
        if (p->work.join.result) {
            WStop();
        }
        p->state = 0;
        IssueMlmeConfirm();
        break;
    }
}

void MLME_JoinTimeOut(void) {
    WlMlme *p = &wlWorkPlain->mlme;
    p->work.join.result = 7;
    p->state = 37;
    AddTask(2, 1);
}

// Auth: sends the authentication frame (sequence 1) and waits for the answer.
void MLME_AuthTask(void) {
    WlMlme *p = &wlWorkPlain->mlme;
    switch (p->state) {
    case 48: {
        WlTxHeapFrame *frame = MakeAuthFrame(p->req.peer->peerMacAdrs, 0, 0);
        if (!frame) {
            p->cfm.header->result = 8;
            p->state = 53;
            AddTask(2, 2);
        } else {
            // body: algorithm, transaction sequence number, status code
            u16 *body = &frame->frame.unk_24;
            body[0] = p->req.peer->param;
            body[1] = 1;
            body[2] = 0;
            p->state = 49;
            TxManCtrlFrame(frame);
            SetupTimeOut(p->req.peer->timeOut, (OSAlarmHandler)MLME_AuthTimeOut);
        }
        break;
    }
    case 53:
        ResetTxqPri(1);
        ClearQueuedPri(1);
        MessageDeleteTx(1, 0);
        p->state = 0;
        IssueMlmeConfirm();
        break;
    }
}

void MLME_AuthTimeOut(void) {
    WlMlme *p = &wlWorkPlain->mlme;
    p->cfm.header->result = 7;
    p->state = 53;
    AddTask(2, 2);
}

// Ass: sends the association request and waits for the response.
void MLME_AssTask(void) {
    WlMlme *p = &wlWorkPlain->mlme;
    switch (p->state) {
    case 80: {
        WlTxHeapFrame *frame = MakeAssReqFrame(p->req.peer->peerMacAdrs);
        if (!frame) {
            p->cfm.header->result = 8;
            p->state = 83;
            AddTask(2, 3);
        } else {
            p->state = 81;
            TxManCtrlFrame(frame);
            SetupTimeOut(p->req.peer->timeOut, (OSAlarmHandler)MLME_AssTimeOut);
        }
        break;
    }
    case 83:
        ResetTxqPri(1);
        ClearQueuedPri(1);
        MessageDeleteTx(1, 0);
        p->state = 0;
        IssueMlmeConfirm();
        break;
    }
}

void MLME_AssTimeOut(void) {
    WlMlme *p = &wlWorkPlain->mlme;
    p->cfm.header->result = 7;
    p->state = 83;
    AddTask(2, 3);
}

// ReAss: sends the reassociation request and waits for the response.
void MLME_ReAssTask(void) {
    WlMlme *p = &wlWorkPlain->mlme;
    switch (p->state) {
    case 96: {
        WlTxHeapFrame *frame = MakeReAssReqFrame(p->req.peer->peerMacAdrs);
        if (!frame) {
            p->cfm.header->result = 8;
            p->state = 99;
            AddTask(2, 4);
        } else {
            p->state = 97;
            TxManCtrlFrame(frame);
            SetupTimeOut(p->req.peer->timeOut, (OSAlarmHandler)MLME_ReAssTimeOut);
        }
        break;
    }
    case 99:
        ClearQueuedPri(1);
        MessageDeleteTx(1, 0);
        p->state = 0;
        IssueMlmeConfirm();
        break;
    }
}

void MLME_ReAssTimeOut(void) {
    WlMlme *p = &wlWorkPlain->mlme;
    p->cfm.header->result = 7;
    p->state = 99;
    AddTask(2, 4);
}

// Measure channel: per channel of the list, samples the carrier sense bit (register 0x19C bit 0) for
// measureTime and reports the busy percentage.
void MLME_MeasChannelTask(void) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlMlme *p = &w->mlme;
    switch (p->state) {
    case 0x80:
        p->work.meas.index = 0;
        p->work.meas.bbp13 = BBP_Read(0x13);
        p->work.meas.bbp35 = BBP_Read(0x35);
        WSetCCA_ED(p->req.meas->ccaMode, p->req.meas->edThreshold);
        bss->mode = 4;
        p->work.meas.result = 0;
    case 0x81: {
        u32 channel;
        p->work.meas.samples = 0;
        p->work.meas.total = 0;
        channel = WL_ReadByte(p->req.meas->channelList + p->work.meas.index);
        if (!channel || p->work.meas.index >= 16) {
            p->state = 0x84;
            break;
        }
        if (FLASH_VerifyCheckSum(NULL)) {
            p->work.meas.result = 14;
            p->state = 0x84;
            break;
        }
        if (p->state == 0x80) {
            WSetChannel(channel, 0);
            WStart();
            p->work.meas.power = WL_REG(0x040);
            WSetForcePowerState(0x8000);
        } else {
            WSetChannel(channel, 0);
        }
        p->state = 0x82;
        SetupTimeOut(p->req.meas->measureTime, (OSAlarmHandler)MLME_MeasChanTimeOut);
    }
    case 0x82:
        p->work.meas.samples++;
        if (WL_REG(0x19C) & 1) {
            p->work.meas.total += 100;
        }
        break;
    case 0x83: {
        u32 channel = WL_ReadByte(p->req.meas->channelList + p->work.meas.index);
        u32 level = 0;
        if (p->work.meas.samples && p->work.meas.total) {
            level = __VENEER__u32_div_f(p->work.meas.total, p->work.meas.samples) + 1;
            if (level > 100) {
                level = 100;
            }
        }
        p->cfm.meas->result[p->work.meas.index] = channel | (level << 8);
        p->work.meas.index++;
        p->state = 0x81;
        break;
    }
    case 0x84: {
        u32 i;
        WStop();
        bss->mode = wlWorkPlain->param.mode;
        BBP_Write(0x13, p->work.meas.bbp13);
        BBP_Write(0x35, p->work.meas.bbp35);
        WSetForcePowerState(p->work.meas.power);
        p->cfm.header->result = p->work.meas.result;
        p->state = 0;
        i = p->work.meas.index;
        while (i < 16) {
            p->cfm.meas->result[i] = 0;
            i++;
        }
        IssueMlmeConfirm();
        break;
    }
    }
    if (p->state) {
        AddTask(2, 5);
    }
}

void MLME_MeasChanTimeOut(void) {
    wlWorkPlain->mlme.state = 0x83;
    AddTask(2, 5);
}

#pragma dont_inline reset
