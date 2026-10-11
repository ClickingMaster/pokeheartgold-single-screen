#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

#define WL_IRQ_MASK 0x01000000 // interrupt bit 24: wireless

// Some accesses to the station table state were volatile in the original code.
#define VOLATILE_U16(lvalue) (*(vu16 *)&(lvalue))

u32 CAM_Search(const u16 *addr) {
    WlWork *w;
    WlStation *sta;
    u32 index, seen;
    if (addr[0] & 1) {
        return 0;
    }
    w = wlWorkPlain;
    if (w->cam.numSta > 1) {
        sta = w->param.camTable + 1;
        seen = 0;
        for (index = 1; index < w->param.maxSta; index++, sta++) {
            if (sta->state) {
                if (MatchMacAdrs(sta->macAdrs, addr)) {
                    return index;
                }
                w = wlWorkPlain;
                seen++;
                if (seen >= w->cam.numSta) {
                    break;
                }
            }
        }
    }
    return 255;
}

// Returns the entry of addr, else a free entry (or the oldest idle one) initialised for it; 255: table full.
u32 CAM_SearchAdd(const u16 *addr) {
    const u16 *address;
    WlWork **root;
    WlParam *param;
    WlStation *sta;
    u32 index, seen, freeIndex;
    WlWork *w;
    address = addr;
    root = (WlWork **)WL_WORK_ADDR;
    w = *root;
    param = &w->param;
    if (address[0] & 1) {
        return 0;
    }
    index = 1;
    if (w->cam.numSta > 1) {
        sta = param->camTable + 1;
        freeIndex = 0;
        seen = 0;
        for (; index < param->maxSta; index++, sta++) {
            if (sta->state) {
                if (MatchMacAdrs(sta->macAdrs, address)) {
                    return index;
                }
                w = *root;
                seen++;
                if (seen >= w->cam.numSta) {
                    break;
                }
            } else if (freeIndex == 0) {
                freeIndex = index;
            }
        }
        if (freeIndex) {
            index = freeIndex;
        }
    }
    if (index >= w->param.maxSta) {
        sta = param->camTable;
        u32 best, count, i;
        count = param->maxSta;
        best = 0x10000;
        i = 1;
        index = 0;
        for (; i < count; i++) {
            WlStation *entry = sta + i;
            if (entry->state < 0x30 && entry->frameCount == 0 && best > entry->timer) {
                best = entry->timer;
                index = i;
            }
        }
        if (index == 0) {
            return 255;
        }
    }
    CAM_InitElement(index, (const u8 *)address);
    return index;
}

void CAM_AddBcFrame(WlHeapBufMan *from, WlHeapBuf *frame) {
    WlHeapBufMan *queue = &wlWorkPlain->heap.bufMan[3];
    u32 mask = __VENEER_OS_DisableIrqMask(WL_IRQ_MASK);
    if (queue->count == 0) {
        CAM_SetTIMElementBitmap(0);
    }
    MoveHeapBuf(from, queue, frame);
    __VENEER_OS_EnableIrqMask(mask);
}

// A queued frame: the station entry it goes to.
typedef struct WlCamFrame {
    u16 unk_00;
    u16 index;
} WlCamFrame;

void CAM_IncFrameCount(void *frame) {
    u32 irq, index;
    WlStation *sta;
    WlCamFrame *camFrame;
    camFrame = frame;
    index = camFrame->index;
    sta = &wlWorkPlain->param.camTable[index];
    irq = __VENEER_OS_DisableIrqMask(WL_IRQ_MASK);
    if (wlWorkPlain->bss.mode == 1 && sta->frameCount == 0) {
        CAM_SetTIMElementBitmap(index);
    }
    sta->frameCount++;
    __VENEER_OS_EnableIrqMask(irq);
    if (!(wlWorkPlain->cam.unk_08 & (1U << index))) {
        sta->timer = sta->lifeTime;
    }
}

