#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
u16 GetMatColAnmValue_(const u8 *base, u32 info, u32 frame);
static inline u16 BlendColor(u32 a, u32 b, u32 weight, u32 shift)
{
    u32 g = ((a & 0x3e0) * weight + (b & 0x3e0)) >> shift;
    u32 rb = ((a & 0x7c1f) * weight + (b & 0x7c1f)) >> shift;
    return (u16)((rb & 0x7c1f) | (g & 0x3e0));
}
// @fn GetMatColAnmValue_
u16 GetMatColAnmValue_(const u8 *base, u32 info, u32 frame)
{
    const u16 *p;
    u32 last, idx, idx0, idx1;
    if (info & 0x20000000) {
        return (u16)info;
    }
    p = (const u16 *)(base + (u16)info);
    if (!(info & 0xc0000000)) {
        return p[frame];
    }
    last = (info & 0x1fff0000) >> 16;
    if (info & 0x40000000) {
        if (frame & 1) {
            if (frame > last) {
                return p[(last >> 1) + 1];
            }
            idx = frame >> 1;
            goto half;
        }
        return p[frame >> 1];
    }
    if (frame & 3) {
        if (frame > last) {
            return (p + (last >> 2))[frame & 3];
        }
        if (frame & 1) {
            if (frame & 2) {
                idx1 = frame >> 2;
                idx0 = idx1 + 1;
            } else {
                idx0 = frame >> 2;
                idx1 = idx0 + 1;
            }
            {
                u32 a = p[idx0];
                u32 b = p[idx1];
                u32 ag = (a & 0x3e0) * 3;
                u32 arb = (a & 0x7c1f) * 3;
                u32 g = (ag + (b & 0x3e0)) >> 2;
                u32 rb = (arb + (b & 0x7c1f)) >> 2;
                return (u16)((rb & 0x7c1f) | (g & 0x3e0));
            }
        }
        idx = frame >> 2;
        goto half;
    }
    return p[frame >> 2];
half:
    {
        u32 a = p[idx];
        u32 b = p[idx + 1];
        u32 g = ((a & 0x3e0) + (b & 0x3e0)) >> 1;
        u32 rb = ((a & 0x7c1f) + (b & 0x7c1f)) >> 1;
        return (u16)((rb & 0x7c1f) | (g & 0x3e0));
    }
}
