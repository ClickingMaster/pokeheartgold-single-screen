#include "to52_r5_overlay71_sprite_fall_r20_private.h"

typedef struct Ov71SpritePair {
    Sprite *first;
    Sprite *second;
} Ov71SpritePair;

typedef struct Ov71SpriteSource {
    u8 padding[0x1c];
    Ov71SpritePair pairs[20];
} Ov71SpriteSource;

typedef struct Ov71FallTask {
    SysTask **owner;
    Ov71SpriteSource *source;
    int visible[20];
    VecFx32 positions[20];
    int delay;
    int reveal;
} Ov71FallTask;

typedef struct Ov71ExitWork {
    u8 padding_00[0xc0];
    BgConfig *bgConfig;
    u8 padding_C4[0xa4];
    void *active;
} Ov71ExitWork;

void ov71_02248B60(SysTask *task, void *arg);
void ov71_02248BBC(Ov71SpriteSource *source, SysTask **owner, Ov71FallTask *state);
void ov71_02248C24(SysTask *task, void *arg);

void ov71_02248B60(SysTask *task, void *arg) {
    Ov71ExitWork *work = arg;

    if (work->active != NULL) {
        work->active = NULL;
        BgSetPosTextAndCommit(work->bgConfig, 2, BG_POS_OP_SET_Y, -256);
        BgSetPosTextAndCommit(work->bgConfig, 6, BG_POS_OP_SET_Y, 0);
        ToggleBgLayer(3, FALSE);
        ToggleBgLayer(7, TRUE);
        *(volatile u16 *)0x04000304 |= 0x8000;
        SysTask_Destroy(task);
    }
}

void ov71_02248BBC(Ov71SpriteSource *source, SysTask **owner, Ov71FallTask *state) {
    int i;

    state->source = source;
    state->owner = owner;
    state->delay = 0;
    state->reveal = 0;
    for (i = 0; i < 20; i++) {
        state->visible[i] = 0;
        state->positions[i] = *Sprite_GetMatrixPtr(source->pairs[i].first);
    }
    *state->owner = SysTask_CreateOnMainQueue(ov71_02248C24, state, 0);
    if (*state->owner == NULL) {
        Heap_Free(state);
    }
}

void ov71_02248C24(SysTask *task, void *arg) {
    Ov71FallTask *state = arg;
    int i;

    state->delay++;
    if (state->delay > 12) {
        state->delay = 0;
        if (state->reveal < 20) {
            state->visible[state->reveal] = TRUE;
            state->reveal++;
        }
    }

    for (i = 0; i < 20; i++) {
        VecFx32 second;

        state->positions[i].y += 0x21000;
        if (state->positions[i].y >= 0x1a8000) {
            state->positions[i].y -= 0x1c4000;
            if (state->visible[i]) {
                Sprite_SetDrawFlag(state->source->pairs[i].first, TRUE);
                Sprite_SetDrawFlag(state->source->pairs[i].second, TRUE);
            }
        }
        Sprite_SetMatrix(state->source->pairs[i].first, &state->positions[i]);
        second = state->positions[i];
        second.y += 0x38000;
        Sprite_SetMatrix(state->source->pairs[i].second, &second);
    }
}
