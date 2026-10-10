#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern fx64c FX_InvFx64c(fx32);
int mtx_inverse44(const MtxFx44 *, MtxFx44 *);

static inline void SwapRows(fx32 *a, fx32 *b, fx32 *oa, fx32 *ob) {
    int k;
    for (k = 0; k < 4; k++) {
        fx32 t = a[k];
        a[k] = b[k];
        b[k] = t;
        t = oa[k];
        oa[k] = ob[k];
        ob[k] = t;
    }
}

static inline void NormalizeRows(fx32 *a, fx32 *b, fx64c inverse) {
    int k;
    for (k = 0; k < 4; k++) {
        a[k] = (s32)(((u64)inverse * (u64)(s64)a[k] + 0x80000000ULL) >> 32);
        b[k] = (s32)(((u64)inverse * (u64)(s64)b[k] + 0x80000000ULL) >> 32);
    }
}

int mtx_inverse44(const MtxFx44 *source, MtxFx44 *output);

int mtx_inverse44(const MtxFx44 *source, MtxFx44 *output) {
    MtxFx44 copy;
    int i;
    int j;
    int best;
    fx32 maximum;
    fx32 x;
    fx64 f;
    int r;
    int k;
    MI_Copy64B(source, &copy);
    MTX_Identity44_(output);
    for (i = 0; i < 4; i++) {
        best = i;
        maximum = 0;
        fx64c inverse;
        for (j = i; j < 4; j++) {
            x = copy.m[j][i];
            if (x < 0) {
                x = (s32)(0u - (u32)x);
            }
            if (x > maximum) {
                maximum = x;
                best = j;
            }
        }
        if (maximum == 0) {
            return -1;
        }
        if (best != i) {
            SwapRows(copy.m[i], copy.m[best], output->m[i], output->m[best]);
        }
        inverse = FX_InvFx64c(copy.m[i][i]);
        NormalizeRows(copy.m[i], output->m[i], inverse);
        for (r = 0; r < 4; r++) {
            if (r != i) {
                f = copy.m[r][i];
                for (k = 0; k < 4; k++) {
                    copy.m[r][k] = (s32)((u32)copy.m[r][k] - (u32)(f * copy.m[i][k] >> 12));
                    output->m[r][k] = (s32)((u32)output->m[r][k] - (u32)(f * output->m[i][k] >> 12));
                }
            }
        }
    }
    return 0;
}
