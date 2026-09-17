#ifndef POKEHEARTGOLD_UNK_02061C40_INTERNAL_H
#define POKEHEARTGOLD_UNK_02061C40_INTERNAL_H
#include "map_object_movement_internal.h"
typedef struct MovementPathState {
    u8 active;
    u8 index;
    u8 limit;
    u8 axis;
    u32 tableId;
    u8 saved[4];
} MovementPathState;
BOOL sub_02061C40(LocalMapObject *object, MovementPathState *state);
u32 sub_02060BB8(LocalMapObject *object, int direction);
void sub_0206207C(LocalMapObject *object, u8 *state);
#endif
