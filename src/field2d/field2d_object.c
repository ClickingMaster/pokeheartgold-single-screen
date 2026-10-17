#include "overlay_42.h"

const s8 sField2DDirectionOffsets[8] = { 0, 240, 0, 16, 240, 0, 16, 0 };
const u8 sField2DOppositeDirections[4] = { 1, 0, 3, 2 };

const Field2DObjectStartFunc sField2DObjectStartFuncs[] = { Field2DObject_StartStand, Field2DObject_StartTurn, Field2DObject_StartWalk, Field2DObject_StartRun, Field2DObject_StartPause, Field2DObject_StartBump, Field2DObject_StartWalkSlow, Field2DObject_StartWalkFast, Field2DObject_StartWalkInPlaceVeryFast, Field2DObject_StartWalkInPlaceFast, Field2DObject_StartWalkInPlace, Field2DObject_StartWalkInPlaceSlow };

const Field2DObjectUpdateFunc sField2DObjectUpdateFuncs[] = { Field2DObject_UpdateStand, Field2DObject_UpdateTurn, Field2DObject_UpdateWalk, Field2DObject_UpdateRun, Field2DObject_UpdatePause, Field2DObject_UpdateBump, Field2DObject_UpdateWalk, Field2DObject_UpdateWalk, Field2DObject_UpdateWalkInPlace, Field2DObject_UpdateWalkInPlace, Field2DObject_UpdateWalkInPlace, Field2DObject_UpdateWalkInPlace };

const Field2DCommandCheckFunc sField2DCommandCheckFuncs[] = { Field2DCommand_CheckStand, Field2DCommand_CheckTurn, Field2DCommand_CheckWalk, Field2DCommand_CheckRun, Field2DCommand_CheckPause, Field2DCommand_CheckBump, Field2DCommand_CheckWalkSlow, Field2DCommand_CheckWalk, Field2DCommand_CheckWalkInPlace, Field2DCommand_CheckWalkInPlace, Field2DCommand_CheckWalkInPlace, Field2DCommand_CheckWalkInPlace };

s32 Field2DMap_IsBlocked(Field2DMap *map, u16 x, u16 y) {
    GF_ASSERT(map);
    Field2DMapCell cell = Field2DMap_GetCell(map, x, y);
    if (cell.value == -1) {
        return 1;
    }
    return (u32)((s32)cell.value << 31) >> 31;
}

u32 Field2DMap_GetTag(Field2DMap *map, u16 x, u16 y) {
    GF_ASSERT(map);
    Field2DMapCell cell = Field2DMap_GetCell(map, x, y);
    if (cell.value == -1) {
        return 0;
    }
    return cell.value / 2;
}

Field2DObjectManager *Field2DObjectManager_New(u32 capacity, enum HeapID heapID) {
    Field2DObjectManager *objectManager = Heap_Alloc(heapID, sizeof(Field2DObjectManager));
    GF_ASSERT(objectManager);
    objectManager->capacity = capacity;
    Field2DObject *object = Heap_Alloc(heapID, capacity * sizeof(Field2DObject));
    objectManager->objects = object;
    GF_ASSERT(object);
    memset(objectManager->objects, 0, objectManager->capacity * sizeof(Field2DObject));
    return objectManager;
}

void Field2DObjectManager_Free(Field2DObjectManager *objectManager) {
    GF_ASSERT(objectManager);
    Heap_Free(objectManager->objects);
    Heap_Free(objectManager);
}

void Field2DObjectManager_StartMove(Field2DObjectManager *objectManager, Field2DMove *move) {
    Field2DObject_StartMove(Field2DObjectManager_FindById(objectManager, move->unk7), move);
}

void Field2DObjectManager_Update(Field2DObjectManager *objectManager) {
    int capacity = Field2DObjectManager_GetCapacity(objectManager);
    for (u32 i = 0; i < capacity; i++) {
        Field2DObject *object = Field2DObjectManager_GetSlot(objectManager, i);
        if (object != NULL) {
            Field2DObject_Update(object);
        }
    }
}

u32 Field2DObjectManager_GetCapacity(Field2DObjectManager *objectManager) {
    GF_ASSERT(objectManager);
    return objectManager->capacity;
}

