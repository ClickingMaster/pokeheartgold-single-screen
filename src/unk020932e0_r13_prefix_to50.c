#include "unk020932e0_r13_prefix_to50_private.h"
void sub_0209515C(WorkR13Prefix *work) {
    int i;
    u8 remaining;
    for (i = 0; i < 30; i++) {
        if (work->entriesB[i].sprite != NULL) {
            Sprite_Delete(work->entriesB[i].sprite);
        }
    }
    for (i = 0; i < work->count; i++) {
        if (work->entries[i].sprite != NULL) {
            Sprite_Delete(work->entries[i].sprite);
        }
    }
    for (i = 0; i < work->count; i++) {
        if (work->list[i].sprite != NULL) {
            Sprite_Delete(work->list[i].sprite);
        }
    }
    if (work->sprite8B4 != NULL) {
        Sprite_Delete(work->sprite8B4);
    }
    if (work->sprite8BC != NULL) {
        Sprite_Delete(work->sprite8BC);
    }
    if (work->sprite8B8 != NULL) {
        Sprite_Delete(work->sprite8B8);
    }
    if (work->sprite8C4 != NULL) {
        Sprite_Delete(work->sprite8C4);
    }
    if (work->sprite8C0 != NULL) {
        Sprite_Delete(work->sprite8C0);
    }
    remaining = 3 - work->count;
    for (i = 0; i < remaining; i++) {
        if (work->extra[i] != NULL) {
            Sprite_Delete(work->extra[i]);
        }
    }
}

void sub_02095238(WorkR13Prefix *work) {
    VecFx32 matrix;
    VecFx32 copy;
    int enabled = 0;
    int type = GridCursor_GetPanel(work->cursor);
    int value = GridCursor_GetIndex(work->cursor);
    matrix.z = 0;
    switch (type) {
    case 5:
        matrix.x = 5 << 16;
        matrix.y = 1 << 16;
        enabled = 1;
        break;
    case 2:
    case 6:
        matrix.x = ((u8)(value % 6) * 24 + 20) << 12;
        matrix.y = ((u8)(value / 6) * 24 + 40) << 12;
        enabled = 1;
        break;
    case 3:
        matrix.x = 0x35 << 14;
        matrix.y = (value + 1) * 40 << 12;
        enabled = 1;
        break;
    case 4:
        matrix.x = 0x35 << 14;
        matrix.y = (value + 1) * 40 << 12;
        if (work->flag46BC != 0) {
            copy = matrix;
            copy.y -= 1 << 14;
            Sprite_SetMatrix(work->entriesPtr[work->index4684].sprite, &copy);
            work->posX = 0xD4;
            work->posY = (value + 1) * 40;
        }
        enabled = 1;
        break;
    case 1:
        matrix.x = 14 << 16;
        matrix.y = 11 << 16;
        enabled = 1;
        break;
    }
    if (enabled != 0) {
        Sprite_SetMatrix(work->sprite8C0, &matrix);
        sub_020956B8(work);
    }
}
