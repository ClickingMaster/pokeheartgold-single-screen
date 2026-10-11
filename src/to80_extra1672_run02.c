#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>

#include "renderer_joint_family_private.h"

void NNSi_G3dGetJointScaleMaya(MdlJointResult *result, const s32 *scale, const u8 *node, u32 flags) {
    u8 kind = node[3];
    if (flags & 4) {
        result->flags |= 1;
        if (kind & 2) {
            u32 index = node[1];
            u32 *bits = gCurrentRenderState->scaleFlags;
            bits[index >> 5] |= 1u << (index & 31);
        }
    } else {
        result->scale[0] = scale[0];
        result->scale[1] = scale[1];
        result->scale[2] = scale[2];
        if (kind & 2) {
            u32 index = node[1];
            u32 *bits = gCurrentRenderState->scaleFlags;
            int offset = (s16)index * (s16)24;
            bits[index >> 5] &= ~(1u << (index & 31));
            *(s32 *)((u8 *)_021DB510 + offset) = scale[3];
            *(s32 *)((u8 *)_021DB514 + offset) = scale[4];
            *(s32 *)((u8 *)_021DB518 + offset) = scale[5];
        }
    }
    if (kind & 1) {
        u32 index = node[2];
        result->flags |= 32;
        if ((gCurrentRenderState->scaleFlags)[index >> 5] & (1u << (index & 31))) {
            result->flags |= 8;
        } else {
            result->inverseTriples[0] = _021DB510[index * 2];
        }
    }
    result->flags |= 16;
}
