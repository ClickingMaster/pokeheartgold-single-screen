#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define BASE    (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))

typedef struct {
    u16 hidden : 1;
    u16 rest : 15;
} To90W37029Flags;

extern u16 BC_ADRS[];
extern u32 WCalcManRate(void);
extern u32 SetSupRateSet(void *);
extern void WSetMacAdrs3(void *, const void *, const void *, const void *);
extern void WL_WriteByte(void *, u32);
extern u32 WL_ReadByte(void *);
void MakeBeaconFrame(void);

void MakeBeaconFrame(void) {
    u8 *source, *cfg, *ctl;
    u32 i;
    u8 *frame, *pos, *body;
    u8 *base = BASE;
    frame = *(u8 **)(base + 0x4ac);
    cfg = base + 0x31c;
    ctl = base + 0x344;
    H(frame, 0) = 0;
    H(frame, 2) = 0;
    H(frame, 4) = 0;
    H(frame, 6) = 0;
    H(frame, 8) = WCalcManRate();
    H(frame, 0xc) = 0x80;
    H(frame, 0xe) = 0;
    WSetMacAdrs3(frame + 0x10, BC_ADRS, cfg + 8, cfg + 8);
    H(frame, 0x22) = 0;
    body = frame + 0x24;
    W(body, 0) = 0;
    W(body, 4) = 0;
    H(body, 8) = H(ctl, 0x6e);
    H(body, 10) = H(ctl, 0x7c);
    pos = body + 12;
    if (!((To90W37029Flags *)(cfg + 0x1e))->hidden) {
        H(ctl, 0x92) = (u16)(pos - body);
        WL_WriteByte(pos, 0);
        {
            u8 *next = pos + 1;
            u32 length = H(ctl, 0x1e) & 255;
            pos += 2;
            WL_WriteByte(next, length);
        }
        for (i = 0; i < H(ctl, 0x1e); i++) {
            WL_WriteByte(pos, WL_ReadByte(ctl + 0x20 + i));
            pos++;
        }
        {
            u32 backwards = 0xffffffffU - i;
            WL_WriteByte((void *)((u32)pos + backwards), i & 255);
        }
    } else {
        H(ctl, 0x92) = 0;
    }
    pos += SetSupRateSet(pos);
    WL_WriteByte(pos, 3);
    WL_WriteByte(pos + 1, 1);
    WL_WriteByte(pos + 2, H(ctl, 0x7a) & 255);
    H(ctl, 0x94) = (u16)(pos + 3 - body);
    *(volatile u16 *)0x4808084 = H(ctl, 0x94) + 2;
    WL_WriteByte(pos + 3, 5);
    WL_WriteByte(pos + 4, 5);
    WL_WriteByte(pos + 5, 0);
    WL_WriteByte(pos + 6, H(ctl, 0x74) & 255);
    WL_WriteByte(pos + 7, 0);
    WL_WriteByte(pos + 8, 0);
    WL_WriteByte(pos + 9, 0);
    H(ctl, 0x96) = (u16)(pos + 10 - body);
    H(ctl, 0xa2) = H(ctl, 0x96) & 1;
    WL_WriteByte(pos + 10, 0xdd);
    WL_WriteByte(pos + 11, (H(ctl, 0xa0) + 8) & 255);
    WL_WriteByte(pos + 12, 0);
    WL_WriteByte(pos + 13, 9);
    WL_WriteByte(pos + 14, 0xbf);
    WL_WriteByte(pos + 15, 0);
    if (H(ctl, 0xe) == 1) {
        WL_WriteByte(pos + 16, H(cfg, 0x20) & 255);
        {
            u8 *next = pos + 17;
            u32 value = (s32)H(cfg, 0x20) >> 8;
            pos += 18;
            WL_WriteByte(next, value & 255);
        }
    } else {
        WL_WriteByte(pos + 16, 255);
        {
            u8 *next = pos + 17;
            pos += 18;
            WL_WriteByte(next, 255);
        }
    }
    {
        u32 value = *(u16 *)0x380fff0;
        WL_WriteByte(pos, value & 255);
        {
            u8 *next = pos + 1;
            pos += 2;
            WL_WriteByte(next, (value >> 8) & 255);
        }
    }
    source = *(u8 **)(ctl + 0x9c);
    for (i = 0; i < H(ctl, 0xa0); i++) {
        WL_WriteByte(pos, WL_ReadByte(source));
        pos++;
        source++;
    }
    if (H(ctl, 0xa2)) {
        u32 length = H(ctl, 0xa0);
        source = (u8 *)((u32) * (u8 **)(ctl + 0x9c) + length - 1U);
        i = 0;
        if (i < length) {
            for (; i < H(ctl, 0xa0);) {
                WL_WriteByte(source + 1, WL_ReadByte(source));
                i++;
                source--;
            }
        }
    }
    if (H(BASE, 0x690) & 4) {
        u32 aligned = ((u32)pos + 3) & ~3U;
        H((void *)aligned, 0) = 0xb6b8;
        H((void *)aligned, 2) = 0x1d46;
    }
    H(ctl, 0xa4) = 0;
    H(frame, 0xa) = (u16)(pos + 0x1c - body);
}
