#include "to52_r6_overlay96_position_matrix_r26_private.h"

void ov96_021EAD88(W07PositionObject *obj, s32 x, s32 y, BOOL updateSecond) {
    VecFx32 matrix;
    VecFx32 position = { 0 };
    Sprite *sprite = ov96_021E8BAC(obj->object);

    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    obj->x = x + obj->halfHeight;
    obj->y = y + obj->radius;
    if (obj->sprite2 != NULL) {
        matrix = position;
        matrix.x += obj->halfHeight << FX32_SHIFT;
        matrix.y += (2 * obj->radius - obj->offsetY) << FX32_SHIFT;
        Sprite_SetMatrix(obj->sprite2, &matrix);
    }
    if (updateSecond && obj->sprite1 != NULL) {
        matrix = position;
        matrix.x += obj->halfHeight << FX32_SHIFT;
        matrix.y += (2 * obj->radius - obj->offsetX) << FX32_SHIFT;
        Sprite_SetMatrix(obj->sprite1, &matrix);
    }
    if (ov96_021E92AC(obj->object)) {
        position.y += 0x8000;
    }
    Sprite_SetMatrix(sprite, &position);
}

void ov96_021EAE4C(W07PositionObject *obj, s32 x, s32 y) {
    VecFx32 position = { 0 };
    Sprite *sprite = ov96_021E8BAC(obj->object);
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    obj->x = x + obj->halfHeight;
    obj->y = y + obj->radius;
    if (ov96_021E92AC(obj->object)) {
        position.y += 0x8000;
    }
    Sprite_SetMatrix(sprite, &position);
}

void ov96_021EAE9C(void *object, s32 *x, s32 *y) {
    VecFx32 *matrix = Sprite_GetMatrixPtr(ov96_021E8BAC(ov96_021EAA20(object)));
    *x = ((s32)(matrix->x + ((u32)(matrix->x >> 11) >> 20))) >> FX32_SHIFT;
    *y = ((s32)(matrix->y + ((u32)(matrix->y >> 11) >> 20))) >> FX32_SHIFT;
}