Field2DObject *Field2DObjectManager_AddObject(Field2DObjectManager *objectManager, Field2DObjectTemplate *template) {
    GF_ASSERT(objectManager);
    GF_ASSERT(template);
    Field2DObject *object;
    object = Field2DObjectManager_GetFreeSlot(objectManager);
    object->pos.unk0 = template->unk0;
    object->pos.unk2 = template->unk2;
    object->prevPos.unk0 = template->unk0;
    object->prevPos.unk2 = template->unk2;
    object->id = template->unk4;
    object->action = template->unk6;
    object->direction = template->unk8;
    object->spriteId = template->unkA;
    object->active = 1;
    return object;
}

void Field2DObject_Delete(void *object) {
    memset(object, 0, 24);
}

Field2DObject *Field2DObjectManager_FindById(Field2DObjectManager *objectManager, u8 id) {
    for (int i = 0; i < objectManager->capacity; i++) {
        if ((objectManager->objects[i].active == 1) && (id == objectManager->objects[i].id)) {
            return &objectManager->objects[i];
        }
    }
    return NULL;
}

Field2DObject *Field2DObjectManager_GetSlot(Field2DObjectManager *objectManager, u16 index) {
    GF_ASSERT(objectManager);
    GF_ASSERT(index < objectManager->capacity);
    if (objectManager->objects[index].active == 1) {
        return &objectManager->objects[index];
    }
    return 0;
}

Field2DObject *Field2DObjectManager_GetById(Field2DObjectManager *objectManager, u8 id) {
    return Field2DObjectManager_FindById(objectManager, id);
}

Field2DObject *Field2DObjectManager_GetByIndex(Field2DObjectManager *objectManager, u16 index) {
    return Field2DObjectManager_GetSlot(objectManager, index);
}

s32 Field2DObject_GetParam(Field2DObject *object, s32 param) {
    s32 value;
    GF_ASSERT(object);
    switch (param) {
    case 0:
        value = object->pos.unk0;
        break;
    case 1:
        value = object->pos.unk2;
        break;
    case 2:
        value = object->prevPos.unk0;
        break;
    case 3:
        value = object->prevPos.unk2;
        break;
    case 4:
        value = object->id;
        break;
    case 5:
        value = object->action;
        break;
    case 6:
        value = object->direction;
        break;
    case 7:
        value = object->spriteId;
        break;
    case 8:
        value = object->actionFrame;
        break;
    case 9:
        value = object->actionDuration;
        break;
    case 10:
        value = object->stateFrames;
        break;
    default:
        GF_ASSERT(FALSE);
    }
    return value;
}

void Field2DObject_SetParam(Field2DObject *object, u32 param, s32 value) {
    GF_ASSERT(object);
    switch (param) {
    case 0:
        object->pos.unk0 = value;
        return;
    case 1:
        object->pos.unk2 = value;
        return;
    case 2:
        object->prevPos.unk0 = value;
        return;
    case 3:
        object->prevPos.unk2 = value;
        return;
    case 4:
        object->id = value;
        return;
    case 5:
        GF_ASSERT(value < 12);
        object->action = value;
        return;
    case 6:
        GF_ASSERT(value < 4);
        object->direction = value;
        return;
    case 7:
        object->spriteId = value;
        return;
    case 8:
        object->actionFrame = value;
        return;
    case 9:
        object->actionDuration = value;
        return;
    case 10:
        object->stateFrames = value;
        return;
    }
    GF_ASSERT(FALSE);
}

Field2DPoint Field2D_StepInDirection(Field2DPoint pos, s32 direction) {
    s32 offsetIndex = direction * 2;
    pos.unk0 = pos.unk0 + sField2DDirectionOffsets[direction * 2];
    pos.unk2 = pos.unk2 + sField2DDirectionOffsets[direction * 2 + 1];

    return pos;
}

u8 Field2D_GetOppositeDirection(s32 direction) {
    return sField2DOppositeDirections[direction];
}

Field2DObject *Field2DObjectManager_GetFreeSlot(Field2DObjectManager *objectManager) {
    for (int i = 0; i < objectManager->capacity; i++) {
        if (objectManager->objects[i].active == 0) {
            return &objectManager->objects[i];
        }
    }
    GF_ASSERT(FALSE);
    return NULL;
}

Field2DPoint Field2DObject_GetPos(Field2DObject *object) {
    return object->pos;
}

Field2DPoint Field2DObject_GetPrevPos(Field2DObject *object) {
    return object->prevPos;
}

