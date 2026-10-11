#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Rate bit (WlRateSet) -> supported rates element value (500 kbps units).
extern u16 RateBit2Element[16];

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Authentication frame; `length` > 0 adds a challenge text element of that size. With `test` set it fails
// (NULL) while too many management frames are queued.
WlTxHeapFrame *MakeAuthFrame(const u16 *macAdrs, u16 length, u32 test) {
    WlMaDataReq *p;
    if (test && !IsEnableManagement()) {
        return 0;
    }
    p = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], length + 61);
    if (!p) {
        SetFatalErr(2);
        return (WlTxHeapFrame *)p;
    }
    p->header.command = 65535;
    InitManHeader(&p->frame, macAdrs);
    if (length) {
        WL_WriteByte((u8 *)&p->frame.frame.unk_28 + 2, (u8)16);
        WL_WriteByte((u8 *)&p->frame.frame.unk_28 + 3, (u8)length);
        length += 2;
    }
    p->frame.bodyLength = length + 6;
    p->frame.frame.length = p->frame.bodyLength + 28;
    p->frame.frame.frameCtrl = 0xB0;
    return &p->frame;
}

// Deauthentication frame with `reason`; with `test` set it fails (NULL) while too many management frames are queued.
WlTxHeapFrame *MakeDeAuthFrame(const u16 *macAdrs, u16 reason, u32 test) {
    WlMaDataReq *p;
    if (test && !IsEnableManagement()) {
        return 0;
    }
    p = AllocateHeapBuf(&wlWorkPlain->heap.bufMan[0], 54);
    if (!p) {
        SetFatalErr(2);
        return (WlTxHeapFrame *)p;
    }
    p->header.command = 65535;
    InitManHeader(&p->frame, macAdrs);
    p->frame.frame.unk_24 = reason;
    p->frame.bodyLength = 2;
    p->frame.frame.length = p->frame.bodyLength + 28;
    p->frame.frame.frameCtrl = 0xC0;
    return &p->frame;
}

// Writes the PS-Poll frame for `aid` into its MAC slot (txq[2]).
void MakePsPollFrame(u16 aid) {
    WlTxFrame *p = (WlTxFrame *)wlWorkPlain->txCtrl.txq[2].frame;
    p->status = 0;
    p->unk_02 = 0;
    p->retry = 0;
    p->length = 20;
    p->frameCtrl = 0xA4;
    p->duration = aid | 0xC000;
    {
        WlWork *w = wlWorkPlain;
        WSetMacAdrs2(p->addr1, w->bss.bssid, w->param.macAddress);
    }
}

// Clears a management frame's header and addresses it from this station (BSSID: the current one).
void InitManHeader(WlTxHeapFrame *frame, const u16 *macAdrs) {
    WlTxHeapFrame *p = frame;
    WlWork *w;
    __VENEER_MIi_CpuClear16(0, p, 44);
    p->frame.rate = WCalcManRate();
    w = wlWorkPlain;
    WSetMacAdrs3(p->frame.addr1, macAdrs, w->param.macAddress, w->bss.bssid);
}

// A management frame of type `control` for `macAdrs` is already queued.
int IsExistManFrame(const u16 *macAdrs, u16 control) {
    WlMaDataReq *p = (WlMaDataReq *)wlWorkPlain->heap.bufMan[2].first;
    while (p != (WlMaDataReq *)0xFFFFFFFF) {
        if (p->frame.frame.frameCtrl == control && MatchMacAdrs(p->frame.frame.addr1, macAdrs)) {
            return 1;
        }
        p = (WlMaDataReq *)GetHeapBufNextAdrs((WlHeapBuf *)p);
    }
    return 0;
}

// SSID element of the current SSID; returns its size.
u32 SetSSIDElement(u8 *dest) {
    u8 *s = (u8 *)&wlWorkPlain->bss;
    u32 count = 0, length = ((WlBss *)s)->ssidLength, i;
    WL_WriteByte(dest, (u8)count);
    WL_WriteByte(dest + 1, (u8)length);
    count += 2;
    s += 0x20; // WlBss.ssid
    for (i = 0; i < length; i++) {
        WL_WriteByte(dest + count, WL_ReadByte(s + i));
        count++;
    }
    return count;
}

// Supported rates element of WlBss.rateSet (basic rates flagged with 0x80); returns its size.
u32 SetSupRateSet(u8 *dest) {
    WlBss *s = &wlWorkPlain->bss;
    u32 count = 0, i;
    WL_WriteByte(dest, 1);
    count += 2;
    for (i = 0; i < 16; i++) {
        if (s->rateSet.supported & (1u << i)) {
            if (s->rateSet.basic & (1u << i)) {
                WL_WriteByte(dest + count, (u8)(RateBit2Element[i] | 0x80));
            } else {
                WL_WriteByte(dest + count, (u8)RateBit2Element[i]);
            }
            count++;
        }
    }
    WL_WriteByte(dest + 1, (u8)(count - 2));
    return count;
}

#pragma dont_inline reset
