#include "to50_palette_fade_12090_private.h"

void sub_02012090(EffectWork12090 *work, const EffectConfig12090 *config, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 heapId) {
    int i;
    s16 *data;

    work->f[3] = (u32)Heap_Alloc((enum HeapID)heapId, config->count * 0x30);
    if (work->f[3] == 0) {
        GF_AssertFail();
    }
    work->f[4] = config->count;
    i = 0;
    if (i < config->count) {
        u32 srcOffset = i;
        u32 dstOffset = srcOffset;
        do {
            void *entry = (u8 *)work->f[3] + dstOffset;
            sub_02010AB0(entry, (u8 *)entry + 0x20, (u8 *)entry + 0x10, (const u8 *)config->src0 + srcOffset, (const u8 *)config->src1 + srcOffset, arg2);
            srcOffset += 4;
            dstOffset += 0x30;
            i++;
        } while (i < config->count);
    }
    ScanlineBufferController_Init(work, config->mode, arg4, heapId);
    work->f[5] = arg2;
    work->f[6] = arg3;
    work->f[7] = 0;
    work->f[9] = (u32)arg5;
    work->f[10] = (u32)arg6;
    work->f[11] = heapId;
    work->f[8] = config->value;
    sub_02012204(work);
    SysTask_CreateOnVWaitQueue(sub_02010F00, work, 0x3FF);
    data = sub_02010EE0(work, 0);
    sub_02010F84((void *)arg5, config->a, config->b, config->mode, arg4, data[0x180], 0, data[0x240], 0xC0, work->f[8]);
    if (config->mode == 0) {
        sub_02011068((void *)work->f[9], 1, arg4, work->f[8]);
    } else {
        sub_02011068((void *)work->f[9], 2, arg4, work->f[8]);
    }
    sub_0200FF88((void *)work->f[10], work, ScanlineWindow_HBlankCallback, arg4, heapId);
}
