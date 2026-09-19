#include "overlay07_callbacks_w06_private.h"

void ov07_0222D56C(W06Sprite *work) {
    Sprite_DeleteAndFreeResources(work->sprite);
}
void ov07_0222D578(W06Sprite *work) {
    ManagedSprite_SetDrawFlag(work->sprite, TRUE);
    ManagedSprite_SetAnimateFlag(work->sprite, TRUE);
}
