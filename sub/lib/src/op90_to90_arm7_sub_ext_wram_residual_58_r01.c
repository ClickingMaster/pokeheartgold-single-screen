#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#define ROOT (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
void *AllocateHeapBuf(void *, u32);
void SetFatalErr(u32);
void InitManHeader(void *, const void *);
u32 SetSupRateSet(void *);
u32 SetSSIDElement(void *);
u8 WL_ReadByte(const void *);
void WL_WriteByte(void *, u8);
int IsEnableManagement(void);
u8 *MakeProbeResFrame(const void *addr);
static inline u32 SetGameInfoElement(u8 *e)
{
    u8 *sta;
    u8 *cfg;
    u32 v;
    u32 i;
    u32 j;
    u8 *src;
    sta = ROOT + 0x344;
    cfg = ROOT + 0x31c;
    i = 0;
    WL_WriteByte(e, 0xdd);
    WL_WriteByte(e + 1, H(ROOT, 0x3e4) + 8);
    WL_WriteByte(e + 2, 0);
    WL_WriteByte(e + 3, 9);
    WL_WriteByte(e + 4, 0xbf);
    WL_WriteByte(e + 5, 0);
    WL_WriteByte(e + 6, H(cfg, 0x20));
    WL_WriteByte(e + 7, H(cfg, 0x20) >> 8);
    v = *(u16 *)0x0380fff0;
    WL_WriteByte(e + 8, v);
    WL_WriteByte(e + 9, v >> 8);
    i += 10;
    if (H(sta, 0xa0)) {
        src = P(sta, 0x9c);
        if (H(sta, 0xa2) & 1)
            src++;
        for (j = 0; j < H(sta, 0xa0); j++) {
            WL_WriteByte(e + i, WL_ReadByte(src));
            i++;
            src++;
        }
    }
    return i;
}
// @fn MakeProbeResFrame
u8 *MakeProbeResFrame(const void *addr)
{
    u8 *sta = ROOT + 0x344;
    u8 *buf;
    u8 *e;
    u32 len;

    if (!IsEnableManagement())
        return NULL;
    buf = AllocateHeapBuf(ROOT + 0x188, H(sta, 0xa0) + 0x78);
    if (!buf) {
        SetFatalErr(2);
        return buf;
    }
    H(buf, 0xc) = 0xffff;
    InitManHeader(buf + 0x10, addr);
    H(buf, 0x44) = H(sta, 0x6e);
    H(buf, 0x46) = H(sta, 0x7c);
    len = SetSSIDElement(buf + 0x48);
    len += SetSupRateSet(buf + 0x48 + len);
    e = buf + 0x48 + len;
    WL_WriteByte(e, 3);
    WL_WriteByte(e + 1, 1);
    WL_WriteByte(e + 2, H(ROOT, 0x3be));
    len += 3;
    len += SetGameInfoElement(buf + 0x48 + len);
    H(buf, 0x16) = len + 0xc;
    H(buf, 0x22) = H(buf, 0x16) + 0x1c;
    H(buf, 0x24) = 0x50;
    return buf + 0x10;
}
