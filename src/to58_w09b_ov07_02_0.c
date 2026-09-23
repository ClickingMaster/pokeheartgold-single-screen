#include "global.h"

extern int ov07_0221FAF8(void *, int);
void ov07_02231E74(void *, int, int);
void ov07_02231EC0(void *, int, int);

void ov07_02231E74(void *arg, int eva, int evb) {
    if (eva == -1) eva = 26;
    if (evb == -1) evb = 5;
    int mask = (1 << ov07_0221FAF8(arg, 2)) | 0x11;
    int mask2 = 1 << ov07_0221FAF8(arg, 1);
    G2x_SetBlendAlpha_(0x04000050, mask2, mask, eva, evb);
}