void CAM_DecFrameCount(void *frame) {
    u32 mask, index;
    WlStation *sta;
    WlCamFrame *camFrame;
    camFrame = frame;
    index = camFrame->index;
    sta = wlWorkPlain->param.camTable + index;
    mask = __VENEER_OS_DisableIrqMask(WL_IRQ_MASK);
    if (wlWorkPlain->bss.mode == 1 && sta->frameCount == 1) {
        CAM_ClrTIMElementBitmap(index);
    }
    sta->frameCount--;
    __VENEER_OS_EnableIrqMask(mask);
}

#pragma opt_common_subs off

void CAM_SetStaState(u16 aid, u16 state) {
    u32 e;
    e = __VENEER_OS_DisableIrqMask(WL_IRQ_MASK);
    if (state < 0x40) {
        wlWorkPlain->cam.activeMap |= 1 << aid;
        wlWorkPlain->cam.unk_06 |= 1 << aid;
        if (wlWorkPlain->bss.mode == 1 && CAM_GetAID(aid) != 0) {
            CAM_ReleaseAID(aid);
        }
    } else {
        wlWorkPlain->cam.unk_06 &= ~(1 << aid);
        if ((wlWorkPlain->cam.powerMgtMap >> aid) & 1) {
            CAM_SetDoze(aid);
        }
    }
    wlWorkPlain->param.camTable[aid].state = state;
    __VENEER_OS_EnableIrqMask(e);
}

#pragma opt_common_subs reset

void CAM_SetRSSI(u32 index, u32 rssi) {
    WlStation *sta = wlWorkPlain->param.camTable + index;
    sta->rssi = (u16)rssi;
}

void CAM_SetPowerMgtMode(u32 index, u16 mode) {
    WlWork *w = wlWorkPlain;
    volatile WlCam *cam = &w->cam;
    u32 awake;
    cam->powerMgtMap = (cam->powerMgtMap & ~(1U << index)) | (mode << index);
    awake = cam->unk_06;
    if (cam->powerMgtMap & ~awake) {
        WL_REG(0x0AC) = 8;
    } else {
        WL_REG(0x0AE) = 8;
    }
}

void CAM_SetDoze(u32 index) {
    if (CAM_GetStaState(index) == 0x40) {
        VOLATILE_U16(wlWorkPlain->cam.activeMap) &= ~(1U << index);
    }
}

void CAM_SetAwake(u32 index) {
    VOLATILE_U16(wlWorkPlain->cam.activeMap) |= 1U << index;
}

void CAM_SetCapaInfo(u32 index, u16 capaInfo) {
    wlWorkPlain->param.camTable[index].capaInfo = capaInfo;
}

void CAM_SetSupRate(u32 index, u16 supRate) {
    wlWorkPlain->param.camTable[index].supRate = supRate;
}

void CAM_SetLastSeqCtrl(u32 index, u16 seqCtrl) {
    wlWorkPlain->param.camTable[index].lastSeqCtrl = seqCtrl;
}

void CAM_SetAuthSeed(u32 index, u16 seed) {
    wlWorkPlain->param.camTable[index].authSeed = seed;
}

void CAM_UpdateLifeTime(u32 index) {
    WlStation *sta = &wlWorkPlain->param.camTable[index];
    sta->timer = sta->lifeTime;
}

u32 CAM_AllocateAID(u32 index) {
    volatile WlCam *cam = &wlWorkPlain->cam;
    u32 aid, old = __VENEER_OS_DisableIrqMask(WL_IRQ_MASK), bit, bitmap;
    for (aid = 1, bit = 2; aid < 16; aid++, bit <<= 1) {
        bitmap = cam->aidMap;
        if (!(bitmap & bit)) {
            cam->aidMap = bitmap | bit;
            cam->aidCount++;
            if (cam->aidCount == 1) {
                WEnableTmpttPowerSave();
            }
            wlWorkPlain->param.camTable[index].aid = aid;
            __VENEER_OS_EnableIrqMask(old);
            return aid;
        }
    }
    __VENEER_OS_EnableIrqMask(old);
    return 0;
}

