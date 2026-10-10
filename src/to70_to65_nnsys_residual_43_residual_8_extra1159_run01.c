#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "to91_tex_callback_41566_private.h"
#pragma force_active on

void NNSi_G3dSendTexSRTXsi(void *w) {
    struct {
        u32 cmd, mode;
        MtxFx44 m;
        u32 restore;
    } c;

    if (To91TexWord41566(w, 0) & 8) {
        c.cmd = 0x101610;
    } else {
        c.cmd = 0x101810;
    }
    c.mode = 3;
    c.restore = 2;
    c.m._33 = 4096;
    c.m._32 = 0;
    c.m._23 = 0;
    c.m._22 = 0;
    c.m._21 = 0;
    c.m._20 = 0;
    c.m._13 = 0;
    c.m._12 = 0;
    c.m._03 = 0;
    c.m._02 = 0;
    if (To91TexWord41566(w, 0) & 1) {
        To91TexStoreWord41566(w, 28, 4096);
        To91TexStoreWord41566(w, 24, 4096);
    }
    if (To91TexWord41566(w, 0) & 2) {
        To91TexStoreHalf41566(w, 34, 4096);
        To91TexStoreHalf41566(w, 32, 0);
    }
    if (To91TexWord41566(w, 0) & 4) {
        To91TexStoreWord41566(w, 40, 0);
        To91TexStoreWord41566(w, 36, 0);
    }
    _02110C5C[To91TexWord41566(w, 0) & 7](&c.m, w);
    if (To91TexSignedWord41566(w, 48) != 4096) {
        c.m._00 = (s64)To91TexSignedWord41566(w, 48) * c.m._00 >> 12;
        c.m._01 = (s64)To91TexSignedWord41566(w, 48) * c.m._01 >> 12;
        c.m._30 = (s64)To91TexSignedWord41566(w, 48) * c.m._30 >> 12;
    }
    if (To91TexSignedWord41566(w, 52) != 4096) {
        c.m._10 = (s64)To91TexSignedWord41566(w, 52) * c.m._10 >> 12;
        c.m._11 = (s64)To91TexSignedWord41566(w, 52) * c.m._11 >> 12;
        c.m._31 = (s64)To91TexSignedWord41566(w, 52) * c.m._31 >> 12;
    }
    NNS_G3dGeBufferOP_N(c.cmd, ((u32 *)&c) + 1, 18);
}
