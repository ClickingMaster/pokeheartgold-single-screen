#include <string.h>

#include "global.h"

#include "field/field_follow_mon.h"
#include "field/overlay_01_021FB878.h"

#include "field_move_environment.h"
#include "field_system.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "task.h"

// The Sweet Scent field move: the user's (or the following Pokemon's) animation, then Task_HoneyOrSweetScent.

// Replaces the FieldMoveData in the task environment (partySlot carried over).
typedef struct SweetScentFieldEnv {
    void *effect;    // the field move effect of the user (overlay 02)
    void *honeyWork; // GetHoneySweetScentWorkSize() block for Task_HoneyOrSweetScent
    u32 partySlot;
} SweetScentFieldEnv;

// The scent model played at the following Pokemon.
typedef struct SweetScentAnimWork {
    NNSFndAllocator allocator;
    Field3dModel model;
    Field3DModelAnimation anims[3];
    Field3dObject object;
} SweetScentAnimWork;

BOOL Task_UseSweetScentInField(TaskManager *taskManager);
void FieldSystem_StartSweetScentFollowMonAnim(FieldSystem *fieldSystem);
BOOL Task_SweetScentFollowMonAnim(TaskManager *taskManager);
void SweetScentFollowMonAnim_Load(enum HeapID heapID, FieldSystem *fieldSystem, SweetScentAnimWork *work);
void SweetScentFollowMonAnim_Unload(SweetScentAnimWork *work);
BOOL SweetScentFollowMonAnims_Advance(Field3DModelAnimation *anims, u32 count);
void SweetScentFollowMonAnims_SetFrame(Field3DModelAnimation *anims, u32 count, fx32 frame);

BOOL Task_UseSweetScentInField(TaskManager *taskManager) {
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FieldMoveEnvironment *env = TaskManager_GetEnvironment(taskManager);
    u32 *state = TaskManager_GetStatePtr(taskManager);
    SweetScentFieldEnv *work = (SweetScentFieldEnv *)env->moveData;

    switch (*state) {
    case 0: {
        u32 partySlot = ((FieldMoveData *)work)->partySlot;
        Heap_Free(work);
        work = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(SweetScentFieldEnv));
        env->moveData = (FieldMoveData *)work;
        work->partySlot = partySlot;
        ++*state;
        break;
    }
    case 1: {
        u32 size = GetHoneySweetScentWorkSize();
        work->honeyWork = Heap_AllocAtEnd(HEAP_ID_FIELD2, size);
        memset(work->honeyWork, 0, size);
        {
            int avatarState = PlayerAvatar_GetState(fieldSystem->playerAvatar);
            if (avatarState == 1 || avatarState == 2) { // walking or biking?
                ++*state;
                break;
            }
        }
        if (work->partySlot == FieldSystem_GetLeadMonIndex(fieldSystem) && FollowMon_IsVisible(fieldSystem)) {
            TaskManager_Call(taskManager, Task_WaitFollowMonMovement, NULL);
            *state = 4;
            break;
        }
        ++*state;
        break;
    }
    case 2:
        work->effect = FieldMoveCutIn_Start(fieldSystem, 0, env->mon, PlayerAvatar_GetGender(fieldSystem->playerAvatar));
        ++*state;
        break;
    case 3:
        if (FieldMoveCutIn_IsFinished(work->effect)) {
            FieldMoveCutIn_Delete(work->effect);
            *state = 6;
        }
        break;
    case 4: {
        u8 kind;
        if (FieldSystem_FirstAliveMonHasType(fieldSystem, 12)) {
            kind = 2;
            FieldSystem_UnkSub108_AddMonMood(fieldSystem->unk108, 1);
        } else {
            kind = 1;
        }
        ov02_022507B4(fieldSystem, kind);
        ++*state;
        break;
    }
    case 5:
        FieldSystem_StartSweetScentFollowMonAnim(fieldSystem);
        ++*state;
        break;
    case 6:
        TaskManager_Call(taskManager, Task_HoneyOrSweetScent, work->honeyWork);
        ++*state;
        break;
    case 7:
        Heap_Free(work);
        Heap_Free(env);
        return TRUE;
    }
    return FALSE;
}

void FieldSystem_StartSweetScentFollowMonAnim(FieldSystem *fieldSystem) {
    SweetScentAnimWork *work = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(SweetScentAnimWork));
    SweetScentFollowMonAnim_Load(HEAP_ID_FIELD1, fieldSystem, work);
    TaskManager_Call(fieldSystem->taskman, Task_SweetScentFollowMonAnim, work);
}

BOOL Task_SweetScentFollowMonAnim(TaskManager *taskManager) {
    SweetScentAnimWork *work = TaskManager_GetEnvironment(taskManager);
    u32 *state = TaskManager_GetStatePtr(taskManager);

    switch (*state) {
    case 0:
        if (SweetScentFollowMonAnims_Advance(work->anims, 3)) {
            ++*state;
        }
        Field3dObject_Draw(&work->object);
        break;
    case 1:
        SweetScentFollowMonAnim_Unload(work);
        Heap_Free(work);
        return TRUE;
    }
    return FALSE;
}

void SweetScentFollowMonAnim_Load(enum HeapID heapID, FieldSystem *fieldSystem, SweetScentAnimWork *work) {
    VecFx32 position;

    HeapExp_FndInitAllocator(&work->allocator, heapID, 0x20);
    Field3dModel_LoadFromFilesystem(&work->model, NARC_a_1_3_4, 23, heapID);
    Field3dModelAnimation_LoadFromFilesystem(&work->anims[0], &work->model, NARC_a_1_3_4, 21, heapID, &work->allocator);
    Field3dModelAnimation_LoadFromFilesystem(&work->anims[1], &work->model, NARC_a_1_3_4, 22, heapID, &work->allocator);
    Field3dModelAnimation_LoadFromFilesystem(&work->anims[2], &work->model, NARC_a_1_3_4, 20, heapID, &work->allocator);
    Field3dObject_InitFromModel(&work->object, &work->model);
    Field3dObject_AddAnimation(&work->object, &work->anims[0]);
    Field3dObject_AddAnimation(&work->object, &work->anims[1]);
    Field3dObject_AddAnimation(&work->object, &work->anims[2]);
    SweetScentFollowMonAnims_SetFrame(work->anims, 3, 0);
    MapObject_CopyPositionVector(FollowMon_GetMapObject(fieldSystem), &position);
    Field3dObject_SetPosEx(&work->object, position.x, position.y, position.z);
    Field3dObject_SetActiveFlag(&work->object, TRUE);
}

void SweetScentFollowMonAnim_Unload(SweetScentAnimWork *work) {
    Field3dModelAnimation_Unload(&work->anims[2], &work->allocator);
    Field3dModelAnimation_Unload(&work->anims[1], &work->allocator);
    Field3dModelAnimation_Unload(&work->anims[0], &work->allocator);
    Field3dModel_Unload(&work->model);
}

BOOL SweetScentFollowMonAnims_Advance(Field3DModelAnimation *anims, u32 count) {
    u8 i;
    u8 numDone = 0;

    for (i = 0; i < count; ++i) {
        if (Field3dModelAnimation_FrameAdvanceAndCheck(&anims[i], FX32_ONE)) {
            ++numDone;
        }
    }
    return numDone == count;
}

void SweetScentFollowMonAnims_SetFrame(Field3DModelAnimation *anims, u32 count, fx32 frame) {
    u8 i;

    for (i = 0; i < count; ++i) {
        Field3dModelAnimation_FrameSet(&anims[i], frame);
    }
}
