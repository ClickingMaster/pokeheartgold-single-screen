#include "to50_overlay73_resource_copy_private.h"

void RecordMixing_BuildSendBuffer(void *ctx, void *dest) {
    u32 remaining = 3000;
    int i;

    for (i = 0; i < 10; i++) {
        u32 size = ov73_021EA744[i].getSize(ctx);
        void *buffer = ov73_021EA744[i].load(ctx, 50, size);
        GF_ASSERT(remaining > size);
        MIi_CpuCopyFast(buffer, dest, size);
        Heap_Free(buffer);
        remaining -= size;
        dest = (u8 *)dest + size;
    }
}
