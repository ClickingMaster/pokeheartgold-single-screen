#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {
    u16 state;      /* 0x00 */
    u16 x02;
    u8 macAdrs[6];  /* 0x04 */
    u8 pad0a[0x18 - 0x0a];
    u16 timer;      /* 0x18 */
    u16 x1a;
} CamSta;
typedef struct {
    u8 pad0[0x31c];
    CamSta *sta;          /* 0x31c */
    u8 pad320[0x322 - 0x320];
    u16 maxSta;           /* 0x322 */
    u8 pad324[0x350 - 0x324];
    u16 mode;             /* 0x350 */
    u8 pad352[0x3cc - 0x352];
    u16 aid;              /* 0x3cc */
    u8 pad3ce[0x52c - 0x3ce];
    u16 numSta;           /* 0x52c */
    u16 x52e;
    u16 map530;           /* 0x530 */
    u16 map532;
    u16 map534;           /* 0x534 */
} WlWork;
#define ROOT (*(WlWork **)0x0380fff4)
extern u16 CAM_GetStaState(u32 aid);
extern void CAM_SetStaState(u16 aid, u16 state);
extern void DeleteTxFrames(u32 aid);
extern void CAM_SetPowerMgtMode(u16 aid, u16 mode);
extern u16 *MakeDeAuthFrame(void *mac, u16 reason, u16 x);
extern void TxManCtrlFrame(u16 *frame);
extern void MLME_IssueDeAuthIndication(void *mac, u16 reason);
extern void WSetStaState(u16 state);
extern void WClearAids(void);
void CAM_TimerTask(void);
// @fn CAM_TimerTask
void CAM_TimerTask(void) {
    WlWork *w;
    CamSta *sta;
    u16 numSta;
    u32 aid;
    u32 count;

    count = 0;
    w = ROOT;
    aid = 1;
    sta = w->sta + 1;
    numSta = w->numSta;
    for (; aid < w->maxSta; aid++, sta++) {
        if (sta->state != 0) {
            if (sta->timer != 0 && sta->timer != 0xffff) {
                sta->timer--;
                if (sta->timer == 0) {
                    if (sta->state >= 0x20) {
                        u16 prevState = CAM_GetStaState(aid);
                        CAM_SetStaState(aid, 0x20);
                        DeleteTxFrames(aid);
                        if (w->mode == 1) {
                            if (prevState <= 0x20) {
                                goto clear;
                            }
                            {
                                u16 *frame;
                                ROOT->map534 |= 1 << aid;
                                CAM_SetPowerMgtMode(aid, 0);
                                ROOT->map530 |= 1 << aid;
                                frame = MakeDeAuthFrame(sta->macAdrs, 1, 0);
                                if (frame) {
                                    *frame = 1;
                                    TxManCtrlFrame(frame);
                                    count++;
                                    continue;
                                }
                                MLME_IssueDeAuthIndication(sta->macAdrs, 1);
                            }
                        } else if (aid == w->aid) {
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
                    w->numSta--;
                }
            }
            count++;
        }
        if (count >= numSta) {
            break;
        }
    }
}
