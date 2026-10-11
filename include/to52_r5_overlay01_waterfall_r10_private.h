#ifndef W18_R5_PRIVATE_H
#define W18_R5_PRIVATE_H
#include "global.h"

typedef struct W18WaterfallWork {
    int state;
    int direction;
    int counter;
    int targetX;
    int targetY;
    int targetZ;
    int progress;
    int deltaX;
    int deltaY;
    int deltaZ;
    int stepX;
    int stepY;
    int stepZ;
    void *fieldSystem;
    u32 pad38;
    void *mapObject;
    u8 pad40[0x10];
    void *effect;
} W18WaterfallWork;

void MapObject_CopyPositionVector(void *, VecFx32 *);
void MapObject_SetPositionVector(void *, VecFx32 *);
int MapObject_GetXCoord(void *);
int MapObject_GetZCoord(void *);
void MapObject_SetCurrentX(void *, int);
void MapObject_SetCurrentY(void *, int);
void MapObject_SetCurrentZ(void *, int);
void MapObject_SetPreviousCoordsToCurrent(void *);
void SetPositionToTileCenter(int, int, VecFx32 *);
int sub_02054790(void *, int, int, fx32, fx32, u8 *);
int GetDeltaXByFacingDirection(int);
int GetDeltaYByFacingDirection(int);
void *ov02_0224D598(void *);
void ov02_0224D5AC(void *);
int ov01_021F29E4(W18WaterfallWork *, void *);
int ov01_021F2AB8(W18WaterfallWork *, void *);
int ov01_021F2AF8(W18WaterfallWork *, void *);

#endif
