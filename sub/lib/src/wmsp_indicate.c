#pragma force_active on
#include "wmsp_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Fields of the WL driver's indication messages, by byte offset.
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*((u8 *)(p) + (o)))

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Receives the WL driver's messages. Indications (command bit 0x80: MLME 0x84-0x8d, MA 0x180-0x186)
// update the status block and are reported to the ARM9, then freed; anything else is the reply to a
// WMSP_WlRequest and is passed on to wmspW.replyQueue. Exits on a NULL message.
void WMSP_IndicateThread(void *arg) {
    WmspWork *w;
    void *received;
    u8 *rxBuf;
    u8 *mpRecvBuf;
    u8 *mpEndBuf;
    u16 rcv;
    int ok;
    u32 *req;
    int i;
    u16 aid85;
    OSIntrMode irq;
    u16 mode;
    u32 minimum;
    u32 seed;
    u32 len;
    int size;
    int wasMP;
    u16 *cb_1;
    u16 *cb_2;
    u16 *cb_3;
    u16 *cb_4;
    u16 *cb_5;
    u16 *cb_6;
    u16 *cb_7;
    u16 *cb_8;
    u16 *cb_9;
    u16 *cb_10;
    u16 *cb_11;
    u16 *cb_12;
    u16 *cb_13;
    u32 level;
    int timeout;
    int flushed;
    int canKick;
    int retry;
    int ack;
    u8 *child;
    int hasData;
    u16 bitmap;
    u16 aid86;
    int cancelled;
    u32 rssi;
    u16 ordinal;
    u16 kind;
    u16 aid84;
    u16 oldFlag;
    u32 missing;
    void *message;
    int j;
    u8 *data;
    u64 last;
    WmspStatus *cur;
    u16 command;
    int resume;
    u64 tick;
    u32 bit;
    int accept;
    u16 more;
    vu16 *timer;
    u8 mac[6];
    u16 current;
    u16 upper;
    WmspStatus *st;
    u16 old;

    w = &wmspW;
    for (;;) {
        __VENEER_OS_ReceiveMessage(&w->indicateQueue, &received, OS_MESSAGE_BLOCK);
        message = received;
        if (!message) {
            __VENEER_OS_ExitThread();
            return;
        }
        kind = H(message, 12) & 0xff00;
        command = H(message, 12) & 0xff;
        if ((kind != 0 && kind != 0x100) || !(command & 0x80)) {
            goto forward;
        }
        if (wmspW.arm9Buf == NULL) {
            continue;
        }
        st = wmspW.status;
        mode = st->state;
        if (mode == 1) {
            continue;
        }
        switch (((u16 *)message)[6]) {
        case 0x84:
            cb_1 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb_1[0] = 0x80;
            cb_1[1] = 0;
            cb_1[2] = 0x13;
            __VENEER_WMSP_ReturnResult2Wm9(cb_1);
            break;
        case 0x85:
            if (mode == 7 || mode == 9) {
                __VENEER_MI_CpuCopy8((u8 *)message + 16, mac, 6);
                aid85 = 0;
                for (i = 0; i < 15; i++) {
                    irq = __VENEER_OS_DisableInterrupts();
                    if ((st->childBitmap & (1 << (i + 1))) && mac[0] == st->childMacAddress[i][0] && mac[1] == st->childMacAddress[i][1] && mac[2] == st->childMacAddress[i][2] && mac[3] == st->childMacAddress[i][3] && mac[4] == st->childMacAddress[i][4] && mac[5] == st->childMacAddress[i][5]) {
                        aid85 = (u16)(i + 1);
                        st->childBitmap &= ~(1 << aid85);
                        st->unk_86 &= ~(1 << aid85);
                        st->lastRecvTick[aid85] = 0;
                        __VENEER_MI_CpuFill8(st->childMacAddress[i], 0, 6);
                        __VENEER_OS_RestoreInterrupts(irq);
                        break;
                    }
                    __VENEER_OS_RestoreInterrupts(irq);
                }
                if (!aid85) {
                    break;
                }
                cb_2 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                cb_2[0] = 8;
                cb_2[1] = 0;
                cb_2[4] = 9;
                cb_2[9] = H(message, 22);
                cb_2[8] = aid85;
                __VENEER_MI_CpuCopy8((u8 *)message + 16, cb_2 + 5, 6);
                cb_2[22] = st->parentSize;
                cb_2[23] = st->childSize;
                __VENEER_WMSP_ReturnResult2Wm9(cb_2);
                if (st->mpActive == 1) {
                    WMSP_CleanSendQueue((u16)(1 << aid85));
                }
            } else {
                wasMP = 0;
                irq = __VENEER_OS_DisableInterrupts();
                if (!st->childBitmap) {
                    __VENEER_OS_RestoreInterrupts(irq);
                    break;
                }
                if (st->mpActive == 1) {
                    st->mpActive = 0;
                    wasMP = 1;
                    WMSP_CancelVAlarm();
                    WMSP_SetThreadPriorityLow();
                }
                st->childBitmap = 0;
                st->unk_86 = 0;
                st->unk_14 = 0;
                st->dcfActive = 0;
                st->unk_1c = 0;
                st->wepEnabled = 0;
                st->wepMode = 0;
                __VENEER_MI_CpuFill8(st->wepKey, 0, 80);
                WMSP_ResetSizeVars();
                st->unk_c2 = 0;
                st->state = 3;
                __VENEER_OS_RestoreInterrupts(irq);
                cb_2 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                cb_2[0] = 12;
                cb_2[1] = 0;
                cb_2[4] = 9;
                cb_2[6] = H(message, 22);
                cb_2[5] = st->aid;
                __VENEER_MI_CpuCopy8(st->bssid, cb_2 + 8, 6);
                cb_2[11] = st->parentSize;
                cb_2[12] = st->childSize;
                __VENEER_WMSP_ReturnResult2Wm9(cb_2);
                if (wasMP) {
                    WMSP_CleanSendQueue(1);
                }
            }
            break;
        case 0x86: {
            aid86 = H(message, 22);
            if (aid86 == 0 || aid86 >= 16) {
                break;
            }
            if (!st->parentParam.unk_0e) {
                req = WMSP_GetInternalRequestBuf();
                if (!req) {
                    ok = 0;
                } else {
                    req[0] = 0x22;
                    __VENEER_MI_CpuCopy8((u8 *)message + 16, req + 1, 6);
                    ok = __VENEER_OS_SendMessage(&wmspW.pxiQueue, req, OS_MESSAGE_NOBLOCK);
                }
                if (!ok) {
                    cb_3 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                    cb_3[0] = 0x80;
                    cb_3[1] = 8;
                    cb_3[2] = 0x16;
                    cb_3[3] = 0x22;
                    __VENEER_WMSP_ReturnResult2Wm9(cb_3);
                }
                break;
            }
            irq = __VENEER_OS_DisableInterrupts();
            st->childBitmap |= 1 << aid86;
            st->unk_86 &= ~(1 << aid86);
            st->lastRecvTick[aid86] = __VENEER_OS_GetTick() | 1;
            __VENEER_MI_CpuCopy8((u8 *)message + 16, st->childMacAddress[aid86 - 1], 6);
            __VENEER_OS_RestoreInterrupts(irq);
            __VENEER_MIi_CpuClear16(1, st->unk_1f8[aid86], 16);
            cb_3 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb_3[0] = 8;
            cb_3[1] = 0;
            cb_3[4] = 7;
            __VENEER_MI_CpuCopy8((u8 *)message + 16, cb_3 + 5, 6);
            cb_3[8] = aid86;
            __VENEER_MIi_CpuCopy16((u8 *)message + 34, cb_3 + 10, 24);
            cb_3[22] = st->parentSize;
            cb_3[23] = st->childSize;
            __VENEER_WMSP_ReturnResult2Wm9(cb_3);
            break;
        }
        case 0x87:
            cb_4 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb_4[0] = 0x80;
            cb_4[1] = 0;
            cb_4[2] = 0x12;
            __VENEER_WMSP_ReturnResult2Wm9(cb_4);
            break;
        case 0x88:
            cb_5 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb_5[0] = 0x80;
            cb_5[1] = 0;
            cb_5[2] = 0x11;
            __VENEER_WMSP_ReturnResult2Wm9(cb_5);
            break;
        case 0x8b:
            if (st->unk_c2) {
                cb_6 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                cb_6[0] = 12;
                cb_6[1] = 0;
                cb_6[4] = 8;
                __VENEER_WMSP_ReturnResult2Wm9(cb_6);
            }
            break;
        case 0x8c:
            if (st->unk_c2) {
                cb_7 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                cb_7[0] = 8;
                cb_7[1] = 0;
                cb_7[4] = 2;
                __VENEER_WMSP_ReturnResult2Wm9(cb_7);
            }
            break;
        case 0x8d: {
            seed = WMSP_GetRssi8_06001B5C(B(message, 31));
            seed ^= *(vu16 *)0x027fff98 << 1;
            seed ^= seed >> 16;
            *(vu16 *)0x027fff98 = (u16)seed;
            if (st->state != 8 && st->state != 10) {
                break;
            }
            if (st->unk_ba != H(message, 0x44)) {
                req = WMSP_GetInternalRequestBuf();
                if (!req) {
                    ok = 0;
                } else {
                    req[0] = 0x25;
                    req[1] = 1;
                    req[2] = 0x8002;
                    ok = __VENEER_OS_SendMessage(&wmspW.pxiQueue, req, OS_MESSAGE_NOBLOCK);
                }
                if (!ok) {
                    cb_8 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                    cb_8[0] = 0x80;
                    cb_8[1] = 8;
                    cb_8[2] = 0x16;
                    cb_8[3] = 0x25;
                    __VENEER_WMSP_ReturnResult2Wm9(cb_8);
                }
                break;
            }
            if (!wmspW.status->unk_c2) {
                break;
            }
            cb_8 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb_8[0] = 0x80;
            cb_8[1] = 0;
            cb_8[2] = 0x10;
            cb_8[3] = H(message, 0x44);
            cb_8[4] = st->state;
            cb_8[5] = H(message, 22);
            if (cb_8[5] <= 128) {
                __VENEER_MIi_CpuCopy16((u8 *)message + 0x3c, cb_8 + 6, (cb_8[5] + 1) & ~1);
            }
            __VENEER_WMSP_ReturnResult2Wm9(cb_8);
            break;
        }
        case 0x180: {
            if (!st->dcfActive) {
                break;
            }
            WMSP_AddRssiToList(WMSP_GetRssi8_06001B5C(B(message, 31)));
            st->linkLevel = WMSP_GetAverageLinkLevel();
            if (WMSP_CheckMacAddress((u8 *)message + 0x2e) == 1) {
                break;
            }
            if (H(message, 22) > 0x5e4) {
                break;
            }
            st->rxBufIndex ^= 1;
            rxBuf = st->rxBuf[st->rxBufIndex];
            __VENEER_MI_CpuCopy8((u8 *)message + 16, rxBuf, (H(message, 22) + 0x2d) & ~1);
            __VENEER_MI_CpuCopy8((u8 *)message + 0x28, rxBuf + 0x18, 6);
            __VENEER_MI_CpuCopy8((u8 *)message + 0x2e, rxBuf + 0x1e, 6);
            cb_9 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb_9[0] = 0x11;
            cb_9[1] = 0;
            cb_9[2] = 15;
            *(u8 **)(cb_9 + 4) = rxBuf;
            __VENEER_WMSP_ReturnResult2Wm9(cb_9);
            break;
        }
        case 0x182: {
            rssi = WMSP_GetRssi8_06001B5C(B(message, 31));
            if (st->minRssi > rssi) {
                st->minRssi = (u16)rssi;
            }
            if (!st->mpActive) {
                break;
            }
            if (st->unk_60 == 1) {
                st->unk_60 = 0;
            }
            oldFlag = st->unk_8e;
            st->mpBufIndex ^= 1;
            mpRecvBuf = st->mpBuf[st->mpBufIndex];
            len = H(message, 22) + 0x30;
            if (st->mpBufSize < len) {
                len = st->mpBufSize;
            }
            __VENEER_MI_CpuCopy8((u8 *)message + 16, mpRecvBuf, len);
            irq = __VENEER_OS_DisableInterrupts();
            cancelled = 0;
            if (st->mpAckAlarmSet == 1) {
                cancelled = 1;
                __VENEER_OS_CancelAlarm(&wmspMPAckAlarm);
            }
            st->mpAckAlarmSet = 1;
            st->unk_82 = H(mpRecvBuf, 10);
            more = H(message, 24);
            hasData = (more & 0x2000) ? 1 : 0;
            st->unk_90 = (u16)hasData;
            __VENEER_OS_SetAlarm(&wmspMPAckAlarm, OS_MilliSecondsToTicks((s64)(((u16)(H(mpRecvBuf, 10) - H(mpRecvBuf, 12)) + 128) << 4)) / 1024, WmspMaMultiPollAckAlarmCallback, 0);
            ack = (more & 0x2800) == 0x2800;
            retry = (more & 0x6000) == 0x6000;
            if (retry) {
                st->unk_8a = 0;
            }
            st->unk_8e = ack ? 1 : 0;
            st->unk_8c = retry ? 1 : 0;
            if (hasData) {
                size = (H(mpRecvBuf, 44) - 102) / 4 - 32;
                if (size >= 0) {
                    if (size > 512) {
                        size = 512;
                    }
                    if (size != st->childSize) {
                        WMSP_SetChildSize((u16)size);
                    }
                }
            }
            __VENEER_OS_RestoreInterrupts(irq);
            if (cancelled) {
                if (oldFlag == 1) {
                    WMSP_FlushSendQueue(cancelled, 0);
                }
                cb_10 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                cb_10[0] = 14;
                cb_10[1] = 9;
                cb_10[2] = 13;
                *(u32 *)(cb_10 + 4) = 0;
                __VENEER_WMSP_ReturnResult2Wm9(cb_10);
            }
            if (!hasData) {
                if (H(mpRecvBuf, 6) >= 2) {
                    st->unk_5e = (H(mpRecvBuf, 0x30) & 0x8000) ? 1 : 0;
                }
                break;
            }
            __VENEER_MI_CpuCopy8((u8 *)message + 0x28, mpRecvBuf + 0x18, 6);
            __VENEER_MI_CpuCopy8((u8 *)message + 0x2e, mpRecvBuf + 0x1e, 6);
            if (H(mpRecvBuf, 6) >= 2) {
                H(mpRecvBuf, 6) -= 2;
                st->unk_5e = (H(mpRecvBuf, 0x30) & 0x8000) ? 1 : 0;
                cb_10 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                cb_10[0] = 14;
                cb_10[1] = 0;
                cb_10[2] = 12;
                *(u8 **)(cb_10 + 4) = mpRecvBuf;
                __VENEER_WMSP_ReturnResult2Wm9(cb_10);
                if (H(mpRecvBuf, 6)) {
                    WMSP_ParsePortPacket(0, H(mpRecvBuf, 0x30), mpRecvBuf + 0x32, H(mpRecvBuf, 6), mpRecvBuf);
                }
            } else {
                H(mpRecvBuf, 6) = 0;
                st->unk_5e = 0;
                cb_10 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                cb_10[0] = 14;
                cb_10[1] = 14;
                cb_10[2] = 12;
                *(u8 **)(cb_10 + 4) = mpRecvBuf;
                __VENEER_WMSP_ReturnResult2Wm9(cb_10);
            }
            if (st->recvTimeout != 0) {
                st->lastRecvTick[0] = __VENEER_OS_GetTick() | 1;
            }
            break;
        }
        case 0x184: {
            resume = 0;
            if (!st->mpActive) {
                break;
            }
            if (H(message, 18) != 0 || (st->resumePending == 1 && H(message, 16) != 0)) {
                WMSP_RequestResumeMP();
                break;
            }
            if (st->resumePending) {
                st->resumePending = 0;
            }
            mpEndBuf = st->mpBuf[st->mpBufIndex];
            len = H(message, 22) * H(message, 20) + 10;
            if (st->mpBufSize < len) {
                len = st->mpBufSize;
            }
            __VENEER_MI_CpuCopy8((u8 *)message + 16, mpEndBuf, len);
            cur = wmspW.status;
            minimum = cur->minRssi;
            if (H(mpEndBuf, 0) == 0 && H(message, 20) >= 1) {
                data = (u8 *)message + 26;
                for (j = 0; j < H(message, 20); j++) {
                    level = WMSP_GetRssi8_06001B5C(data[H(message, 22) * j + 3]);
                    if (level < minimum) {
                        minimum = level;
                    }
                }
                cur->minRssi = minimum;
            }
            tick = __VENEER_OS_GetTick() | 1;
            missing = H(mpEndBuf, 0);
            child = mpEndBuf + 10;
            for (ordinal = 0; ordinal < H(mpEndBuf, 4); ordinal++, child += H(mpEndBuf, 6)) {
                aid84 = H(child, 4);
                rcv = H(child, 0);
                if (aid84 < 1 || aid84 > 15) {
                    continue;
                }
                if (rcv >= 2 && rcv != 0xffff) {
                    rcv = (u16)(rcv - 2);
                    H(child, 0) = rcv;
                    st->unk_86 |= 1 << aid84;
                    st->lastRecvTick[aid84] = tick;
                    if (rcv) {
                        WMSP_ParsePortPacket(aid84, H(child, 8), child + 10, rcv, mpEndBuf);
                    }
                } else if (rcv == 0) {
                    last = st->lastRecvTick[aid84];
                    bit = 1 << aid84;
                    missing |= 1 << aid84;
                    if (st->recvTimeout != 0 && last != 0 && tick - last > st->recvTimeout) {
                        req = WMSP_GetInternalRequestBuf();
                        st->lastRecvTick[aid84] = 0;
                        if (!req) {
                            ok = 0;
                        } else {
                            req[0] = 0x25;
                            req[1] = bit;
                            req[2] = 0x8001;
                            ok = __VENEER_OS_SendMessage(&wmspW.pxiQueue, req, OS_MESSAGE_NOBLOCK);
                        }
                        if (!ok) {
                            cb_11 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                            cb_11[0] = 0x80;
                            cb_11[1] = 8;
                            cb_11[2] = 0x16;
                            cb_11[3] = 0x25;
                            __VENEER_WMSP_ReturnResult2Wm9(cb_11);
                        }
                    }
                }
            }
            WMSP_FlushSendQueue(0, (u16)missing);
            if (H(mpEndBuf, 0)) {
                resume = 1;
            }
            cb_11 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb_11[0] = 14;
            cb_11[1] = 0;
            cb_11[2] = 11;
            *(u8 **)(cb_11 + 4) = mpEndBuf;
            __VENEER_WMSP_ReturnResult2Wm9(cb_11);
            st->mpBufIndex ^= 1;
            irq = __VENEER_OS_DisableInterrupts();
            if (!resume) {
                st->unk_62--;
            }
            if (st->unk_64 > 0) {
                st->unk_64--;
            }
            canKick = st->unk_62 > 0 && st->unk_64 > 0;
            __VENEER_OS_RestoreInterrupts(irq);
            if (canKick) {
                if (resume == 1) {
                    bitmap = H(message, 16);
                } else {
                    bitmap = 0xffff;
                }
                if (st->parentInterval) {
                    __VENEER_OS_CancelAlarm(&wmspMPIntervalAlarm);
                    __VENEER_OS_SetAlarm(&wmspMPIntervalAlarm, st->parentIntervalTick, WmspMPParentIntervalAlarmCallback, (void *)bitmap);
                } else {
                    WmspKickMPParent(bitmap);
                }
            }
            break;
        }
        case 0x185: {
            flushed = 0;
            if (!st->mpActive) {
                break;
            }
            if (!H(message, 14)) {
                timer = (vu16 *)0x048080f8;
                timeout = 1;
                old = timer[0];
                upper = timer[1];
                current = timer[0];
                if (old > current) {
                    upper = timer[1];
                }
                if ((s16)((u16)((upper << 12) | (current >> 4)) - st->unk_82) <= 0) {
                    break;
                }
            } else {
                timeout = 0;
            }
            irq = __VENEER_OS_DisableInterrupts();
            if (!st->mpAckAlarmSet) {
                __VENEER_OS_RestoreInterrupts(irq);
                break;
            }
            st->mpAckAlarmSet = 0;
            hasData = st->unk_90;
            __VENEER_OS_CancelAlarm(&wmspMPAckAlarm);
            __VENEER_OS_RestoreInterrupts(irq);
            retry = st->unk_8c;
            accept = 1;
            if (retry && (timeout || !(H(message, 0x3e) & (1 << st->aid)))) {
                accept = 0;
            }
            if (retry) {
                st->unk_8c = 0;
            }
            if (st->unk_8e) {
                st->unk_8e = 0;
                flushed = WMSP_FlushSendQueue(timeout, accept ? 1 : 0);
            }
            if (!hasData) {
                break;
            }
            cb_12 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
            cb_12[0] = 14;
            if (timeout) {
                cb_12[1] = 9;
            } else if (H(message, 0x3e) & (1 << st->aid)) {
                cb_12[1] = 15;
            } else {
                cb_12[1] = 0;
            }
            cb_12[2] = 13;
            *(u32 *)(cb_12 + 4) = 0;
            if (!timeout) {
                cb_12[6] = H(message, 28);
                cb_12[7] = H(message, 30);
                __VENEER_MI_CpuCopy8((u8 *)message + 0x28, cb_12 + 8, 6);
                __VENEER_MI_CpuCopy8((u8 *)message + 0x2e, cb_12 + 11, 6);
                cb_12[14] = H(message, 0x3a);
                cb_12[15] = H(message, 0x3c);
                cb_12[16] = H(message, 0x3e);
            }
            __VENEER_WMSP_ReturnResult2Wm9(cb_12);
            if (hasData) {
                if (flushed == 1 || !st->unk_5e) {
                    if (st->childInterval) {
                        __VENEER_OS_CancelAlarm(&wmspMPIntervalAlarm);
                        __VENEER_OS_SetAlarm(&wmspMPIntervalAlarm, st->childIntervalTick, WmspMPChildIntervalAlarmCallback, 0);
                    } else {
                        WmspKickMPChild();
                    }
                } else {
                    st->unk_5e = 0;
                    st->unk_60 = 1;
                    st->unk_88 = 0;
                }
            }
            break;
        }
        case 0x186:
            if (st->unk_7cc == 1 && H(message, 16) == 32) {
                irq = __VENEER_OS_DisableInterrupts();
                __VENEER_OS_CancelAlarm(&wmspMPAckAlarm);
                st->mpAckAlarmSet = 0;
                __VENEER_OS_RestoreInterrupts(irq);
                st->unk_8a = 0;
                WMSP_FlushSendQueue(1, 0);
                cb_13 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                cb_13[0] = 0x80;
                cb_13[1] = 0;
                cb_13[2] = 0x17;
                cb_13[3] = 1;
                __VENEER_WMSP_ReturnResult2Wm9(cb_13);
                break;
            }
            req = WMSP_GetInternalRequestBuf();
            if (!req) {
                ok = 0;
            } else {
                req[0] = 0x25;
                req[2] = 0x8003;
                if (st->state == 9 || st->state == 7) {
                    req[1] = 0x7ffe;
                } else if (st->state == 10 || st->state == 8) {
                    req[1] = 1;
                }
                ok = __VENEER_OS_SendMessage(&wmspW.pxiQueue, req, OS_MESSAGE_NOBLOCK);
            }
            if (!ok) {
                cb_13 = __VENEER_WMSP_GetBuffer4Callback2Wm9();
                cb_13[0] = 0x80;
                cb_13[1] = 8;
                cb_13[2] = 0x16;
                cb_13[3] = 0x25;
                __VENEER_WMSP_ReturnResult2Wm9(cb_13);
            }
            break;
        default:
            break;
        }
        WmspFreeBufOfWL(message);
        continue;
forward:
        __VENEER_OS_SendMessage(&w->replyQueue, message, OS_MESSAGE_BLOCK);
    }
}

