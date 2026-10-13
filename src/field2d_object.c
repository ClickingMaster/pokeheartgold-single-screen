#include "overlay_42.h"

const s8 sField2DDirectionOffsets[8] = { 0, 240, 0, 16, 240, 0, 16, 0 };
const u8 sField2DOppositeDirections[4] = { 1, 0, 3, 2 };

const Field2DObjectStartFunc sField2DObjectStartFuncs[] = { Field2DObject_StartStand, Field2DObject_StartTurn, Field2DObject_StartWalk, Field2DObject_StartRun, Field2DObject_StartPause, Field2DObject_StartBump, Field2DObject_StartWalkSlow, Field2DObject_StartWalkFast, Field2DObject_StartWalkInPlaceVeryFast, Field2DObject_StartWalkInPlaceFast, Field2DObject_StartWalkInPlace, Field2DObject_StartWalkInPlaceSlow };

const Field2DObjectUpdateFunc sField2DObjectUpdateFuncs[] = { Field2DObject_UpdateStand, Field2DObject_UpdateTurn, Field2DObject_UpdateWalk, Field2DObject_UpdateRun, Field2DObject_UpdatePause, Field2DObject_UpdateBump, Field2DObject_UpdateWalk, Field2DObject_UpdateWalk, Field2DObject_UpdateWalkInPlace, Field2DObject_UpdateWalkInPlace, Field2DObject_UpdateWalkInPlace, Field2DObject_UpdateWalkInPlace };

const Field2DCommandCheckFunc sField2DCommandCheckFuncs[] = { Field2DCommand_CheckStand, Field2DCommand_CheckTurn, Field2DCommand_CheckWalk, Field2DCommand_CheckRun, Field2DCommand_CheckPause, Field2DCommand_CheckBump, Field2DCommand_CheckWalkSlow, Field2DCommand_CheckWalk, Field2DCommand_CheckWalkInPlace, Field2DCommand_CheckWalkInPlace, Field2DCommand_CheckWalkInPlace, Field2DCommand_CheckWalkInPlace };

s32 Field2DMap_IsBlocked(Field2DMap *arg0, u16 arg1, u16 arg2) {
    GF_ASSERT(arg0);
    Field2DMapCell temp_r0 = Field2DMap_GetCell(arg0, arg1, arg2);
    if (temp_r0.value == -1) {
        return 1;
    }
    return (u32)((s32)temp_r0.value << 31) >> 31;
}

u32 Field2DMap_GetTag(Field2DMap *arg0, u16 arg1, u16 arg2) {
    GF_ASSERT(arg0);
    Field2DMapCell sp0 = Field2DMap_GetCell(arg0, arg1, arg2);
    if (sp0.value == -1) {
        return 0;
    }
    return sp0.value / 2;
}

Field2DObjectManager *Field2DObjectManager_New(u32 arg0, enum HeapID arg1) {
    Field2DObjectManager *temp_r0 = Heap_Alloc(arg1, sizeof(Field2DObjectManager));
    GF_ASSERT(temp_r0);
    temp_r0->capacity = arg0;
    Field2DObject *temp_r0_2 = Heap_Alloc(arg1, arg0 * sizeof(Field2DObject));
    temp_r0->objects = temp_r0_2;
    GF_ASSERT(temp_r0_2);
    memset(temp_r0->objects, 0, temp_r0->capacity * sizeof(Field2DObject));
    return temp_r0;
}

void Field2DObjectManager_Free(Field2DObjectManager *arg0) {
    GF_ASSERT(arg0);
    Heap_Free(arg0->objects);
    Heap_Free(arg0);
}

void Field2DObjectManager_StartMove(Field2DObjectManager *arg0, Field2DMove *arg1) {
    Field2DObject_StartMove(Field2DObjectManager_FindById(arg0, arg1->unk7), arg1);
}

