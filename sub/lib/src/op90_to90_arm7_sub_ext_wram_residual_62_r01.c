#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#define ROOT (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))
typedef struct {
    u8 *elements;
    u16 x04;
    u16 x06;
    u16 length;
    u16 valid;
    u16 x0c;
    u16 x0e;
    u16 x10;
    u16 x12;
    u16 x14;
    u16 x16;
    u16 x18;
    u16 x1a;
    u8 *ssid;
    u8 *cf;
    u8 *tim;
    u8 *vendor;
} Elements;
extern u8 NULL_ADRS[];
extern int MatchMacAdrs(const void *, const void *);
extern void __VENEER_MIi_CpuClear16(u32, void *, u32);
extern void __VENEER_MIi_CpuClear32(u32, void *, u32);
extern void ElementChecker(Elements *);
extern u8 WL_ReadByte(const void *);
extern void WL_WriteByte(void *, u8);
extern void WSetMacAdrs1(void *, const void *);
extern void ClearTimeOut(void);
extern void AddTask(u32, u32);
void RxProbeResFrame(u8 *frame, Elements *p);
// @fn RxProbeResFrame
void RxProbeResFrame(u8 *frame, Elements *p)
{
    u8 *wl = ROOT;
    u8 *m = wl + 0x404;
    u8 *hdr;
    u8 *bss;
    u8 *src;
    u8 *dst;
    u32 i, k, n, len;
    u8 *mac;
    u32 id, l;
    Elements info;

    if (H(wl, 0x404) != 0x13)
        return;
    hdr = P(m, 0x1c);
    mac = P(m, 0x18) + 0x4e;
    for (i = 0; i < H(P(m, 0x18), 0x4c); i++) {
        if (MatchMacAdrs(mac, frame + 0x24)) {
            H(hdr, 6) |= 1 << i;
            return;
        }
        mac += 6;
    }
    bss = hdr + 0xa;
    for (i = 0; i < H(hdr, 8); i++) {
        if (MatchMacAdrs(frame + 0x24, bss + 4))
            return;
        bss += H(bss, 0) * 2;
    }
    __VENEER_MIi_CpuClear16(0, bss, 0x40);
    len = H(frame, 6);
    if (len <= 0xc)
        return;
    if (p == NULL) {
        __VENEER_MIi_CpuClear32(0, &info, sizeof(info));
        info.elements = frame + 0x38;
        info.length = len - 0xc;
        info.valid = 3;
        info.x0c = 0x38;
        info.x04 = H(frame, 8);
        info.x06 = H(frame, 0x36);
        ElementChecker(&info);
        p = &info;
    }
    if (p->vendor) {
        H(bss, 0x3c) = WL_ReadByte(p->vendor + 1) - 8;
        n = H(bss, 0x3c);
    } else {
        n = p->x1a;
    }
    H(bss, 0) = (n + 0x41) >> 1;
    if ((p->valid & 1) == 1 && H(ROOT, 0x408) >= H(bss, 0)) {
        H(bss, 0x2c) = H(frame, 0x36);
        WSetMacAdrs1(bss + 4, frame + 0x24);
        H(bss, 0x32) = H(frame, 0x34);
        H(bss, 2) = H(frame, 0x12) & 0xff;
        if (p->vendor) {
            for (i = 0; i < H(bss, 0x3c); i++)
                WL_WriteByte(bss + 0x40 + i, WL_ReadByte(p->vendor + 0xa + i));
        } else {
            H(bss, 0x3e) = p->x18;
            if (p->x18) {
                src = frame + 0x38;
                dst = bss + 0x40;
                k = 0;
                while (k < p->x18) {
                    id = WL_ReadByte(src);
                    l = WL_ReadByte(src + 1);
                    if (id > 6 && src != p->vendor) {
                        n = l + 2;
                        for (i = 0; i < n; i++) {
                            WL_WriteByte(dst, WL_ReadByte(src));
                            dst++;
                            src++;
                        }
                        k++;
                    } else {
                        src += l + 2;
                    }
                }
            }
        }
        if (p->ssid) {
            H(bss, 0xa) = WL_ReadByte(p->ssid + 1);
            for (i = 0; i < H(bss, 0xa); i++)
                WL_WriteByte(bss + 0xc + i, WL_ReadByte(p->ssid + 2 + i));
        } else {
            H(bss, 0xa) = 0;
            for (i = 0; i < 0x20; i++)
                WL_WriteByte(bss + 0xc + i, 0);
        }
        H(bss, 0x2e) = p->x14;
        H(bss, 0x30) = p->x16;
        H(bss, 0x36) = p->x12;
        if (p->cf)
            H(bss, 0x38) = WL_ReadByte(p->cf + 3);
        if (p->tim)
            H(bss, 0x34) = WL_ReadByte(p->tim + 3);
        H(hdr, 2) += H(bss, 0);
        H(hdr, 8)++;
        H(m, 4) -= H(bss, 0);
        if (H(m, 4) < 0x20) {
            ClearTimeOut();
            H(m, 0) = 0x15;
            AddTask(2, 0);
        }
    } else {
        WSetMacAdrs1(bss + 4, NULL_ADRS);
    }
}
