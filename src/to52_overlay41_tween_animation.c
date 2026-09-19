#include "to52_overlay41_tween_animation_private.h"

BOOL ov41_0224B32C(W07Tween *p) {
    int next;
    fx32 value;
    value = FX_Div((fx32)(((s64)p->distance * (p->frame << FX32_SHIFT) + FX32_HALF) >> FX32_SHIFT), p->frames << FX32_SHIFT);
    p->out = value + p->base;
    next = p->frame + 1;
    if (next <= p->frames) {
        p->frame = next;
        return FALSE;
    }
    p->frame = p->frames;
    return TRUE;
}

void ov41_0224B374(W07Source *src, W07Anim *anim) {
    VecFx32 pos;
    fx32 scale;
    int i;

    pos.y = 0x3A << FX32_SHIFT;
    pos.y += 2 << 20;
    for (i = 0; i < 2; i++) {
        anim->sprites[i] = src->sprites[i];
        Sprite_SetAffineOverwriteMode(anim->sprites[i], 2);
        pos.x = 0x67 + 0x18 * i;
        pos.x <<= FX32_SHIFT;
        anim->positions[i] = pos;
    }

    i = 10 - src->value;
    if (i > 0) {
        scale = FX_Mul(i << FX32_SHIFT, 0x266) + FX32_ONE;
    } else {
        scale = FX32_ONE;
    }
    ov41_0224B31C(&anim->scale, scale, FX32_ONE, 0x10);
    scale = FX_Mul(scale, 0x18000) - 0x18000;
    ov41_0224B31C(&anim->x, scale, 0, 0x10);
    ov41_0224B31C(&anim->y, scale, 0, 0x10);
    anim->active = TRUE;
}

void ov41_0224B450(W07Anim *p) {
    VecFx32 scale;
    VecFx32 vec;
    BOOL done;
    if (p->active == 0) {
        return;
    }
    done = ov41_0224B32C(&p->scale);
    ov41_0224B32C(&p->x);
    ov41_0224B32C(&p->y);
    scale.x = p->scale.out;
    scale.y = p->scale.out;
    scale.z = p->scale.out;
    Sprite_SetAffineScale(p->sprite0, &scale);
    Sprite_SetAffineScale(p->sprite1, &scale);
    vec = p->pos0;
    vec.x -= p->x.out;
    vec.y -= p->y.out;
    Sprite_SetMatrix(p->sprite0, &vec);
    vec = p->pos1;
    vec.y -= p->y.out;
    Sprite_SetMatrix(p->sprite1, &vec);
    if (done) {
        p->active = 0;
    }
}

void ov41_0224B4E8(void **dst, W07Arg *arg, void *out) {
    W07LoadArgs args;
    *dst++ = arg;
    args.p = (u8 *)arg + 0x48;
    args.member = arg->index * 4 + 0x89;
    args.count = 0x10;
    args.dst = out;
    ov41_02249CE0(dst, &args);
}