void Field2DObjectManager_Update(Field2DObjectManager *arg0) {
    int temp = Field2DObjectManager_GetCapacity(arg0);
    for (u32 i = 0; i < temp; i++) {
        Field2DObject *temp_r0 = Field2DObjectManager_GetSlot(arg0, i);
        if (temp_r0 != NULL) {
            Field2DObject_Update(temp_r0);
        }
    }
}

u32 Field2DObjectManager_GetCapacity(Field2DObjectManager *arg0) {
    GF_ASSERT(arg0);
    return arg0->capacity;
}

Field2DObject *Field2DObjectManager_AddObject(Field2DObjectManager *arg0, Field2DObjectTemplate *arg1) {
    GF_ASSERT(arg0);
    GF_ASSERT(arg1);
    Field2DObject *temp_r0;
    temp_r0 = Field2DObjectManager_GetFreeSlot(arg0);
    temp_r0->pos.unk0 = arg1->unk0;
    temp_r0->pos.unk2 = arg1->unk2;
    temp_r0->prevPos.unk0 = arg1->unk0;
    temp_r0->prevPos.unk2 = arg1->unk2;
    temp_r0->id = arg1->unk4;
    temp_r0->action = arg1->unk6;
    temp_r0->direction = arg1->unk8;
    temp_r0->spriteId = arg1->unkA;
    temp_r0->active = 1;
    return temp_r0;
}

void Field2DObject_Delete(void *arg0) {
    memset(arg0, 0, 24);
}

Field2DObject *Field2DObjectManager_FindById(Field2DObjectManager *arg0, u8 arg1) {
    for (int i = 0; i < arg0->capacity; i++) {
        if ((arg0->objects[i].active == 1) && (arg1 == arg0->objects[i].id)) {
            return &arg0->objects[i];
        }
    }
    return NULL;
}

Field2DObject *Field2DObjectManager_GetSlot(Field2DObjectManager *arg0, u16 arg1) {
    GF_ASSERT(arg0);
    GF_ASSERT(arg1 < arg0->capacity);
    if (arg0->objects[arg1].active == 1) {
        return &arg0->objects[arg1];
    }
    return 0;
}

Field2DObject *Field2DObjectManager_GetById(Field2DObjectManager *arg0, u8 arg1) {
    return Field2DObjectManager_FindById(arg0, arg1);
}

Field2DObject *Field2DObjectManager_GetByIndex(Field2DObjectManager *arg0, u16 arg1) {
    return Field2DObjectManager_GetSlot(arg0, arg1);
}

s32 Field2DObject_GetParam(Field2DObject *arg0, s32 arg1) {
    s32 var_r5;
    GF_ASSERT(arg0);
    switch (arg1) {
    case 0:
        var_r5 = arg0->pos.unk0;
        break;
    case 1:
        var_r5 = arg0->pos.unk2;
        break;
    case 2:
        var_r5 = arg0->prevPos.unk0;
        break;
    case 3:
        var_r5 = arg0->prevPos.unk2;
        break;
    case 4:
        var_r5 = arg0->id;
        break;
    case 5:
        var_r5 = arg0->action;
        break;
    case 6:
        var_r5 = arg0->direction;
        break;
    case 7:
        var_r5 = arg0->spriteId;
        break;
    case 8:
        var_r5 = arg0->actionFrame;
        break;
    case 9:
        var_r5 = arg0->actionDuration;
        break;
    case 10:
        var_r5 = arg0->stateFrames;
        break;
    default:
        GF_ASSERT(FALSE);
    }
    return var_r5;
}

