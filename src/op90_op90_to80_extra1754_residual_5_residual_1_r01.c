#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
extern const u8 _021094B8[];
BOOL getRotDataByIdx_(s32 *m, const s16 *pivotData, const s16 *basisData, u32 info);
#pragma push
#pragma opt_common_subs off
// @fn getRotDataByIdx_
BOOL getRotDataByIdx_(s32 *restrict m, const s16 *pivotData, const s16 *basisData, u32 info) {
    s32 mode;
    s16 c;
    if (info & 0x8000) {
        s32 pv;
        const s16 *p;
        u32 j;
        m[8] = 0;
        m[7] = 0;
        m[6] = 0;
        m[5] = 0;
        m[4] = 0;
        m[3] = 0;
        m[2] = 0;
        m[1] = 0;
        m[0] = 0;
        info &= 0x7fff;
        info += info << 1;
        p = pivotData + info;
        mode = p[0];
        s32 a = p[1];
        info = p[2];
        if (mode & 0x10) pv = -0x1000; else pv = 0x1000;
        mode &= 0xf;
        int i = _021094B8[mode * 4];
        m[mode] = pv;
        j = (_021094B8 + 1)[mode * 4];
        m[i] = a;
        m[j] = info;
        if (p[0] & 0x20) {
            info = -info;
        }
        m[(_021094B8 + 2)[mode * 4]] = info;
        if (p[0] & 0x40) {
            a = -a;
        }
        m[(_021094B8 + 3)[mode * 4]] = a;
        return FALSE;
    } else {
        s32 x0, x1, x2, x3, x4;
        const s16 *p;
        info &= 0x7fff;
        info += info << 2;
        p = basisData + info;
        x4 = p[4];
        m[4] = x4 >> 3;
        x0 = p[0];
        m[0] = x0 >> 3;
        c = (s16)(x4 & 7);
        c = ((x0 & 7) | (c << 3));
        x1 = p[1];
        m[1] = x1 >> 3;
        c = (s16)((x1 & 7) | (c << 3));
        x2 = p[2];
        m[2] = x2 >> 3;
        x3 = p[3];
        m[3] = x3 >> 3;
        c = (s16)((x2 & 7) | (c << 3));
        c = (s16)((x3 & 7) | (c << 3));
        m[5] = (s32)(((s32)c << 19)) >> 19;
        return TRUE;
    }
}
#pragma pop