// clang-format off
asm void __VENEER_OS_ExitThread(void) {
    ldr pc, =OS_ExitThread
}
// clang-format on

// clang-format off
asm void __VENEER_MI_CpuFill8(void *dest, u8 data, u32 size) {
    ldr pc, =MI_CpuFill8
}
// clang-format on

// clang-format off
asm OSTick __VENEER_OS_GetTick(void) {
    ldr pc, =OS_GetTick
}
// clang-format on

// clang-format off
asm void __VENEER_MIi_CpuCopy16(const void *src, void *dest, u32 size) {
    ldr pc, =MIi_CpuCopy16
}
// clang-format on

// clang-format off
asm void __VENEER_OS_CancelAlarm(OSAlarm *alarm) {
    ldr pc, =OS_CancelAlarm
}
// clang-format on

// clang-format off
asm void __VENEER_OS_SetAlarm(OSAlarm *alarm, OSTick tick, OSAlarmHandler handler, void *arg) {
    ldr pc, =OS_SetAlarm
}
// clang-format on

void WmspFreeBufOfWL(void *buf) {
    OSIntrMode irq = __VENEER_OS_DisableInterrupts();
    __VENEER_OS_FreeToHeap(wmspW.heapArena, wmspW.heapHandle, buf);
    __VENEER_OS_RestoreInterrupts(irq);
}

// clang-format off
asm void __VENEER_OS_FreeToHeap(OSArenaId id, OSHeapHandle heap, void *ptr) {
    ldr pc, =OS_FreeToHeap
}
// clang-format on

// Converts the RSSI byte of a received frame.
u8 WMSP_GetRssi8_06001B5C(int rssi) {
    if (rssi & 2) {
        return rssi >> 2;
    } else {
        return (rssi >> 2) + 25;
    }
}

#pragma dont_inline reset