void Field2DObject_SetParam(Field2DObject *arg0, u32 arg1, s32 arg2) {
    GF_ASSERT(arg0);
    switch (arg1) {
    case 0:
        arg0->pos.unk0 = arg2;
        return;
    case 1:
        arg0->pos.unk2 = arg2;
        return;
    case 2:
        arg0->prevPos.unk0 = arg2;
        return;
    case 3:
        arg0->prevPos.unk2 = arg2;
        return;
    case 4:
        arg0->id = arg2;
        return;
    case 5:
        GF_ASSERT(arg2 < 12);
        arg0->action = arg2;
        return;
    case 6:
        GF_ASSERT(arg2 < 4);
        arg0->direction = arg2;
        return;
    case 7:
        arg0->spriteId = arg2;
        return;
    case 8:
        arg0->actionFrame = arg2;
        return;
    case 9:
        arg0->actionDuration = arg2;
        return;
    case 10:
        arg0->stateFrames = arg2;
        return;
    }
    GF_ASSERT(FALSE);
}

Field2DPoint Field2D_StepInDirection(Field2DPoint arg0, s32 arg1) {
    s32 temp_r0 = arg1 * 2;
    arg0.unk0 = arg0.unk0 + sField2DDirectionOffsets[arg1 * 2];
    arg0.unk2 = arg0.unk2 + sField2DDirectionOffsets[arg1 * 2 + 1];

    return arg0;
}

u8 Field2D_GetOppositeDirection(s32 arg0) {
    return sField2DOppositeDirections[arg0];
}

Field2DObject *Field2DObjectManager_GetFreeSlot(Field2DObjectManager *arg0) {
    for (int i = 0; i < arg0->capacity; i++) {
        if (arg0->objects[i].active == 0) {
            return &arg0->objects[i];
        }
    }
    GF_ASSERT(FALSE);
    return NULL;
}

Field2DPoint Field2DObject_GetPos(Field2DObject *arg0) {
    return arg0->pos;
}

Field2DPoint Field2DObject_GetPrevPos(Field2DObject *arg0) {
    return arg0->prevPos;
}

Field2DPoint Field2DObject_GetDrawPos(Field2DObject *arg0) {
    s32 temp_r4 = Field2DObject_GetParam(arg0, 8);
    s32 temp_r6 = Field2DObject_GetParam(arg0, 9);

    Field2DPoint temp_r0 = Field2DObject_GetPos(arg0);
    Field2DPoint temp_r0_2 = Field2DObject_GetPrevPos(arg0);

    temp_r0.unk0 -= temp_r0_2.unk0;
    temp_r0.unk2 -= temp_r0_2.unk2;
    if (temp_r4 > 0) {
        temp_r0.unk0 = temp_r4 * temp_r0.unk0 / temp_r6;
        temp_r0.unk2 = temp_r4 * temp_r0.unk2 / temp_r6;
    } else {
        temp_r0.unk0 = 0;
        temp_r0.unk2 = 0;
    }
    temp_r0.unk0 += temp_r0_2.unk0;
    temp_r0.unk2 += temp_r0_2.unk2;
    return temp_r0;
}

void Field2DObject_SetPos(Field2DObject *arg0, Field2DPoint arg1) {
    arg0->pos = arg1;
}

void Field2DObject_SetPrevPos(Field2DObject *arg0, Field2DPoint arg1) {
    arg0->prevPos = arg1;
}

Field2DObject *Field2DObject_FindObjectInDirection(Field2DObject *arg0, Field2DObjectManager *arg1, s32 arg2) {
    u32 temp_r0_4;
    u32 sp4;

    Field2DPoint temp_r0_3 = Field2DObject_GetPos(arg0);
    temp_r0_3 = Field2D_StepInDirection(temp_r0_3, arg2);
    sp4 = Field2DObject_GetParam(arg0, 4);
    temp_r0_4 = Field2DObjectManager_GetCapacity(arg1);
    for (u32 i = 0; i < temp_r0_4; i++) {
        Field2DObject *temp_r0 = Field2DObjectManager_GetByIndex(arg1, i);
        if (temp_r0 != 0) {
            u32 r1 = Field2DObject_GetParam(temp_r0, 4);
            if (r1 != sp4) {
                Field2DPoint temp_r0_5 = Field2DObject_GetPos(temp_r0);
                if ((temp_r0_5.unk0 == temp_r0_3.unk0) && (temp_r0_5.unk2 == temp_r0_3.unk2)) {
                    return temp_r0;
                }
                temp_r0_5 = Field2DObject_GetPrevPos(temp_r0);
                if ((temp_r0_5.unk0 == temp_r0_3.unk0) && (temp_r0_5.unk2 == temp_r0_3.unk2)) {
                    return temp_r0;
                }
            }
        }
    }
    return NULL;
}

