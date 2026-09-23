#include "global.h"

typedef struct { u32 a, b, c; } BlendConfig;
extern const BlendConfig ov07_02236858;
extern void sub_02015510(BlendConfig *);
extern int ov07_0221FAF8(void *, int);

void ov07_02231DE8(int kind, BlendConfig *out);
void ov07_02231E08(void *arg, int eva, int evb);

void ov07_02231DE8(int kind, BlendConfig *out) {
    if (kind == 0) {
        sub_02015510(out);
    } else {
        *out = ov07_02236858;
    }
}

void ov07_02231E08(void *arg, int eva, int evb) {
    if (eva == -1) eva = 26;
    if (evb == -1) evb = 5;
    int mask = (1 << ov07_0221FAF8(arg, 2)) | 1;
    G2x_SetBlendAlpha_(0x04000050, 0, mask, eva, evb);
}
