#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
u32 GetTexSRTAnmSinCosVal_(const u8 *base, u32 info, u32 value, u32 idx);
// @fn GetTexSRTAnmSinCosVal_
u32 GetTexSRTAnmSinCosVal_(const u8 *base, u32 info, u32 value, u32 idx)
{
    const u8 *data;
    u32 last, idx0, idx1;
    const s16 *p;
    if (info & 0x20000000) {
        return value;
    }
    data = base + value;
    if (info & 0xc0000000) {
        last = (u16)info;
        if (info & 0x40000000) {
            if (idx & 1) {
                if (idx > last) {
                    idx = (last >> 1) + 1;
                    goto direct;
                }
                idx0 = idx >> 1;
                goto half;
            }
            idx >>= 1;
            goto direct;
        }
        if (idx & 3) {
            if (idx > last) {
                idx = (idx & 3) + (last >> 2);
                goto direct;
            }
            if (idx & 1) {
                if (idx & 2) {
                    idx1 = idx >> 2;
                    idx0 = idx1 + 1;
                } else {
                    idx0 = idx >> 2;
                    idx1 = idx0 + 1;
                }
                {
                    const s16 *p = (const s16 *)data;
                    s32 a = p[idx0 * 2 + 1] * 3;
                    s32 hi = (a + p[idx1 * 2 + 1]) >> 2;
                    s32 b = p[idx0 * 2] * 3;
                    s32 lo = (b + p[idx1 * 2]) >> 2;
                    return (u16)lo | (hi << 16);
                }
            }
            idx0 = idx >> 2;
            goto half;
        }
        idx >>= 2;
    }
direct:
    return ((const u32 *)data)[idx];
half:
    p = (const s16 *)data;
    return (u16)((p[idx0 * 2] + p[idx0 * 2 + 2]) >> 1) | (((p[idx0 * 2 + 1] + p[idx0 * 2 + 3]) >> 1) << 16);
}