Field2DObject *Field2DObjectManager_FindObjectAt(Field2DObjectManager *arg0, Field2DPoint arg1) {
    u32 r0 = Field2DObjectManager_GetCapacity(arg0);
    for (u32 i = 0; i < r0; i++) {
        Field2DObject *temp = Field2DObjectManager_GetByIndex(arg0, i);
        if (temp != NULL) {
            Field2DPoint struct0 = Field2DObject_GetPos(temp);
            if ((struct0.unk0 == arg1.unk0) && (struct0.unk2 == arg1.unk2)) {
                return temp;
            }
            struct0 = Field2DObject_GetPrevPos(temp);
            if ((struct0.unk0 == arg1.unk0) && (struct0.unk2 == arg1.unk2)) {
                return temp;
            }
        }
    }
    return NULL;
}

void Field2DObject_StartMove(Field2DObject *arg0, Field2DMove *arg1) {
    sField2DObjectStartFuncs[arg1->unk4](arg0, arg1->unk0, arg1->unk6);
    arg0->stateFrames = 0;
}

void Field2DObject_Update(Field2DObject *arg0) {
    s32 temp_r0 = sField2DObjectUpdateFuncs[Field2DObject_GetParam(arg0, 5)](arg0);
    arg0->stateFrames++;
    if (temp_r0 == 1) {
        Field2DObject_EndAction(arg0);
        arg0->stateFrames = 0;
    }
}

void Field2DObject_StartStand(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DObject_SetPos(arg0, arg1);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 0);
}

void Field2DObject_StartTurn(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DObject_SetPos(arg0, arg1);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 1);
    Field2DObject_SetParam(arg0, 8, 0);
    Field2DObject_SetParam(arg0, 9, 2);
}

void Field2DObject_StartWalk(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DPoint temp_r0 = Field2D_GetStepTarget(arg1, arg2);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetPos(arg0, temp_r0);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 2);
    Field2DObject_SetParam(arg0, 8, 0);
    Field2DObject_SetParam(arg0, 9, 8);
}

void Field2DObject_StartRun(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DPoint temp_r0 = Field2D_GetStepTarget(arg1, arg2);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetPos(arg0, temp_r0);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 3);
    Field2DObject_SetParam(arg0, 8, 0);
    Field2DObject_SetParam(arg0, 9, 4);
}

void Field2DObject_StartPause(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DObject_SetPos(arg0, arg1);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 4);
}

void Field2DObject_StartBump(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DObject_SetPos(arg0, arg1);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 5);
    Field2DObject_SetParam(arg0, 8, 0);
    Field2DObject_SetParam(arg0, 9, 16);
}

void Field2DObject_StartWalkSlow(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DPoint temp_r0 = Field2D_GetStepTarget(arg1, arg2);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetPos(arg0, temp_r0);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 6);
    Field2DObject_SetParam(arg0, 8, 0);
    Field2DObject_SetParam(arg0, 9, 16);
}

void Field2DObject_StartWalkFast(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DPoint temp_r0 = Field2D_GetStepTarget(arg1, arg2);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetPos(arg0, temp_r0);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 7);
    Field2DObject_SetParam(arg0, 8, 0);
    Field2DObject_SetParam(arg0, 9, 4);
}

void Field2DObject_StartWalkInPlaceVeryFast(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetPos(arg0, arg1);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 8);
    Field2DObject_SetParam(arg0, 8, 0);
    Field2DObject_SetParam(arg0, 9, 2);
}

