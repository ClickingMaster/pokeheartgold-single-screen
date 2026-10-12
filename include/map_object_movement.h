#ifndef POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_H
#define POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_H

#include "map_object.h"

typedef struct LocalMovementState {
    s16 state;
    s16 delay;
    u32 restrictToRange;
    u32 heldMovement;
    u32 directionTable;
} LocalMovementState;

typedef struct LocalMovementBounds {
    s32 minX;
    s32 minZ;
    s32 maxX;
    s32 maxZ;
} LocalMovementBounds;

typedef struct DirectionCycle {
    int value[5];
} DirectionCycle;

typedef struct DirectionCycleState {
    s8 state;
    u8 unk_01;
    u8 flag;
} DirectionCycleState;

typedef struct Unk020619FCState {
    s16 state;
    s16 turn;
    u8 data[1];
} Unk020619FCState;

typedef struct MovementPathState {
    u8 active;
    u8 index;
    u8 limit;
    u8 axis;
    u32 tableId;
    u8 saved[4];
} MovementPathState;

typedef struct MovementListEntry {
    int id;
    const int *list;
} MovementListEntry;

typedef BOOL (*MovementStateFunc)(LocalMapObject *object, u8 *state);

void RandomLookMovement_Init(LocalMapObject *object, u32 value);
void sub_020612B4(LocalMapObject *object);
void sub_020612C0(LocalMapObject *object);
void sub_020612CC(LocalMapObject *object);
void sub_020612D8(LocalMapObject *object);
void sub_020612E4(LocalMapObject *object);
void sub_020612F0(LocalMapObject *object);
void sub_020612FC(LocalMapObject *object);
void sub_02061308(LocalMapObject *object);
void sub_02061314(LocalMapObject *object);
void sub_02061320(LocalMapObject *object);
void sub_0206132C(LocalMapObject *object);
void RandomLookMovement_Update(LocalMapObject *object);
void RandomLookMovement_Delete(LocalMapObject *object);
void sub_020613A0(LocalMapObject *object);
void sub_020613B0(LocalMapObject *object);
void sub_020613C0(LocalMapObject *object);
void sub_020613D0(LocalMapObject *object, u32 first, u32 second, u32 third);
void sub_020613F8(LocalMapObject *object);
void MapObject_GetMovementBounds(LocalMapObject *object, LocalMovementBounds *bounds);
BOOL MapObject_IsWithinMovementBounds(LocalMapObject *object, int direction);
void sub_02061648(LocalMapObject *object, u32 direction);
void sub_0206166C(LocalMapObject *object);
void sub_02061690(LocalMapObject *object);
void sub_0206169C(LocalMapObject *object);
void sub_020616A8(LocalMapObject *object);
void sub_020616B4(LocalMapObject *object);
void sub_020616C0(LocalMapObject *object, u32 value);
void sub_020616E4(LocalMapObject *object);
void sub_020616F0(LocalMapObject *object);
void sub_020616FC(LocalMapObject *object);
BOOL sub_02061720(LocalMapObject *object, u8 *state);
BOOL sub_02061754(LocalMapObject *object, u8 *state);
BOOL sub_02061770(LocalMapObject *object, u8 *state);
BOOL sub_020617AC(LocalMapObject *object, s8 *state);
void sub_02061844(LocalMapObject *object);
void sub_02061850(LocalMapObject *object);
void sub_02061874(LocalMapObject *object, u8 *state);
BOOL sub_02061894(LocalMapObject *object, u8 *state);
BOOL sub_020618B0(LocalMapObject *object, u8 *state);
BOOL sub_020618C8(LocalMapObject *object, DirectionCycleState *state);
void sub_0206197C(LocalMapObject *object);
void sub_0206199C(LocalMapObject *object);
void sub_020619C0(LocalMapObject *object, u8 *state);
int sub_020619FC(void *mapObject, Unk020619FCState *state);
BOOL sub_02061ABC(LocalMapObject *object, u8 *state);
void sub_02061AEC(LocalMapObject *object, u32 first, u32 second, u32 value);
void sub_02061B1C(LocalMapObject *object);
void sub_02061B2C(LocalMapObject *object);
void sub_02061B3C(LocalMapObject *object);
void sub_02061B4C(LocalMapObject *object);
void sub_02061B5C(LocalMapObject *object);
void sub_02061B6C(LocalMapObject *object);
void sub_02061B7C(LocalMapObject *object);
void sub_02061B8C(LocalMapObject *object);
void sub_02061B9C(LocalMapObject *object);
void sub_02061BAC(LocalMapObject *object);
void sub_02061BBC(LocalMapObject *object);
void sub_02061BCC(LocalMapObject *object);
void sub_02061BDC(LocalMapObject *object);
void sub_02061BEC(LocalMapObject *object);
void sub_02061BFC(LocalMapObject *object);
void sub_02061C0C(LocalMapObject *object);
void sub_02061C1C(LocalMapObject *object);
BOOL sub_02061C40(LocalMapObject *object, MovementPathState *state);
BOOL sub_02061D50(LocalMapObject *object, u8 *state);
void sub_02061D80(LocalMapObject *object);
void sub_02061D90(LocalMapObject *object);
void sub_02061DA0(LocalMapObject *object);
void sub_02061DB0(LocalMapObject *object);
void sub_02061DC0(LocalMapObject *object);
void sub_02061DD0(LocalMapObject *object);
void sub_02061DE0(LocalMapObject *object);
void sub_02061DF0(LocalMapObject *object);
int DirectionList_Count(const int *list, int value);
int DirectionList_GetRandom(const int *list, int value);
int GetRandomDirectionFromTable(int tableId, int value);
const int *GetDirectionTable(int id);
int MapObject_GetDirectionToNearbyPlayer(LocalMapObject *object);
int sub_02061F5C(LocalMapObject *object, int tableId, int value);
BOOL sub_02062050(LocalMapObject *object);
void sub_02062064(LocalMapObject *object, u8 *state);
void sub_0206207C(LocalMapObject *object, s8 *state);
void sub_020620F8(LocalMapObject *object, u8 *state);

#endif // POKEHEARTGOLD_MAP_OBJECT_MOVEMENT_H
