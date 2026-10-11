#include "global.h"

typedef struct Movement075WorkPrivate {
    u32 parameter;
    void *effect;
} Movement075WorkPrivate;

typedef struct MovementPathWorkPrivate {
    s16 facing;
    s16 field2;
    s16 count;
    s16 index;
} MovementPathWorkPrivate;

extern void *sub_0205F3C0(void *, u32);
extern void *sub_0205F3E4(void *);
extern void *EmoteMark_Spawn(void *, u32, u32);
extern BOOL EmoteMark_IsDone(void *);
extern void FieldEffect_Destroy(void *);
extern void MapObject_IncrementMovementStep(void *);
extern void MapObject_MoveCoordsInDirection(void *, u32);
extern void MapObject_SetOrQueueFacing(void *, u32);
extern void sub_0205F328(void *, u32);
extern void MapObject_SetFlagsBits(void *, u32);
extern void MapObject_OffsetPositionInDirection(void *, s32, u32);
extern void MapObject_UpdateHeight(void *);
extern void MapObject_SetPreviousCoordsToCurrent(void *);
extern void sub_0205F484(void *);

void sub_02062F48(void *, u32);
BOOL MapObjectMovementCmd075_Step1(void *, void *);
BOOL MapObjectMovementCmd075_Step0(void *, void *);
BOOL MapObjectMovementCmd103_Step0(void *, void *);
void sub_02062FAC(void *, u32, u32, u32);
BOOL sub_02062FEC(void *, const u32 *);
