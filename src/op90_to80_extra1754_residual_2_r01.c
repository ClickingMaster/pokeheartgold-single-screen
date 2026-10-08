#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u32 info; u32 offset; } TransTrack;
void getTransData_(s32 *out, s32 frame, const TransTrack *trk, const u8 *base);
// @fn getTransData_
void getTransData_(s32 *out, s32 frame, const TransTrack *trk, const u8 *base)
{
    u32 info;
    u32 last, idx0, idx1, r;
    u32 idx = (u32)(frame >> 12);
    const u8 * data = base + trk->offset;
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
                    {s32 a=p[idx0];*out=(a*3+p[idx1])>>2;}
                }
                else
                {
                    const s32 * p = (const s32 *)data;
                    {s32 a=p[idx0];*out=(s32)(((s64)a*3+p[idx1])>>2);}
                }
                return;
            }
            idx0 = idx >> 2;
        }
        else
        {
            idx >>= 2;
            goto direct;
        }
half:
        if (info & 0x20000000)
        {
            const s16 * p = (const s16 *)data;
            *out = (p[idx0] + p[idx0 + 1]) >> 1;
        }
        else
        {
            const s32 * p = (const s32 *)data;
            *out = (p[idx0] >> 1) + (p[idx0 + 1] >> 1);
        }
        return;
    }
direct:
    if (info & 0x20000000)
    {
        *out = ((const s16 *)data)[idx];
    }
    else
    {
        *out = ((const s32 *)data)[idx];
    }
}
