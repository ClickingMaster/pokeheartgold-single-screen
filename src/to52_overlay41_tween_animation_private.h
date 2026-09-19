#ifndef W07_OV41_PRIVATE_H
#define W07_OV41_PRIVATE_H
#include "global.h"
#include "sprite.h"
typedef struct W07Tween { fx32 out; fx32 base; s32 distance; s32 frame; s32 frames; } W07Tween;
typedef struct W07Anim {
    u32 active;
    union { struct { Sprite *sprite0; Sprite *sprite1; }; Sprite *sprites[2]; };
    union { struct { VecFx32 pos0; VecFx32 pos1; }; VecFx32 positions[2]; };
    W07Tween scale;
    W07Tween x;
    W07Tween y;
} W07Anim;
typedef struct W07Source { u8 pad[0x10]; Sprite *sprites[2]; u8 pad18[4]; s32 value; } W07Source;
typedef struct W07Arg { u8 pad[0x74]; u32 index; } W07Arg;
typedef struct W07LoadArgs { void *p; u32 member; u32 count; void *dst; } W07LoadArgs;
BOOL ov41_0224B32C(W07Tween *);
void ov41_0224B31C(W07Tween *, fx32, fx32, int);
void ov41_0224B374(W07Source *, W07Anim *);
void ov41_0224B450(W07Anim *);
void ov41_0224B4E8(void **, W07Arg *, void *);
void ov41_02249CE0(void *, W07LoadArgs *);
#endif
