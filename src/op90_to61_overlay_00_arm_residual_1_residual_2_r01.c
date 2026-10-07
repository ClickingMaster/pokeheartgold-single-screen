#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

void *ov00_021EC2B8(void *ctx, u32 size, int flags);
u32 ov00_021EC284(const void *src);
void ov00_021EC2EC(void *ctx, const void *src, u32 len);
void *ov00_021EC33C(void *ctx, const void *src, u32 oldSize, u32 size, int flags);

// @fn ov00_021EC33C
void *ov00_021EC33C(void *ctx, const void *src, u32 oldSize, u32 size, int flags) {
    void *buf = ov00_021EC2B8(ctx, size, flags);
    if (buf == NULL) {
        return NULL;
    }
    if (src != NULL) {
        oldSize = ov00_021EC284(src);
        if (oldSize <= size) {
            size = oldSize;
        }
        MI_CpuCopy8(src, buf, size);
        ov00_021EC2EC(ctx, src, oldSize);
    }
    return buf;
}
