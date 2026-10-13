#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// Body of a received / sent authentication frame.
#define RX_AUTH(frame)     WL_RX_BODY(frame, WlAuthBody)
#define TX_AUTH(heapFrame) ((WlAuthBody *)&(heapFrame)->frame.body0)

// Shared key, parent: the challenge text of the station's third frame against the one sent (the random sequence
// of the station's seed).
static inline BOOL CheckChallenge(WlRxFrame *frame) {
    u32 i, n;
    u16 *text;
    RND_seed(CAM_GetAuthSeed(frame->station));
    text = (u16 *)(RX_AUTH(frame)->challenge + 2);
    n = WL_ReadByte(RX_AUTH(frame)->challenge + 1);
    for (i = 0; i < n >> 1; i++) {
        if (RND_rand() != *text++) {
            return FALSE;
        }
    }
    if (n & 1) {
        if ((u8)RND_rand() != (u8)*text) {
            return FALSE;
        }
    }
    return TRUE;
}

// Authentication frame. Parent: runs the station's side of open system / shared key authentication (sequence 1 and
// 3) and answers (status 0: success, 1: failure, 13: algorithm not supported, 14: bad sequence, 15: challenge
// failed, 19: station table full). Child: the parent's answer (sequence 2 and 4) to the MLME authentication request
// in progress (state 0x31 / 0x33), sends the challenge back for shared key.
void RxAuthFrame(WlRxFrame *frame) {
    WlBss *bss;
    WlMlme *mlme;
    WlTxHeapFrame *reply;
    u32 send;
    u32 seqNo;
    u32 status;
    WlWork *w;
    u32 station;
    w = wlWorkPlain;
    bss = &w->bss;
    mlme = &w->mlme;

    if (w->hwWorkarounds & 8) {
        // TX reset workaround: reset the MAC's TX side unless a queue is sending
        u16 busy = WL_REG(0x0B0);
        WlTxCtrl *tx = &w->txCtrl;
        if (((busy & 1) == 0 || tx->txq[0].active == 0) && ((busy & 4) == 0 || tx->txq[1].active == 0) && ((busy & 8) == 0 || tx->txq[2].active == 0) && !(WL_REG(0x19C) & 1)) {
            WL_REG(0x032) = 0;
            WL_REG(0x032) = 0x8000;
            wlWorkPlain->rxCtrl.cntupBusyCount = 0;
        }
    }
    if (IsExistManFrame(frame->addr2, 0xB0)) {
        return;
    }
    seqNo = (u16)(RX_AUTH(frame)->seqNo + 1);
    station = frame->station;
    send = FALSE;
    if (station == 0) {
        status = 19;
        send = TRUE;
        goto tx;
    }
    if (bss->mode == 1) {
        if (CAM_GetStaState(station) > 0x20) {
            CAM_SetStaState(station, 0x20);
            MLME_IssueDeAuthIndication(frame->addr2, 1);
        }
        if ((frame->status & 0x400) && CAM_GetAuthSeed(station)) {
            send = TRUE;
            RX_AUTH(frame)->algorithm = 1;
            status = 15;
            seqNo = 4;
            CAM_SetAuthSeed(station, 0);
            goto tx;
        }
    }
    switch (RX_AUTH(frame)->algorithm) {
    case 0: // open system
        if (bss->mode == 1 && wlWorkPlain->param.authAlgo == 1) {
            status = 13;
            send = TRUE;
        } else if (bss->mode == 1) {
            send = TRUE;
            if (RX_AUTH(frame)->seqNo == 1) {
                status = 0;
            } else {
                status = 14;
                seqNo = 2;
            }
        } else if (bss->mode != 1 && RX_AUTH(frame)->seqNo == 2 && mlme->req.peer->param == 0 && MatchMacAdrs(mlme->req.peer->peerMacAdrs, frame->addr2) && mlme->state == 0x31) {
            ClearTimeOut();
            if (RX_AUTH(frame)->statusCode) {
                mlme->cfm.auth->header.result = 12;
                mlme->cfm.auth->statusCode = RX_AUTH(frame)->statusCode;
            } else {
                WSetStaState(0x30);
                mlme->cfm.auth->header.result = 0;
                mlme->cfm.auth->statusCode = 0;
            }
            mlme->state = 0x35;
            AddTask(2, 2);
        }
        break;
    case 1: // shared key
        if (bss->mode == 1) {
            CAM_SetStaState(station, 0x20);
            if (RX_AUTH(frame)->seqNo == 1) {
                // send a challenge text: the random sequence of a new seed
                u16 seed;
                u32 n, i;
                u16 *text;
                reply = MakeAuthFrame(frame->addr2, 0x80, 1);
                if (reply == NULL) {
                    break;
                }
                TX_AUTH(reply)->algorithm = RX_AUTH(frame)->algorithm;
                TX_AUTH(reply)->seqNo = seqNo;
                TX_AUTH(reply)->statusCode = 0;
                seed = WL_REG(0x044) + (WL_REG(0x044) << 8);
                if (seed == 0) {
                    seed = 1;
                }
                RND_seed(seed);
                CAM_SetAuthSeed(station, seed);
                text = (u16 *)(TX_AUTH(reply)->challenge + 2);
                n = WL_ReadByte(TX_AUTH(reply)->challenge + 1);
                for (i = 0; i < n; i += 2) {
                    *text++ = RND_rand();
                }
                TxManCtrlFrame(reply);
            } else if (RX_AUTH(frame)->seqNo == 3) {
                if (CAM_GetStaState(station) != 0x20 || CAM_GetAuthSeed(station) == 0) {
                    status = 1;
                    send = TRUE;
                } else {
                    if (!CheckChallenge(frame)) {
                        send = TRUE;
                        status = 15;
                        CAM_SetAuthSeed(station, 0);
                    } else {
                        send = TRUE;
                        CAM_SetAuthSeed(station, 0);
                        status = 0;
                    }
                }
            } else {
                CAM_SetAuthSeed(station, 0);
                status = 14;
                seqNo = 2;
                send = TRUE;
            }
        } else {
            if (mlme->req.peer->param != 1) {
                break;
            }
            if (!MatchMacAdrs(mlme->req.peer->peerMacAdrs, frame->addr2)) {
                break;
            }
            if (RX_AUTH(frame)->seqNo == 2) {
                if (mlme->state != 0x31) {
                    break;
                }
                if (RX_AUTH(frame)->statusCode) {
                    ClearTimeOut();
                    mlme->state = 0x35;
                    mlme->cfm.auth->header.result = 12;
                    mlme->cfm.auth->statusCode = RX_AUTH(frame)->statusCode;
                    AddTask(2, 2);
                    WSetStaState(0x20);
                    break;
                }
                // send the challenge back, encrypted
                mlme->state = 0x33;
                reply = MakeAuthFrame(frame->addr2, WL_ReadByte(RX_AUTH(frame)->challenge + 1), 1);
                if (reply == NULL) {
                    break;
                }
                reply->frame.frameCtrl |= 0x4000;
                __VENEER_MIi_CpuCopy16(RX_AUTH(frame), TX_AUTH(reply), frame->length + 1);
                TX_AUTH(reply)->algorithm = RX_AUTH(frame)->algorithm;
                TX_AUTH(reply)->seqNo = 3;
                TX_AUTH(reply)->statusCode = 0;
                TxManCtrlFrame(reply);
            } else if (RX_AUTH(frame)->seqNo == 4 && mlme->state == 0x33) {
                ClearTimeOut();
                if (RX_AUTH(frame)->statusCode) {
                    mlme->cfm.auth->header.result = 12;
                    mlme->cfm.auth->statusCode = RX_AUTH(frame)->statusCode;
                } else {
                    WSetStaState(0x30);
                    mlme->cfm.auth->header.result = 0;
                    mlme->cfm.auth->statusCode = 0;
                }
                mlme->state = 0x35;
                AddTask(2, 2);
            }
        }
        break;
    default:
        if (bss->mode == 1) {
            status = 13;
            send = TRUE;
        }
        break;
    }
tx:
    if (send) {
        reply = MakeAuthFrame(frame->addr2, 0, status != 0);
        if (reply) {
            TX_AUTH(reply)->algorithm = RX_AUTH(frame)->algorithm;
            TX_AUTH(reply)->seqNo = seqNo;
            TX_AUTH(reply)->statusCode = status;
            TxManCtrlFrame(reply);
        }
    }
}
