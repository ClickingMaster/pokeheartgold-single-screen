#include "to50_overlay15_r19_private.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void BagApp_ShowCancelButton(void *work, u32 mode) {
    if (mode == 1) {
        BagApp_SetCancelSpriteVisible(work, 1);
        BagApp_PrintCancelButton(work, 0);
        ScheduleWindowCopyToVram((u8 *)work + 0x74);
        ManagedSprite_SetAnimationFrame(PTR(work, 0x29c), 0);
        ManagedSprite_SetAnim(PTR(work, 0x29c), 0x10);
    } else {
        BagApp_SetCancelSpriteVisible(work, 0);
        BagApp_ClearCancelButtonText(work);
    }
}

typedef struct Ov15Request {
    u8 index;
    u8 palette : 4;
    u8 cycle : 4;
    u8 active;
    u8 phaseA : 4;
    u8 phaseB : 4;
    void *callback;
} Ov15Request;

int BagApp_StartButtonFlash(void *work, u8 index, u8 palette, u8 cycle, void *callback) {
    Ov15Request *state = (Ov15Request *)((u8 *)work + 0x940);
    state->active = 0;
    state->index = index;
    state->palette = palette;
    state->cycle = cycle;
    state->callback = callback;
    state->phaseA = 0;
    state->phaseB = 0;
    return 0x23;
}

int BagApp_StartButtonAnim(void *work, u8 index, u32 anim, void *callback) {
    Ov15Request *state = (Ov15Request *)((u8 *)work + 0x940);
    void **sprites = (void **)((u8 *)work + 0x250);
    state->active = 1;
    state->index = index;
    state->callback = callback;
    state->phaseA = 0;
    state->phaseB = 0;
    ManagedSprite_SetAnimationFrame(sprites[index], 0);
    ManagedSprite_SetAnim(sprites[index], anim);
    return 0x23;
}

int BagApp_State_ButtonFeedback(void *work) {
    Ov15Request *state = (Ov15Request *)((u8 *)work + 0x940);
    void **sprites = (void **)((u8 *)work + 0x250);
    if (state->active == 1) {
        if (ManagedSprite_IsAnimated(PTR((u8 *)work + state->index * 4, 0x250)) == 0) {
            return (int)state->callback;
        }
    } else {
        switch (state->phaseA) {
        case 0:
            ManagedSprite_SetPaletteOverride(PTR((u8 *)work + state->index * 4, 0x250), state->palette);
            state->phaseA++;
            break;
        case 1:
            state->phaseB++;
            if (state->phaseB == 4) {
                ManagedSprite_SetPaletteOverride(PTR((u8 *)work + state->index * 4, 0x250), state->cycle);
                state->phaseB = 0;
                state->phaseA++;
            }
            break;
        case 2:
            state->phaseB++;
            if (state->phaseB == 2) {
                return (int)state->callback;
            }
            break;
        }
    }
    return 0x23;
}