void CAM_ReleaseAID(u32 index) {
    WlWork *w = wlWorkPlain;
    u32 aid;
    CAM_ClrTIMElementBitmap(index);
    aid = CAM_GetAID(index);
    if (aid) {
        wlWorkPlain->param.camTable[index].aid = 0;
        VOLATILE_U16(w->cam.aidMap) &= ~(1U << aid);
        --VOLATILE_U16(w->cam.aidCount);
        if (VOLATILE_U16(w->cam.aidCount) == 0) {
            WDisableTmpttPowerSave();
        }
    }
}

u16 CAM_GetStaState(u32 index) {
    return wlWorkPlain->param.camTable[index].state;
}

u32 CAM_IsActive(u32 index) {
    return ((int)VOLATILE_U16(wlWorkPlain->cam.activeMap) >> index) & 1;
}

u32 CAM_GetPowerMgtMode(u32 index) {
    return ((int)VOLATILE_U16(wlWorkPlain->cam.powerMgtMap) >> index) & 1;
}

u16 *CAM_GetMacAdrs(u32 index) {
    return wlWorkPlain->param.camTable[index].macAdrs;
}

u16 CAM_GetAuthSeed(u32 index) {
    return wlWorkPlain->param.camTable[index].authSeed;
}

u16 CAM_GetLastSeqCtrl(u32 index) {
    return wlWorkPlain->param.camTable[index].lastSeqCtrl;
}

u16 CAM_GetTxRate(u32 index) {
    if (wlWorkPlain->param.camTable[index].supRate & 2) {
        return 20;
    }
    return 10;
}

u16 CAM_GetAID(u32 index) {
    return wlWorkPlain->param.camTable[index].aid;
}

u16 CAM_GetFrameCount(u32 index) {
    return wlWorkPlain->param.camTable[index].frameCount;
}

// Sets the station's bit in the TIM element of the beacon (entry 0: the multicast bit).
void CAM_SetTIMElementBitmap(u32 index) {
    u8 *tim;
    u32 old, aid;
    u8 value;
    if (CAM_GetStaState(index) != 0x40) {
        return;
    }
    if (VOLATILE_U16(wlWorkPlain->cam.unk_08) & (1U << index)) {
        return;
    }
    tim = (u8 *)0x0480425C + VOLATILE_U16(wlWorkPlain->bss.unk_94);
    old = __VENEER_OS_DisableIrqMask(WL_IRQ_MASK);
    if (index == 0) {
        value = WL_ReadByte(tim + 4);
        WL_WriteByte(tim + 4, (u8)(value | 1));
    } else {
        aid = CAM_GetAID(index);
        tim = tim + 5 + (aid >> 3);
        value = WL_ReadByte(tim);
        WL_WriteByte(tim, (u8)(value | (1U << (aid & 7))));
    }
    __VENEER_OS_EnableIrqMask(old);
}

void CAM_ClrTIMElementBitmap(u32 index) {
    u8 *tim;
    u32 old, aid;
    u8 value;
    if (CAM_GetStaState(index) != 0x40) {
        return;
    }
    tim = (u8 *)0x0480425C + VOLATILE_U16(wlWorkPlain->bss.unk_94);
    old = __VENEER_OS_DisableIrqMask(WL_IRQ_MASK);
    if (index == 0) {
        value = WL_ReadByte(tim + 4);
        WL_WriteByte(tim + 4, (u8)(value & 0xFE));
    } else {
        aid = CAM_GetAID(index);
        tim = tim + 5 + (aid >> 3);
        value = WL_ReadByte(tim);
        WL_WriteByte(tim, (u8)(~(1U << (aid & 7)) & value));
    }
    __VENEER_OS_EnableIrqMask(old);
}

