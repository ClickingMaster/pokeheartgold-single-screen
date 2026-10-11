#include "to52_r5_overlay01_waterfall_r10_private.h"

int ov01_021F29E4(W18WaterfallWork *work, void *taskManager) {
    VecFx32 position;
    VecFx32 step;
    u8 kind;
    int x = MapObject_GetXCoord(work->mapObject) + (GetDeltaXByFacingDirection(0) << 1);
    int z = MapObject_GetZCoord(work->mapObject) + (GetDeltaYByFacingDirection(0) << 1);

    SetPositionToTileCenter(x, z, (VecFx32 *)&work->stepX);
    work->stepY = sub_02054790(work->fieldSystem, 1, work->stepY, work->stepX, work->stepZ, &kind);
    GF_ASSERT(kind != 0);
    work->targetX = x;
    work->targetY = (work->stepY >> 3) / FX32_ONE;
    work->targetZ = z;
    MapObject_CopyPositionVector(work->mapObject, &position);
    GF_ASSERT(position.z > work->stepZ);
    GF_ASSERT(position.y < work->stepY);
    step.x = 0;
    step.y = work->stepY - position.y;
    step.z = work->stepZ - position.z;
    step.y /= 64;
    step.z /= 64;
    *(VecFx32 *)&work->deltaX = step;
    work->effect = ov02_0224D598(work->fieldSystem);
    work->state++;
    return 0;
}

int ov01_021F2AB8(W18WaterfallWork *work, void *taskManager) {
    VecFx32 position;

    MapObject_CopyPositionVector(work->mapObject, &position);
    position.y += work->deltaY;
    if (position.y > work->stepY) {
        position.y = work->stepY;
    }
    MapObject_SetPositionVector(work->mapObject, &position);
    work->counter++;
    if (work->counter >= 0x20) {
        work->counter = 0;
        work->state++;
    }
    return 0;
}

int ov01_021F2AF8(W18WaterfallWork *work, void *taskManager) {
    VecFx32 position;

    MapObject_CopyPositionVector(work->mapObject, &position);
    position.y += work->deltaY;
    if (position.y > work->stepY) {
        position.y = work->stepY;
    }
    position.z += work->deltaZ;
    if (position.z < work->stepZ) {
        position.z = work->stepZ;
    }
    MapObject_SetPositionVector(work->mapObject, &position);
    work->counter++;
    if (work->counter < 0x40) {
        return 0;
    }
    GF_ASSERT(position.z == work->stepZ);
    GF_ASSERT(position.y == work->stepY);
    MapObject_SetCurrentX(work->mapObject, work->targetX);
    MapObject_SetCurrentY(work->mapObject, work->targetY);
    MapObject_SetCurrentZ(work->mapObject, work->targetZ);
    MapObject_SetPreviousCoordsToCurrent(work->mapObject);
    ov02_0224D5AC(work->effect);
    return 1;
}
