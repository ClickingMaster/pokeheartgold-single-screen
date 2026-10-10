#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 flags;
    u32 scale[3];
    u32 parentScale[3];
    s32 pad[3];
    u32 matrix[9];
    u32 translation[3];
} Joint;

void NNSi_G3dSendJointSRTMaya(const Joint *);

void NNSi_G3dSendJointSRTMaya(const Joint *p) {
    BOOL translate = FALSE;
    if (!(p->flags & 4)) {
        translate = TRUE;
    }
    if ((p->flags & 32) && !(p->flags & 8)) {
        if (translate) {
            NNS_G3dGeBufferOP_N(28, p->translation, 3);
            translate = FALSE;
        }
        NNS_G3dGeBufferOP_N(27, p->parentScale, 3);
    }
    if (!(p->flags & 2)) {
        if (translate) {
            NNS_G3dGeBufferOP_N(25, p->matrix, 12);
        } else {
            NNS_G3dGeBufferOP_N(26, p->matrix, 9);
        }
    } else if (translate) {
        NNS_G3dGeBufferOP_N(28, p->translation, 3);
    }
    if (!(p->flags & 1)) {
        NNS_G3dGeBufferOP_N(27, p->scale, 3);
    }
}
