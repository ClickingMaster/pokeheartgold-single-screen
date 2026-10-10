#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
int NNS_G3dResDefaultSetup(void *);
int NNS_G3dScrPosToWorldLine(int, int, VecFx32 *, VecFx32 *);
fx64 FX_GetDivResultFx64c(void);
fx32 FX_Div(fx32, fx32);
void FX_InvAsync(fx32);
extern void NNS_G3dGlbGetViewPort(int *, int *, int *, int *);
extern const MtxFx44 *NNS_G3dGlbGetInvVP(void);

int NNS_G3dResDefaultSetup(void *file) {
    NNSG3dResTex *t;
    int a, b, c;
    u32 x, y, z, kx, ky, kz;
    switch (*(u32 *)file) {
    case 0x30415642:
    case 0x30414d42:
    case 0x30414342:
    case 0x30415442:
    case 0x30505442:
        goto ignored;
    case 0x30444d42:
    case 0x30585442: {
        a = 1;
        b = a;
        c = a;
        t = NNS_G3dGetTex(file);
        if (t) {
            x = NNS_G3dTexGetRequiredSize(t);
            y = NNS_G3dTex4x4GetRequiredSize(t);
            z = NNS_G3dPlttGetRequiredSize(t);
            if (x) {
                kx = NNS_GfdDefaultFuncAllocTexVram(x, 0, 0);
                if (!kx) {
                    a = 0;
                }
            } else {
                kx = 0;
            }
            if (y) {
                ky = NNS_GfdDefaultFuncAllocTexVram(y, 1, 0);
                if (!ky) {
                    b = 0;
                }
            } else {
                ky = 0;
            }
            if (z) {
                kz = NNS_GfdDefaultFuncAllocPlttVram(z, (*(u16 *)((u8 *)t + 0x20)) & 0x8000, 0);
                if (!kz) {
                    c = 0;
                }
            } else {
                kz = 0;
            }
            if (!a || !b || !c) {
                if (c) {
                    NNS_GfdDefaultFuncFreePlttVram(kz);
                }
                if (b) {
                    NNS_GfdDefaultFuncFreeTexVram(ky);
                }
                if (a) {
                    NNS_GfdDefaultFuncFreeTexVram(kx);
                }
                return 0;
            }
            NNS_G3dTexSetTexKey(t, kx, ky);
            NNS_G3dPlttSetPlttKey(t, kz);
            NNS_G3dTexLoad(t, 1);
            NNS_G3dPlttLoad(t, 1);
        }
        if (*(u32 *)file == 0x30444d42) {
            void *m = NNS_G3dGetMdlSet(file);
            if (t) {
                NNS_G3dBindMdlSet(m, t);
            }
        }
        return 1;
    }
    default:
        goto failure;
    }
ignored:
    return 1;
failure:
    return 0;
}
