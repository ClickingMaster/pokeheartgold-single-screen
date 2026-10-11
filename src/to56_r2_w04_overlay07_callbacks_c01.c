#include "to56_r2_w04_overlay07_callbacks_private.h"

#pragma force_active on

#define NEXT_ARG(ctx) (*(u32 *)((ctx)->script += 4))

void ov07_0221F120(Ov07CallbackContext *ctx) {
    Ov07SoundEffect *effect = ov07_0221C53C(ctx);
    memset(effect, 0, sizeof(*effect));
    effect->kind = 1;
    effect->sequence = (u16)NEXT_ARG(ctx);
    effect->x = NEXT_ARG(ctx);
    effect->y = NEXT_ARG(ctx);
    effect->z = NEXT_ARG(ctx);
    effect->byte03 = (u8)NEXT_ARG(ctx);
    ctx->script += 4;
    effect->x = ov07_0221F8C8(ctx, (s8)effect->x);
    effect->y = ov07_0221F8C8(ctx, (s8)effect->y);
    effect->z = ov07_0221F980((s8)effect->x, (s8)effect->y, (s8)effect->z);
    PlaySE(effect->sequence);
    SetSeqTrackPan(effect->sequence, 0xFFFF, effect->x);
    ov07_0221C56C(ctx, effect);
}

void ov07_0221F1C0(Ov07CallbackContext *ctx) {
    Ov07SoundEffect *effect = ov07_0221C53C(ctx);
    memset(effect, 0, sizeof(*effect));
    effect->kind = 2;
    effect->sequence = (u16)NEXT_ARG(ctx);
    effect->x = (s8)NEXT_ARG(ctx);
    effect->y = (s8)NEXT_ARG(ctx);
    effect->z = (s8)NEXT_ARG(ctx);
    effect->byte03 = (u8)NEXT_ARG(ctx);
    ctx->script += 4;
    PlaySE(effect->sequence);
    SetSeqTrackPan(effect->sequence, 0xFFFF, effect->x);
    ov07_0221C56C(ctx, effect);
}

void ov07_0221F238(Ov07CallbackContext *ctx) {
    Ov07SoundEffect *effect = ov07_0221C53C(ctx);
    memset(effect, 0, sizeof(*effect));
    effect->kind = 1;
    effect->sequence = (u16)NEXT_ARG(ctx);
    effect->x = (s8)NEXT_ARG(ctx);
    effect->y = (s8)NEXT_ARG(ctx);
    effect->z = (s8)NEXT_ARG(ctx);
    effect->byte03 = (u8)NEXT_ARG(ctx);
    ctx->script += 4;
    effect->x = ov07_0221F8C8(ctx, (s8)effect->x);
    effect->y = ov07_0221F8C8(ctx, (s8)effect->y);
    effect->z = ov07_0221F8C8(ctx, (s8)effect->z);
    PlaySE(effect->sequence);
    SetSeqTrackPan(effect->sequence, 0xFFFF, effect->x);
    ov07_0221C56C(ctx, effect);
}

void ov07_0221F2DC(Ov07CallbackContext *ctx) {
    Ov07SoundEffect *effect = ov07_0221C53C(ctx);
    memset(effect, 0, sizeof(*effect));
    effect->kind = 4;
    effect->sequence = (u16)NEXT_ARG(ctx);
    effect->value14 = (s8)NEXT_ARG(ctx);
    effect->byte03 = (u8)NEXT_ARG(ctx);
    effect->byte18 = (u8)NEXT_ARG(ctx);
    ctx->script += 4;
    effect->byte04 = effect->byte03;
    effect->value14 = ov07_0221F8C8(ctx, (s8)effect->value14);
    ov07_0221C56C(ctx, effect);
}

void ov07_0221F340(Ov07CallbackContext *ctx) {
    Ov07SoundEffect *effect = ov07_0221C53C(ctx);
    memset(effect, 0, sizeof(*effect));
    effect->kind = 5;
    effect->sequence = (u16)NEXT_ARG(ctx);
    effect->value14 = (s8)NEXT_ARG(ctx);
    effect->byte03 = (u8)NEXT_ARG(ctx);
    ctx->script += 4;
    effect->value14 = ov07_0221F8C8(ctx, (s8)effect->value14);
    ov07_0221C56C(ctx, effect);
}