// Counts the station timers down; a station whose timer runs out is deauthenticated and dropped.
void CAM_TimerTask(void) {
    WlWork *w;
    WlStation *sta;
    u16 numSta;
    u32 aid;
    u32 count;

    count = 0;
    w = wlWorkPlain;
    aid = 1;
    sta = w->param.camTable + 1;
    numSta = w->cam.numSta;
    for (; aid < w->param.maxSta; aid++, sta++) {
        if (sta->state != 0) {
            if (sta->timer != 0 && sta->timer != 0xFFFF) {
                sta->timer--;
                if (sta->timer == 0) {
                    if (sta->state >= 0x20) {
                        u16 prevState = CAM_GetStaState(aid);
                        CAM_SetStaState(aid, 0x20);
                        DeleteTxFrames(aid);
                        if (w->bss.mode == 1) {
                            if (prevState <= 0x20) {
                                goto clear;
                            }
                            {
                                u16 *frame;
                                wlWorkPlain->cam.unk_08 |= 1 << aid;
                                CAM_SetPowerMgtMode((u16)aid, 0);
                                wlWorkPlain->cam.activeMap |= 1 << aid;
                                frame = MakeDeAuthFrame(sta->macAdrs, 1, 0);
                                if (frame) {
                                    *frame = 1;
                                    TxManCtrlFrame(frame);
                                    count++;
                                    continue;
                                }
                                MLME_IssueDeAuthIndication(sta->macAdrs, 1);
                            }
                        } else if (aid == w->bss.unk_88) {
                            u16 *frame = MakeDeAuthFrame(sta->macAdrs, 1, 0);
                            if (frame) {
                                *frame = 1;
                                TxManCtrlFrame(frame);
                                count++;
                                continue;
                            }
                            WSetStaState(0x20);
                            WClearAids();
                            MLME_IssueDeAuthIndication(sta->macAdrs, 1);
                        }
                    }
clear:
                    sta->state = 0;
                    w->cam.numSta--;
                }
            }
            count++;
        }
        if (count >= numSta) {
            break;
        }
    }
}

void CAM_Delete(u32 index) {
    DeleteTxFrames(index);
    wlWorkPlain->param.camTable[index].state = 0;
    --VOLATILE_U16(wlWorkPlain->cam.numSta);
}

void InitializeCAM(void) {
    WlStation *sta = wlWorkPlain->param.camTable;
    u32 count = VOLATILE_U16(wlWorkPlain->param.maxSta), j;
    __VENEER_MIi_CpuClear16(0, sta, count * sizeof(WlStation));
    __VENEER_MIi_CpuClear16(0, &wlWorkPlain->cam, sizeof(WlCam));
    sta[0].lifeTime = 0xFFFF;
    for (j = 1; j < count; j++) {
        sta[j].lifeTime = 0xFFFF;
    }
    CAM_InitElement(0, BC_ADRS);
    CAM_SetStaState(0, 0x40);
}

void InitCAM(void) {
    WlWork *w = wlWorkPlain;
    WlStation *table = w->param.camTable;
    WlCam *cam = &w->cam;
    u32 i, n = w->param.maxSta;
    for (i = 1; i < n; i++) {
        __VENEER_MIi_CpuClear16(0, &table[i], 26);
    }
    cam->numSta = 1;
    cam->powerMgtMap = 0;
    cam->activeMap = 1;
    cam->unk_06 = 0xFFFE;
    cam->aidCount = 0;
    cam->unk_08 = 0;
    cam->aidMap = 1;
}

void CAM_InitElement(u32 index, const u8 *addr) {
    WlWork *w = wlWorkPlain;
    WlStation *sta = w->param.camTable + index;
    if (!sta->state) {
        w->cam.numSta++;
    }
    __VENEER_MIi_CpuClear16(0, sta, 26);
    w = wlWorkPlain;
    w->cam.unk_08 &= ~(1U << index);
    CAM_SetPowerMgtMode((u16)index, 0);
    w = wlWorkPlain;
    w->cam.activeMap |= (1U << index);
    WSetMacAdrs1(sta->macAdrs, (const u16 *)addr);
    sta->lastSeqCtrl = 0xFFFF;
    sta->supRate = wlWorkPlain->bss.rateSet.supported;
    sta->timer = sta->lifeTime;
    CAM_SetStaState(index & 0xFFFF, 0x20);
}
