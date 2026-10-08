#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#define ROOT (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
typedef struct {
    u32 head;
    u32 tail;
    u16 count;
    u16 pad;
} TxQueue;
typedef struct {
    u16 busy;
    u16 txCount;
    u16 errCount;
    u16 pad;
    u8 *macBuf;
    u8 *frame;
    void (*callback)(void *, u32);
} TxCtrl;
extern u32 __VENEER_OS_DisableIrqMask(u32);
extern u32 __VENEER_OS_EnableIrqMask(u32);
extern u32 GetHeapBufNextAdrs(u32);
extern int CheckFrameTimeout(void *);
extern u32 CAM_GetStaState(u32);
extern int CAM_IsActive(u32);
extern void IssueMaDataConfirm(void *, void *);
extern void CAM_DecFrameCount(void *);
extern void WSetPowerState(u32);
extern void CopyTxFrmToMacBuf(void *, void *);
extern u32 CAM_GetFrameCount(u32);
void TxqPri(u32 pri);
// @fn TxqPri
void TxqPri(u32 pri)
{
    u8 * cfg;
    u8 * wl = ROOT;
    u8 * mac;
    TxCtrl * c;
    TxQueue * q = (TxQueue *)(wl + 0x194) + pri;
    u32 cur, x, next;
    mac = wl + 0x42c;
    c = (TxCtrl *)mac + pri;
    cfg = wl + 0x344;
    u8 * f, * buf;
    s32 aid, fc;
    vu16 * reg;
    u32 ofs;
    if (q->count == 0)
    {
        return;
    }
    x = __VENEER_OS_DisableIrqMask(0x1000000);
    if (c->busy)
    {
        __VENEER_OS_EnableIrqMask(x);
        return;
    }
    next = q->head;
    while (1)
    {
        if (next == 0xffffffff)
        {
            __VENEER_OS_EnableIrqMask(x);
            return;
        }
        cur = next;
        next = GetHeapBufNextAdrs(cur);
        f = (u8 *)cur + 0x10;
        aid = H(f, 2);
        if (CheckFrameTimeout(f))
        {
            H(mac, 0xae)++;
            H(f, 8) = 2;
            c->errCount++;
            c->callback(f, 0);
            continue;
        }
        if (0 == pri || (pri == 1 && CAM_GetStaState(aid) == 0x40))
        {
            if (!CAM_IsActive(aid))
            {
                continue;
            }
            if (CAM_GetStaState(aid) != 0x40)
            {
                H(f, 8) = 2;
                IssueMaDataConfirm(q, f - 0x10);
                CAM_DecFrameCount(f);
                continue;
            }
        }
        break;
    }
    c->busy = 1;
    c->txCount++;
    c->frame = f;
    buf = c->macBuf;
    if (H(cfg, 0x10) == 0)
    {
        WSetPowerState(2);
    }
    CopyTxFrmToMacBuf(buf, (void *)cur);
    if (H(cfg, 0xc) == 1 && CAM_GetFrameCount(aid) > 1)
    {
        H(buf, 0xc) = H(buf, 0xc) | (0x2000);
    }
    fc = H(f, 0x14);
    u32 bufAdrs = (u32)buf;
    reg = (vu16 *)(0x048080a0 + pri * 4);
    ofs = (bufAdrs & 0x3fff) >> 1;
    if (4 == (fc & 0xc))
    {
        *reg = (u16)ofs | 0xa000;
    }
    else
    {
        if ((fc & 0xfc) == 0x50)
        {
            *reg = (u16)ofs | 0x9000;
        }
        else
        {
            *reg = (u16)ofs | 0x8000;
        }
    }
    __VENEER_OS_EnableIrqMask(x);
}
