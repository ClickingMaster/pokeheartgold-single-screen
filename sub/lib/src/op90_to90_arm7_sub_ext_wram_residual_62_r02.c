#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#define ROOT (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
#define V(o) (*(volatile u16 *)(o))
extern int MatchMacAdrs(const void *, const void *);
extern int IsExistManFrame(const void *, u16);
extern u32 CAM_GetStaState(u32);
extern void CAM_SetStaState(u16, u32);
extern u32 CAM_GetAuthSeed(u32);
extern void CAM_SetAuthSeed(u32, u32);
extern void MLME_IssueDeAuthIndication(const void *, u32);
extern void ClearTimeOut(void);
extern void WSetStaState(u32);
extern void AddTask(u32, u32);
extern u8 *MakeAuthFrame(const void *, u16, u32);
extern void TxManCtrlFrame(void *);
extern void RND_seed(u32);
extern u32 RND_rand(void);
extern u8 WL_ReadByte(const void *);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
void RxAuthFrame(u8 *frame);
static inline BOOL CheckChallenge(u8 *frame)
{
    u32 i, n;
    u16 *p;
    RND_seed(CAM_GetAuthSeed(H(frame, 2)));
    p = (u16 *)(frame + 0x34);
    n = WL_ReadByte(frame + 0x33);
    for (i = 0; i < n >> 1; i++) {
        if (RND_rand() != *p++)
            return FALSE;
    }
    if (n & 1) {
        if ((u8)RND_rand() != (u8)*p)
            return FALSE;
    }
    return TRUE;
}
// @fn RxAuthFrame
void RxAuthFrame(u8 *frame)
{
    u8 *sta;
    u8 *mlme;
    u8 *f;
    u32 send;
    u32 seq;
    u32 status;
    u8 *wl;
    u32 aid;
    wl = ROOT;
    sta = wl + 0x344;
    mlme = wl + 0x404;

    if (H(wl, 0x690) & 8) {
        u16 r = V(0x048080b0);
        u8 *tx = wl + 0x42c;
        if (((r & 1) == 0 || H(tx, 0) == 0) && ((r & 4) == 0 || H(tx, 0x14) == 0) && ((r & 8) == 0 || H(tx, 0x28) == 0)
            && !(V(0x0480819c) & 1)) {
            V(0x04808032) = 0;
            V(0x04808032) = 0x8000;
            H(ROOT, 0x4de) = 0;
        }
    }
    if (IsExistManFrame(frame + 0x1e, 0xb0))
        return;
    seq = (u16)(H(frame, 0x2e) + 1);
    aid = H(frame, 2);
    send = FALSE;
    if (aid == 0) {
        status = 0x13;
        send = TRUE;
        goto tx;
    }
    if (H(sta, 0xc) == 1) {
        if (CAM_GetStaState(aid) > 0x20) {
            CAM_SetStaState(aid, 0x20);
            MLME_IssueDeAuthIndication(frame + 0x1e, 1);
        }
        if ((H(frame, 8) & 0x400) && CAM_GetAuthSeed(aid)) {
            send = TRUE;
            H(frame, 0x2c) = 1;
            status = 0xf;
            seq = 4;
            CAM_SetAuthSeed(aid, 0);
            goto tx;
        }
    }
    switch (H(frame, 0x2c)) {
    case 0:
        if (H(sta, 0xc) == 1 && H(ROOT, 0x332) == 1) {
            status = 0xd;
            send = TRUE;
        } else if (H(sta, 0xc) == 1) {
            send = TRUE;
            if (H(frame, 0x2e) == 1) {
                status = 0;
            } else {
                status = 0xe;
                seq = 2;
            }
        } else if (H(sta, 0xc) != 1 && H(frame, 0x2e) == 2 && H(P(mlme, 0x18), 0x16) == 0 && MatchMacAdrs(P(mlme, 0x18) + 0x10, frame + 0x1e) && H(mlme, 0) == 0x31) {
            ClearTimeOut();
            if (H(frame, 0x30)) {
                H(P(mlme, 0x1c), 4) = 0xc;
                H(P(mlme, 0x1c), 6) = H(frame, 0x30);
            } else {
                WSetStaState(0x30);
                H(P(mlme, 0x1c), 4) = 0;
                H(P(mlme, 0x1c), 6) = 0;
            }
            H(mlme, 0) = 0x35;
            AddTask(2, 2);
        }
        break;
    case 1:
        if (H(sta, 0xc) == 1) {
            CAM_SetStaState(aid, 0x20);
            if (H(frame, 0x2e) == 1) {
                u32 seed, n, i;
                u16 *p;
                f = MakeAuthFrame(frame + 0x1e, 0x80, 1);
                if (f == NULL)
                    break;
                H(f, 0x2c) = H(frame, 0x2c);
                H(f, 0x2e) = seq;
                H(f, 0x30) = 0;
                seed = (u16)(V(0x04808044) + (V(0x04808044) << 8));
                if (seed == 0)
                    seed = 1;
                RND_seed(seed);
                CAM_SetAuthSeed(aid, seed);
                p = (u16 *)(f + 0x34);
                n = WL_ReadByte(f + 0x33);
                for (i = 0; i < n; i += 2)
                    *p++ = RND_rand();
                TxManCtrlFrame(f);
            } else if (H(frame, 0x2e) == 3) {
                if (CAM_GetStaState(aid) != 0x20 || CAM_GetAuthSeed(aid) == 0) {
                    status = 1;
                    send = TRUE;
                } else {
                    if (!CheckChallenge(frame)) {
                        send = TRUE;
                        status = 0xf;
                        CAM_SetAuthSeed(aid, 0);
                    } else {
                        send = TRUE;
                        CAM_SetAuthSeed(aid, 0);
                        status = 0;
                    }
                }
            } else {
                CAM_SetAuthSeed(aid, 0);
                status = 0xe;
                seq = 2;
                send = TRUE;
            }
        } else {
            if (H(P(mlme, 0x18), 0x16) != 1)
                break;
            if (!MatchMacAdrs(P(mlme, 0x18) + 0x10, frame + 0x1e))
                break;
            if (H(frame, 0x2e) == 2) {
                if (H(mlme, 0) != 0x31)
                    break;
                if (H(frame, 0x30)) {
                    ClearTimeOut();
                    H(mlme, 0) = 0x35;
                    H(P(mlme, 0x1c), 4) = 0xc;
                    H(P(mlme, 0x1c), 6) = H(frame, 0x30);
                    AddTask(2, 2);
                    WSetStaState(0x20);
                    break;
                }
                H(mlme, 0) = 0x33;
                f = MakeAuthFrame(frame + 0x1e, WL_ReadByte(frame + 0x33), 1);
                if (f == NULL)
                    break;
                H(f, 0x14) |= 0x4000;
                __VENEER_MIi_CpuCopy16(frame + 0x2c, f + 0x2c, H(frame, 6) + 1);
                H(f, 0x2c) = H(frame, 0x2c);
                H(f, 0x2e) = 3;
                H(f, 0x30) = 0;
                TxManCtrlFrame(f);
            } else if (H(frame, 0x2e) == 4 && H(mlme, 0) == 0x33) {
                ClearTimeOut();
                if (H(frame, 0x30)) {
                    H(P(mlme, 0x1c), 4) = 0xc;
                    H(P(mlme, 0x1c), 6) = H(frame, 0x30);
                } else {
                    WSetStaState(0x30);
                    H(P(mlme, 0x1c), 4) = 0;
                    H(P(mlme, 0x1c), 6) = 0;
                }
                H(mlme, 0) = 0x35;
                AddTask(2, 2);
            }
        }
        break;
    default:
        if (H(sta, 0xc) == 1) {
            status = 0xd;
            send = TRUE;
        }
        break;
    }
tx:
    if (send) {
        f = MakeAuthFrame(frame + 0x1e, 0, status != 0);
        if (f) {
            H(f, 0x2c) = H(frame, 0x2c);
            H(f, 0x2e) = seq;
            H(f, 0x30) = status;
            TxManCtrlFrame(f);
        }
    }
}
