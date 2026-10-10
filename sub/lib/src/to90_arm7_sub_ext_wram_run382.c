#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define G       (*(u8 *volatile *)0x0380fff4)
#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define P(p, n) (*(u8 **)((u8 *)(p) + (n)))
u32 WL_ReadByte(const void *);
int WCheckSSID(u16, const void *);
void WElement2RateSet(const void *, u16 *);
void ElementChecker(void *);

void ElementChecker(void *packet) {
    u8 *s = G + 0x344, *p = P(packet, 0);
    int remaining;
    u32 len, id;
    H(packet, 18) = H(s, 0x7a);
    if (H(packet, 12) & 0x800) {
        H(packet, 10) |= 1;
    }
    remaining = H(packet, 8);
    while (remaining > 0) {
        id = WL_ReadByte(p);
        {
            u8 *l = p + 1;
            p += 2;
            len = WL_ReadByte(l);
        }
        switch (id) {
        case 0:
            if (len <= 32) {
                H(packet, 12) |= 1;
                P(packet, 28) = p - 2;
                if (len == 0 && (H(packet, 12) & 0x800)) {
                    H(packet, 10) |= 1;
                } else {
                    H(packet, 10) &= ~1u;
                    if (WCheckSSID((u16)len, p)) {
                        H(packet, 10) |= 1;
                    }
                }
            }
            break;
        case 1:
            if (len >= 1) {
                u32 basic, supported, actual;
                H(packet, 12) |= 4;
                WElement2RateSet(p - 2, (u16 *)((u8 *)packet + 20));
                basic = H(s, 0x60);
                supported = H(s, 0x62);
                actual = H(packet, 20);
                if (!(actual & ~(basic | supported)) && (((actual | H(packet, 22)) & basic) == basic)) {
                    H(packet, 10) |= 4;
                } else {
                    H(packet, 10) &= ~4u;
                }
            }
            break;
        case 2:
        case 6:
            break;
        case 3:
            if (len >= 1) {
                H(packet, 12) |= 2;
                H(packet, 18) = WL_ReadByte(p);
                if (H(packet, 18) == H(P(G, 0x41c), 0x4a)) {
                    H(packet, 10) |= 2;
                } else {
                    H(packet, 10) &= ~2u;
                }
            }
            break;
        case 5:
            if (len >= 3) {
                H(packet, 12) |= 0x100;
                P(packet, 36) = p - 2;
            }
            break;
        case 4:
            if (len >= 6) {
                H(packet, 12) |= 0x200;
                P(packet, 32) = p - 2;
            }
            break;
        case 221:
            if (len < 8 || WL_ReadByte(p) != 0 || WL_ReadByte(p + 1) != 9 || WL_ReadByte(p + 2) != 0xbf || WL_ReadByte(p + 3) != 0) {
                H(packet, 24)
                ++;
                H(packet, 26) += len + 2;
            } else {
                H(packet, 12) |= 0x400;
                P(packet, 40) = p - 2;
            }
            break;
        default:
            H(packet, 24)
            ++;
            H(packet, 26) += len + 2;
            break;
        }
        p += len;
        remaining -= len + 2;
    }
    if ((H(packet, 12) & 8) && ((H(s, 0x64) & 1) || (H(packet, 4) & 0x8000))) {
        H(packet, 10) |= 8;
    }
    if (H(packet, 12) & 0x30) {
        if ((H(packet, 6) & 3) == (H(s, 0x7c) & 3)) {
            H(packet, 10) |= 16;
        }
        if ((H(packet, 6) & 16) == (H(s, 0x7c) & 16)) {
            H(packet, 10) |= 32;
        }
    }
}
