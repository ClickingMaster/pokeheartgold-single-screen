#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u32 info; u32 offset; } SclTrack;

void getScaleData_(s32 *out, s32 frame, const SclTrack *trk, const u8 *base);
#pragma opt_propagation off
// @fn getScaleData_
void getScaleData_(s32 *out, s32 frame, const SclTrack *trk, const u8 *base)
{
    u32 info;
    u32 idx = (u32)(frame >> 12);
    const u8 * data = base + trk->offset;
    u32 last, idx0, idx1, r;
    info = trk->info;
    if (info & 0xc0000000)
    {
        last = (info & 0x1fff0000) >> 16;
        if (info & 0x40000000)
        {
            if (idx & 1)
            {
                if (idx > last)
                {
                    idx = (last >> 1) + 1;
                    goto direct;
                }
                idx0 = idx >> 1;
                goto half;
            }
            idx >>= 1;
            goto direct;
        }
        r = idx & 3;
        if (r)
        {
            if (idx > last)
            {
                idx = (last >> 2) + r;
                goto direct;
            }
            if (idx & 1)
            {
                if (idx & 2)
                {
                    idx1 = idx >> 2;
                    idx0 = idx1 + 1;
                }
                else
                {
                    idx0 = idx >> 2;
                    idx1 = idx0 + 1;
                }
                if (info & 0x20000000)
                {
                    const s16 * p = (const s16 *)data;
                    {
                        s32 a = p[idx0 * 2];
                        s32 b = p[idx1 * 2];
                        a = a * 3 + b;
                        out[0] = a >> 2;
                    }
                    {
                        s32 a = p[idx0 * 2 + 1];
                        s32 b = p[idx1 * 2 + 1];
                        a = a * 3 + b;
                        out[1] = a >> 2;
                    }
                }
                else
                {
                    const s32 * p = (const s32 *)data;
                    {
                        s32 b = p[idx1 * 2];
                        s32 a = p[idx0 * 2];
                        out[0] = (s32)(((s64)a * 3 - -(s64)b) >> 2);
                    }
                    {
                        s32 a = p[idx0 * 2 + 1];
                        out[1] = (s32)(((s64)a * 3 + p[idx1 * 2 + 1]) >> 2);
                    }
                }
                return;
            }
            idx0 = idx >> 2;
            goto half;
        }
        idx >>= 2;
    }
direct:
    if (info & 0x20000000)
    {
        const s16 * p = (const s16 *)data;
        out[0] = p[idx * 2];
        out[1] = p[idx * 2 + 1];
    }
    else
    {
        const s32 * p = (const s32 *)data;
        out[0] = p[idx * 2];
        out[1] = p[idx * 2 + 1];
    }
    return;
half:
    if (info & 0x20000000)
    {
        const s16 * p = (const s16 *)data;
        out[0] = (p[idx0 * 2] + p[idx0 * 2 + 2]) >> 1;
        out[1] = (p[idx0 * 2 + 1] + p[idx0 * 2 + 3]) >> 1;
    }
    else
    {
        const s32 * p = (const s32 *)data;
        out[0] = (p[idx0 * 2] + p[idx0 * 2 + 2]) >> 1;
        out[1] = (p[idx0 * 2 + 1] + p[idx0 * 2 + 3]) >> 1;
    }
    return;
}


#pragma opt_propagation reset
