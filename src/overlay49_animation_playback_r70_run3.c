#include "overlay49_animation_playback_r70_private.h"

void ov49_02265BE8(void *ctx, Ov49AnimationState *state, u32 index, u32 animation, s32 limit) {
    const u8 *desc = state->descs[index];
    s32 duration = ModelAnimation_GetFrameCount((u8 *)ctx + 0x106DC + desc[2 + animation] * 0x14);

    if (duration < limit) {
        state->frames[index][animation] = duration - 0x800;
    } else {
        state->frames[index][animation] = limit;
    }
}

s32 ov49_02265C40(void *ctx, Ov49AnimationState *state, u32 index, u32 animation) {
    const u8 *desc = state->descs[index];
    return ModelAnimation_GetFrame((u8 *)ctx + 0x106DC + desc[2 + animation] * 0x14);
}