void Field2DObject_StartWalkInPlaceFast(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetPos(arg0, arg1);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 9);
    Field2DObject_SetParam(arg0, 8, 0);
    Field2DObject_SetParam(arg0, 9, 4);
}

void Field2DObject_StartWalkInPlace(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetPos(arg0, arg1);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 10);
    Field2DObject_SetParam(arg0, 8, 0);
    Field2DObject_SetParam(arg0, 9, 8);
}

void Field2DObject_StartWalkInPlaceSlow(Field2DObject *arg0, Field2DPoint arg1, u32 arg2) {
    GF_ASSERT(arg0);
    Field2DObject_SetPrevPos(arg0, arg1);
    Field2DObject_SetPos(arg0, arg1);
    Field2DObject_SetParam(arg0, 6, arg2);
    Field2DObject_SetParam(arg0, 5, 11);
    Field2DObject_SetParam(arg0, 8, 0);
    Field2DObject_SetParam(arg0, 9, 16);
}

s32 Field2DObject_UpdateStand(Field2DObject *arg0) {
    return 0;
}

s32 Field2DObject_UpdateTurn(Field2DObject *arg0) {
    return Field2DObject_AdvanceActionFrame(arg0);
}

s32 Field2DObject_UpdateWalk(Field2DObject *arg0) {
    s32 temp_r0 = Field2DObject_AdvanceActionFrame(arg0);
    if (temp_r0 == 1) {
        Field2DPoint temp_r0_2 = Field2DObject_GetPos(arg0);
        Field2DObject_SetPrevPos(arg0, temp_r0_2);
    }
    return temp_r0;
}

s32 Field2DObject_UpdateRun(Field2DObject *arg0) {
    s32 temp_r0 = Field2DObject_AdvanceActionFrame(arg0);
    if (temp_r0 == 1) {
        Field2DPoint temp_r0_2 = Field2DObject_GetPos(arg0);
        Field2DObject_SetPrevPos(arg0, temp_r0_2);
    }
    return temp_r0;
}

s32 Field2DObject_UpdatePause(Field2DObject *arg0) {
    return 0;
}

s32 Field2DObject_UpdateBump(Field2DObject *arg0) {
    return Field2DObject_AdvanceActionFrame(arg0);
}

s32 Field2DObject_UpdateWalkInPlace(Field2DObject *arg0) {
    return Field2DObject_AdvanceActionFrame(arg0);
}

Field2DPoint Field2D_GetStepTarget(Field2DPoint arg0, s32 arg1) {
    return Field2D_StepInDirection(arg0, arg1);
}

s32 Field2DObject_AdvanceActionFrame(Field2DObject *arg0) {
    s32 temp_r4 = Field2DObject_GetParam(arg0, 8);
    s32 temp_r0 = Field2DObject_GetParam(arg0, 9);
    if (temp_r4 < temp_r0) {
        Field2DObject_SetParam(arg0, 8, temp_r4 + 1);
        if ((temp_r4 + 1) < temp_r0) {
            return 0;
        }
    }
    return 1;
}

void Field2DObject_EndAction(Field2DObject *arg0) {
    Field2DPoint sp = Field2DObject_GetPos(arg0);
    u32 temp_r0 = Field2DObject_GetParam(arg0, 6);
    Field2DObject_StartStand(arg0, sp, temp_r0);
}

s32 Field2DObjectManager_TryCommand(Field2DMap *arg0, Field2DObjectManager *arg1, Field2DCommand *arg2, Field2DMove *arg3) {
    GF_ASSERT(arg0);
    GF_ASSERT(arg1);
    GF_ASSERT(arg2);
    GF_ASSERT(arg3);
    Field2DObject *temp_r0 = Field2DObjectManager_GetById(arg1, arg2->unk3);
    GF_ASSERT(temp_r0);
    return sField2DCommandCheckFuncs[arg2->unk0](temp_r0, arg0, arg1, arg2, arg3);
}
