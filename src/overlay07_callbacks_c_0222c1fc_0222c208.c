#include "overlay07_callbacks_w06_private.h"

void ov07_0222C1FC(W06Sprite *work) {
    Sprite_DeleteAndFreeResources(work->sprite);
}
void ov07_0222C208(W06Sprite *work, BOOL draw) {
    ManagedSprite_SetDrawFlag(work->sprite, draw);
}