Field2DPoint Field2DObject_GetDrawPos(Field2DObject *object) {
    s32 actionFrame = Field2DObject_GetParam(object, 8);
    s32 actionDuration = Field2DObject_GetParam(object, 9);

    Field2DPoint drawPos = Field2DObject_GetPos(object);
    Field2DPoint prevPos = Field2DObject_GetPrevPos(object);

    drawPos.unk0 -= prevPos.unk0;
    drawPos.unk2 -= prevPos.unk2;
    if (actionFrame > 0) {
        drawPos.unk0 = actionFrame * drawPos.unk0 / actionDuration;
        drawPos.unk2 = actionFrame * drawPos.unk2 / actionDuration;
    } else {
        drawPos.unk0 = 0;
        drawPos.unk2 = 0;
    }
    drawPos.unk0 += prevPos.unk0;
    drawPos.unk2 += prevPos.unk2;
    return drawPos;
}

void Field2DObject_SetPos(Field2DObject *object, Field2DPoint pos) {
    object->pos = pos;
}

void Field2DObject_SetPrevPos(Field2DObject *object, Field2DPoint prevPos) {
    object->prevPos = prevPos;
}

Field2DObject *Field2DObject_FindObjectInDirection(Field2DObject *object, Field2DObjectManager *objectManager, s32 direction) {
    u32 capacity;
    u32 id;

    Field2DPoint targetPos = Field2DObject_GetPos(object);
    targetPos = Field2D_StepInDirection(targetPos, direction);
    id = Field2DObject_GetParam(object, 4);
    capacity = Field2DObjectManager_GetCapacity(objectManager);
    for (u32 i = 0; i < capacity; i++) {
        Field2DObject *other = Field2DObjectManager_GetByIndex(objectManager, i);
        if (other != 0) {
            u32 otherId = Field2DObject_GetParam(other, 4);
            if (otherId != id) {
                Field2DPoint otherPos = Field2DObject_GetPos(other);
                if ((otherPos.unk0 == targetPos.unk0) && (otherPos.unk2 == targetPos.unk2)) {
                    return other;
                }
                otherPos = Field2DObject_GetPrevPos(other);
                if ((otherPos.unk0 == targetPos.unk0) && (otherPos.unk2 == targetPos.unk2)) {
                    return other;
                }
            }
        }
    }
    return NULL;
}

Field2DObject *Field2DObjectManager_FindObjectAt(Field2DObjectManager *objectManager, Field2DPoint pos) {
    u32 capacity = Field2DObjectManager_GetCapacity(objectManager);
    for (u32 i = 0; i < capacity; i++) {
        Field2DObject *object = Field2DObjectManager_GetByIndex(objectManager, i);
        if (object != NULL) {
            Field2DPoint struct0 = Field2DObject_GetPos(object);
            if ((struct0.unk0 == pos.unk0) && (struct0.unk2 == pos.unk2)) {
                return object;
            }
            struct0 = Field2DObject_GetPrevPos(object);
            if ((struct0.unk0 == pos.unk0) && (struct0.unk2 == pos.unk2)) {
                return object;
            }
        }
    }
    return NULL;
}

void Field2DObject_StartMove(Field2DObject *object, Field2DMove *move) {
    sField2DObjectStartFuncs[move->unk4](object, move->unk0, move->unk6);
    object->stateFrames = 0;
}

void Field2DObject_Update(Field2DObject *object) {
    s32 done = sField2DObjectUpdateFuncs[Field2DObject_GetParam(object, 5)](object);
    object->stateFrames++;
    if (done == 1) {
        Field2DObject_EndAction(object);
        object->stateFrames = 0;
    }
}

void Field2DObject_StartStand(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DObject_SetPos(object, pos);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 0);
}

void Field2DObject_StartTurn(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DObject_SetPos(object, pos);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 1);
    Field2DObject_SetParam(object, 8, 0);
    Field2DObject_SetParam(object, 9, 2);
}

void Field2DObject_StartWalk(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DPoint targetPos = Field2D_GetStepTarget(pos, direction);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetPos(object, targetPos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 2);
    Field2DObject_SetParam(object, 8, 0);
    Field2DObject_SetParam(object, 9, 8);
}

void Field2DObject_StartRun(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DPoint targetPos = Field2D_GetStepTarget(pos, direction);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetPos(object, targetPos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 3);
    Field2DObject_SetParam(object, 8, 0);
    Field2DObject_SetParam(object, 9, 4);
}

void Field2DObject_StartPause(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DObject_SetPos(object, pos);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 4);
}

