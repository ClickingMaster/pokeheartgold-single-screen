#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// MLME confirms and indications to the WMSP. The Issue*Indication functions return 0 (and set fatal
// error 1) when the driver heap is out of memory.

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Task 12 (WlIntrPreTbtt: too many beacons lost).
void MLME_BeaconLostTask(void) {
    MLME_IssueBeaconLostIndication(wlWorkPlain->bss.unk_82);
}

// Sends the confirm of the MLME request in progress and lets RequestCmdTask take the next request.
void IssueMlmeConfirm(void) {
    WlWork *w = wlWorkPlain;
    WlHeap *heap = &w->heap;
    {
        // (the cast keeps the compiler from folding the member offset, as in the original)
        WlCmdIf *cmdIf = (WlCmdIf *)&w->cmdIf;
        cmdIf->busy &= ~1;
    }
    SendMessageToWmDirect(&heap->bufMan[10], w->cmdIf.request);
    if (heap->bufMan[10].count) {
        AddTask(2, 11);
    }
}

int MLME_IssueAuthIndication(const u16 *macAdrs, int algorithm) {
    WlMlmePeerInd *ind = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 24);
    if (!ind) {
        SetFatalErr(1);
        return 0;
    }
    ind->header.command = 132;
    ind->header.length = 4;
    WSetMacAdrs1(ind->peerMacAdrs, macAdrs);
    ind->param = algorithm;
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], ind);
    return 1;
}

int MLME_IssueDeAuthIndication(const u16 *macAdrs, int reason) {
    WlMlmePeerInd *ind = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 24);
    if (!ind) {
        SetFatalErr(1);
        return 0;
    }
    ind->header.command = 133;
    ind->header.length = 4;
    WSetMacAdrs1(ind->peerMacAdrs, macAdrs);
    ind->param = reason;
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], ind);
    return 1;
}

// `ssidElement`: the SSID element of the request (element id, length, SSID).
int MLME_IssueAssIndication(const u16 *macAdrs, u32 aid, const u8 *ssidElement) {
    WlMlmeAssInd *ind = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 58);
    u32 i;
    if (!ind) {
        SetFatalErr(1);
        return 0;
    }
    ind->header.command = 134;
    ind->header.length = 21;
    WSetMacAdrs1(ind->peerMacAdrs, macAdrs);
    ind->aid = aid & 4095;
    ind->ssidLength = WL_ReadByte(ssidElement + 1);
    ssidElement += 2;
    i = 0;
    while (i < ind->ssidLength) {
        if (i >= 32) {
            break;
        }
        WL_WriteByte(ind->ssid + i, WL_ReadByte(ssidElement + i));
        i++;
    }
    while (i < 32) {
        WL_WriteByte(ind->ssid + i, 0);
        i++;
    }
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], ind);
    return 1;
}

int MLME_IssueReAssIndication(const u16 *macAdrs, u32 aid, const u8 *ssidElement) {
    WlMlmeAssInd *ind = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 58);
    u32 i;
    if (!ind) {
        SetFatalErr(1);
        return 0;
    }
    ind->header.command = 135;
    ind->header.length = 21;
    WSetMacAdrs1(ind->peerMacAdrs, macAdrs);
    ind->aid = aid;
    ind->ssidLength = WL_ReadByte(ssidElement + 1);
    ssidElement += 2;
    i = 0;
    while (i < ind->ssidLength) {
        if (i >= 32) {
            break;
        }
        WL_WriteByte(ind->ssid + i, WL_ReadByte(ssidElement + i));
        i++;
    }
    while (i < 32) {
        WL_WriteByte(ind->ssid + i, 0);
        i++;
    }
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], ind);
    return 1;
}

int MLME_IssueDisAssIndication(const u16 *macAdrs, int reason) {
    WlMlmePeerInd *ind = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 24);
    if (!ind) {
        SetFatalErr(1);
        return 0;
    }
    ind->header.command = 136;
    ind->header.length = 4;
    WSetMacAdrs1(ind->peerMacAdrs, macAdrs);
    ind->param = reason;
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], ind);
    return 1;
}

int MLME_IssueBeaconLostIndication(const u16 *macAdrs) {
    WlMlmeBeaconLostInd *ind = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 22);
    if (!ind) {
        SetFatalErr(1);
        return 0;
    }
    ind->header.command = 139;
    ind->header.length = 3;
    WSetMacAdrs1(ind->macAdrs, macAdrs);
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], ind);
    return 1;
}

int MLME_IssueBeaconSendIndication(void) {
    WlIndHeader *ind = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 16);
    if (!ind) {
        SetFatalErr(1);
        return 0;
    }
    ind->command = 140;
    ind->length = 0;
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], ind);
    return 1;
}

int MLME_IssueBeaconRecvIndication(WlRxFrame *frame) {
    WlWork *w = wlWorkPlain;
    WlBss *bss = &w->bss;
    WlMlmeBeaconRecvInd *ind = AllocateHeapBuf(&w->heap.bufMan[0], w->bss.gameInfoLength + 62);
    if (!ind) {
        SetFatalErr(1);
        return 0;
    }
    ind->header.command = 0x8D;
    ind->header.length = ((int)bss->gameInfoLength + 1) / 2 + 22;
    WL_WriteByte(&ind->unk_1f, frame->unk_12 & 255);
    WL_WriteByte(&ind->unk_1e, frame->unk_0e & 255);
    WSetMacAdrs1(ind->srcMacAdrs, frame->addr2);
    ind->gameInfoLength = bss->gameInfoLength;
    if (ind->gameInfoLength) {
        if (bss->unk_a2 & 1) {
            u32 i;
            u8 *src, *dst;
            dst = ind->gameInfo;
            src = bss->gameInfo + 1;
            i = 0;
            while (i < ind->gameInfoLength) {
                WL_WriteByte(dst, WL_ReadByte(src));
                src++;
                dst++;
                i++;
            }
        } else {
            __VENEER_MIi_CpuCopy16(bss->gameInfo, ind->gameInfo, ind->gameInfoLength + 1);
        }
    }
    SendMessageToWmDirect(&wlWorkPlain->heap.bufMan[0], ind);
    return 1;
}

void InitializeMLME(void) {
    MIi_CpuClear16(0, (u16 *)&wlWorkPlain->mlme, 32);
}

#pragma dont_inline reset
