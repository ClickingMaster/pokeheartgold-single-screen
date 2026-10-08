#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#define ROOT (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
typedef struct {
    u16 frag : 4;
    u16 seq : 12;
} SeqCtrl;
#define SC(p, o) (*(SeqCtrl *)((u8 *)(p) + (o)))
typedef struct {
    u8 a1[6];
    u8 a2[6];
    SeqCtrl sc;
    u16 pad;
} DefragKey;
typedef struct {
    u16 state;
    u16 fragLen;
    u8 a1[6];
    u8 a2[6];
    SeqCtrl sc;
    u16 x12;
    u8 *buf;
} Entry;
extern int MatchMacAdrs(const void *, const void *);
extern void WSetMacAdrs1(void *, const void *);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
extern void *AllocateHeapBuf(void *, u32);
extern void SetFatalErr(u32);
extern void __VENEER_MI_WaitDma(u32);
extern void ReleaseHeapBuf(void *, void *);
extern void MoveHeapBuf(void *, void *, void *);
extern void AddTask(u32, u32);
extern u32 __VENEER__u32_div_f(u32, u32);
void DefragTask(void);
// @fn DefragTask
void DefragTask(void)
{

    Entry *e;
    u8 *p;
    u8 *rx;
    u32 n2;
    int free;
    int d;
    Entry *e2;
    u8 *wl;
    u32 total;
    u8 *frm;
    u32 i;
    u8 *buf;
    u16 fc;
    int len2;
    Entry *tbl;
    int k;
    int len;
    u32 n;
    u32 off;
    u32 i2;
    Entry *tbl2;

    DefragKey key;

    rx = ROOT + 0x17c;
    frm = P(rx, 0x6c);

    if (frm == (u8 *)0xffffffff)
        return;
    if (H(ROOT, 0x34c) != 0x40 || H(frm, 0x20) > 0x5fc)
        goto done;
    fc = H(frm, 0x24);
    if (fc & 0x100) {
        WSetMacAdrs1(key.a1, frm + 0x34);
        if (fc & 0x200)
            goto done;
        WSetMacAdrs1(key.a2, frm + 0x2e);
    } else {
        WSetMacAdrs1(key.a1, frm + 0x28);
        if (fc & 0x200)
            WSetMacAdrs1(key.a2, frm + 0x34);
        else
            WSetMacAdrs1(key.a2, frm + 0x2e);
    }
    *(u16 *)&key.sc = H(frm, 0x3a);
    if ((fc & 0x400) && SC(frm, 0x3a).frag == 0) {
        free = -1;
        tbl = (Entry *)(ROOT + 0x4e4);
        for (i = 0; i < 3; i++) {
            e = &tbl[i];
            if (tbl[i].state != 0) {
                if (!MatchMacAdrs(e->a1, key.a1))
                    continue;
                if (!MatchMacAdrs(e->a2, key.a2))
                    continue;
                if (key.sc.seq != e->sc.seq)
                    continue;
                n = (int)(H(frm, 0x18) & 0xf0) / 16;
                d = n - e->sc.frag;
                if (d == 0 || (d & 0x80000000))
                    goto done;
                buf = tbl[i].buf;
                len = H(frm, 0x20) - H(buf, 0x20) - 0x18;
                if (len == 0 || (len & 0x80000000))
                    goto done;
                __VENEER_MIi_CpuCopy16(frm + 0x3c + H(buf, 0x20), buf + 0x3c + H(buf, 0x20), len);
                H(buf, 0x20) += len;
                e->sc.frag = n;
                W(ROOT, 0x560) += n;
    goto done;
            } else {
                free = i;
            }
        }
        if (free == -1)
            goto done;
        buf = AllocateHeapBuf(ROOT + 0x188, 0x622);
        if (buf) {
            e = &tbl[free];
            __VENEER_MIi_CpuCopy16(&key, e->a1, 0x10);
            tbl[free].state = 5;
            tbl[free].buf = buf;
            __VENEER_MIi_CpuCopy16(frm + 0x18, buf + 0x18, H(frm, 0x20) + 0xc);
            __VENEER_MI_WaitDma(W(ROOT, 0x30c));
            n = (int)(H(buf, 0x18) & 0xf0) / 16;
            e->sc.frag = n;
            W(ROOT, 0x560) += n;
            H(buf, 0x20) = H(frm, 0x20) - 0x18;
            tbl[free].fragLen = __VENEER__u32_div_f(H(buf, 0x20), n);
        } else {
            SetFatalErr(4);
        }
    } else {
        wl = ROOT;
        H(frm, 0x20) -= 0x18;
        tbl2 = (Entry *)(wl + 0x4e4);
        for (i2 = 0; i2 < 3; i2++) {
            e2 = &tbl2[i2];
            if (tbl2[i2].state == 0)
                continue;
            if (!MatchMacAdrs(e2->a1, key.a1))
                continue;
            if (!MatchMacAdrs(e2->a2, key.a2))
                continue;
            if (key.sc.seq != e2->sc.seq)
                continue;
            k = *(u16 *)((u8 *)tbl2 + i2 * 0x18 + 0x10) - *(u16 *)&key.sc;
            if (k & 0x80000000)
                goto done;
            off = k * tbl2[i2].fragLen;
            len2 = H(frm, 0x20) - off;
            if (len2 == 0)
                goto done;
            if (!(len2 & 0x80000000))
                break;
            goto done;
        }
        if (i2 == 3)
            goto done;
        p = tbl2[i2].buf + 0x10;
        total = H(p, 0x10) + len2;
        if (total > 0x5e4) {
            ReleaseHeapBuf(wl + 0x188, p - 0x10);
            tbl2[i2].state = 0;
            goto done;
        }
        __VENEER_MIi_CpuCopy16(frm + 0x3c + off, p + 0x2c + H(p, 0x10), len2 + 1);
        H(p, 0x10) = total;
        n2 = (int)(H(frm, 0x18) & 0xf0) / 16;
        tbl2[i2].sc.frag += n2 - k;
        W(ROOT, 0x560) += n2;
        if (H(frm, 0x18) & 0x100)
            goto done;
        tbl2[i2].state = 0;
        H(p, 8) = (H(p, 8) & ~0xf0) + 0x10;
        H(p, 0x10) += 0x18;
        switch (H(p, 8) & 0xf) {
        case 8:
            MoveHeapBuf(wl + 0x188, wl + 0x1c4, p - 0x10);
            AddTask(2, 6);
            break;
        case 0:
            MoveHeapBuf(wl + 0x188, wl + 0x1dc, p - 0x10);
            AddTask(1, 7);
            break;
        default:
            ReleaseHeapBuf(wl + 0x188, p - 0x10);
            break;
        }
    }
done:
    ReleaseHeapBuf(rx + 0x6c, frm);
    if (H(rx, 0x74))
        AddTask(2, 9);
}