void Field2DObject_StartBump(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DObject_SetPos(object, pos);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 5);
    Field2DObject_SetParam(object, 8, 0);
    Field2DObject_SetParam(object, 9, 16);
}

void Field2DObject_StartWalkSlow(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DPoint targetPos = Field2D_GetStepTarget(pos, direction);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetPos(object, targetPos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 6);
    Field2DObject_SetParam(object, 8, 0);
    Field2DObject_SetParam(object, 9, 16);
}

void Field2DObject_StartWalkFast(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DPoint targetPos = Field2D_GetStepTarget(pos, direction);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetPos(object, targetPos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 7);
    Field2DObject_SetParam(object, 8, 0);
    Field2DObject_SetParam(object, 9, 4);
}

void Field2DObject_StartWalkInPlaceVeryFast(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetPos(object, pos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 8);
    Field2DObject_SetParam(object, 8, 0);
    Field2DObject_SetParam(object, 9, 2);
}

void Field2DObject_StartWalkInPlaceFast(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetPos(object, pos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 9);
    Field2DObject_SetParam(object, 8, 0);
    Field2DObject_SetParam(object, 9, 4);
}

void Field2DObject_StartWalkInPlace(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetPos(object, pos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 10);
    Field2DObject_SetParam(object, 8, 0);
    Field2DObject_SetParam(object, 9, 8);
}

void Field2DObject_StartWalkInPlaceSlow(Field2DObject *object, Field2DPoint pos, u32 direction) {
    GF_ASSERT(object);
    Field2DObject_SetPrevPos(object, pos);
    Field2DObject_SetPos(object, pos);
    Field2DObject_SetParam(object, 6, direction);
    Field2DObject_SetParam(object, 5, 11);
    Field2DObject_SetParam(object, 8, 0);
    Field2DObject_SetParam(object, 9, 16);
}

s32 Field2DObject_UpdateStand(Field2DObject *object) {
    return 0;
}

s32 Field2DObject_UpdateTurn(Field2DObject *object) {
    return Field2DObject_AdvanceActionFrame(object);
}

s32 Field2DObject_UpdateWalk(Field2DObject *object) {
    s32 done = Field2DObject_AdvanceActionFrame(object);
    if (done == 1) {
        Field2DPoint pos = Field2DObject_GetPos(object);
        Field2DObject_SetPrevPos(object, pos);
    }
    return done;
}

s32 Field2DObject_UpdateRun(Field2DObject *object) {
    s32 done = Field2DObject_AdvanceActionFrame(object);
    if (done == 1) {
        Field2DPoint pos = Field2DObject_GetPos(object);
        Field2DObject_SetPrevPos(object, pos);
    }
    return done;
}

s32 Field2DObject_UpdatePause(Field2DObject *object) {
    return 0;
}

s32 Field2DObject_UpdateBump(Field2DObject *object) {
    return Field2DObject_AdvanceActionFrame(object);
}

s32 Field2DObject_UpdateWalkInPlace(Field2DObject *object) {
    return Field2DObject_AdvanceActionFrame(object);
}

Field2DPoint Field2D_GetStepTarget(Field2DPoint pos, s32 direction) {
    return Field2D_StepInDirection(pos, direction);
}

s32 Field2DObject_AdvanceActionFrame(Field2DObject *object) {
    s32 actionFrame = Field2DObject_GetParam(object, 8);
    s32 actionDuration = Field2DObject_GetParam(object, 9);
    if (actionFrame < actionDuration) {
        Field2DObject_SetParam(object, 8, actionFrame + 1);
        if ((actionFrame + 1) < actionDuration) {
            return 0;
        }
    }
    return 1;
}

void Field2DObject_EndAction(Field2DObject *object) {
    Field2DPoint pos = Field2DObject_GetPos(object);
    u32 direction = Field2DObject_GetParam(object, 6);
    Field2DObject_StartStand(object, pos, direction);
}

s32 Field2DObjectManager_TryCommand(Field2DMap *map, Field2DObjectManager *objectManager, Field2DCommand *command, Field2DMove *move) {
    GF_ASSERT(map);
    GF_ASSERT(objectManager);
    GF_ASSERT(command);
    GF_ASSERT(move);
    Field2DObject *object = Field2DObjectManager_GetById(objectManager, command->unk3);
    GF_ASSERT(object);
    return sField2DCommandCheckFuncs[command->unk0](object, map, objectManager, command, move);
}
