#include "union_room_objects.h"

#include "global.h"

#include "assert.h"
#include "field_effects.h"
#include "field_system.h"
#include "heap.h"
#include "mail_message.h"
#include "map_object.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "save.h"
#include "save_palpad.h"
#include "sound_playback.h"
#include "sys_task_api.h"
#include "task.h"
#include "map_object_movement_commands.h"
#include "unk_0206793C.h"

// The 10 players' records of the union room link state (src/field_application_tasks_10.c).
typedef struct UnionRoomGroup {
    u32 trainerIds[4];
    u8 unk_10[8];
    u8 members[4]; // 0 = empty, else 0x80 | sprite
} UnionRoomGroup;

typedef struct UnionRoomPlayer {
    u32 id; // changes when the player is replaced
    u8 unk_04[0xC];
    PlayerProfile profile; // 0x10
    UnionRoomGroup group;  // 0x30
} UnionRoomPlayer;

typedef struct UnionRoomPlayerRecord {
    u8 unk_00[0x50];
    UnionRoomPlayer player; // 0x50
} UnionRoomPlayerRecord;

FieldSystem *UnionRoom_GetFieldSystem(struct UnionRoom *unionRoom);
UnionRoomPlayerRecord *UnionRoom_GetRecord(struct UnionRoom *unionRoom, int index);
s32 CommState_GetJoinResult(void);
s32 CommState_IsAlternateLinked(void);
UnkStruct_0206793C *FieldShadow_CreateFriendShadow(LocalMapObject *object, int index);
UnkStruct_0206793C *UnionChatMarker_Spawn(LocalMapObject *object);

extern const u16 gUnionRoomGroupFirstSlot[];

void UnionRoomObjects_ResetSlots(UnionRoomObjects *objects);
void UnionRoomObjects_Task(SysTask *task, void *data);
BOOL UnionRoomObjects_UpdateGroup(UnionRoomObjects *objects, int index, UnionRoomPlayerRecord *record, SavePalPad *palPad);
void UnionRoomSlots_SetRequest(UnionRoomSlot *slots, int index, u8 request);
void UnionRoomObjects_UpdatePlayers(UnionRoomObjects *objects, struct UnionRoom *unionRoom, MapObjectManager *mapObjectManager, SavePalPad *palPad);
void UnionRoomObjects_UpdateMapObjects(UnionRoomObjects *objects, MapObjectManager *mapObjectManager);
void UnionRoomSlot_DestroyEffects(UnionRoomSlot *slot, BOOL both);
void UnionRoomSlot_TickEmote(UnionRoomSlot *slot);
void UnionRoomSlot_Appear(UnionRoomSlot *slot, LocalMapObject *object, u32 x, u32 z);
void UnionRoomSlot_Leave(UnionRoomSlot *slot, LocalMapObject *object);
void UnionRoomSlot_StartEmote(UnionRoomSlot *slot, LocalMapObject *object);
void UnionRoomObjects_ShowRange(MapObjectManager *mapObjectManager, int first, int last);
void UnionRoomMessage_Init(UnionRoomMessage *message);
void UnionRoomMessages_Init(UnionRoomMessage *messages);
void UnionRoomMessage_Free(UnionRoomMessage *message);
void UnionRoomMessages_FreeAll(UnionRoomMessage *messages);
UnionRoomMessage *UnionRoomMessages_New(int heapID);
void UnionRoomMessages_Free(UnionRoomMessage *messages);

UnionRoomObjects *UnionRoomObjects_New(struct UnionRoom *unionRoom) {
    UnionRoomObjects *objects = Heap_Alloc((enum HeapID)0x1F, sizeof(UnionRoomObjects));
    MIi_CpuClearFast(0, (u32 *)objects, sizeof(UnionRoomObjects));
    objects->unionRoom = unionRoom;
    objects->enabled = 1;
    objects->task = SysTask_CreateOnMainQueue(UnionRoomObjects_Task, objects, 11);
    objects->fieldSystem = UnionRoom_GetFieldSystem(unionRoom);
    objects->palPad = SaveArray_Get(objects->fieldSystem->saveData, 8);
    objects->avatar = objects->fieldSystem->playerAvatar;
    Heap_CreateAtEnd((enum HeapID)11, (enum HeapID)0x57, 0x2710);
    objects->messages = UnionRoomMessages_New(0x57);
    UnionRoomObjects_ResetSlots(objects);
    return objects;
}

void UnionRoomObjects_RequestLeaveAll(UnionRoomObjects *objects) {
    int i;
    for (i = 0; i < 51; i++) {
        if (objects->slots[i].state != 0) {
            objects->slots[i].request = 3;
        }
    }
}

void UnionRoomObjects_ResetSlots(UnionRoomObjects *objects) {
    int i;
    for (i = 0; i < 51; i++) {
        objects->slots[i].request = 0;
        objects->slots[i].state = 0;
        objects->slots[i].friendship = 0;
        objects->slots[i].emoteRequest = 0;
    }
}

void UnionRoomObjects_Delete(UnionRoomObjects *objects) {
    SysTask_Destroy(objects->task);
    UnionRoomMessages_Free(objects->messages);
    Heap_Destroy((enum HeapID)0x57);
    Heap_Free(objects);
}

void UnionRoomObjects_Task(SysTask *task, void *data) {
    UnionRoomObjects *objects = data;
    struct UnionRoom *unionRoom = objects->unionRoom;

    if (!FieldSystem_TaskIsRunning(objects->fieldSystem)) {
        objects->avatar = objects->fieldSystem->playerAvatar;
        UnionRoomObjects_UpdatePlayers(objects, unionRoom, objects->fieldSystem->mapObjectManager, objects->palPad);
        UnionRoomObjects_UpdateMapObjects(objects, objects->fieldSystem->mapObjectManager);
    }
}

#pragma force_active on
#pragma opt_strength_reduction off

BOOL UnionRoomObjects_UpdateGroup(UnionRoomObjects *objects, int index, UnionRoomPlayerRecord *record, SavePalPad *palPad) {
    BOOL ret = FALSE;
    int i;
    int k;
    UnionRoomPlayer *player;
    UnionRoomGroup *group;
    u32 *trainerIds;

    if (record == NULL) {
        k = gUnionRoomGroupFirstSlot[index];
        for (i = 0; i < 4; i++) {
            UnionRoomSlots_SetRequest(objects->slots, k, 3);
            k++;
        }
        return FALSE;
    }
    player = &record->player;
    group = &player->group;
    if (player->id != objects->slots[index].id) {
        k = gUnionRoomGroupFirstSlot[index];
        for (i = 0; i < 4; i++) {
            UnionRoomSlots_SetRequest(objects->slots, k, 3);
            k++;
        }
        return FALSE;
    }
    k = gUnionRoomGroupFirstSlot[index];
    trainerIds = group->trainerIds;
    for (i = 0; i < 4; i++) {
        switch (objects->slots[k].state) {
        case 0:
            if (group->members[i] != 0) {
                objects->slots[k].request = 2;
                objects->slots[k].sprite = group->members[i] & 0x7F;
                objects->slots[k].friendship = PalPad_PlayerIdIsFriendOrMutual(palPad, *trainerIds);
                ret = TRUE;
            }
            break;
        case 2:
            if (group->members[i] == 0) {
                UnionRoomSlots_SetRequest(objects->slots, k, 3);
            } else {
                ret = TRUE;
            }
            break;
        case 4:
            objects->slots[k].request = 0;
            break;
        }
        k++;
        trainerIds++;
    }
    return ret;
}

#pragma opt_strength_reduction reset
#pragma force_active off

void UnionRoomSlots_SetRequest(UnionRoomSlot *slots, int index, u8 request) {
    slots[index].request = request;
}

void UnionRoomObjects_UpdatePlayers(UnionRoomObjects *objects, struct UnionRoom *unionRoom, MapObjectManager *mapObjectManager, SavePalPad *palPad) {
    int i;

    for (i = 0; i < 10; i++) {
        UnionRoomPlayerRecord *record = UnionRoom_GetRecord(unionRoom, i);
        UnionRoomPlayer *player;
        PlayerProfile *profile;
        if (record != NULL) {
            player = &record->player;
            profile = &player->profile;
        } else {
            player = NULL;
            profile = NULL;
        }

        switch (objects->slots[i].state) {
        case 0:
            if (record != NULL) {
                objects->slots[i].sprite = PlayerProfile_GetAvatar(profile);
                objects->slots[i].friendship = PalPad_PlayerIdIsFriendOrMutual(palPad, PlayerProfile_GetTrainerID(profile));
                objects->slots[i].id = player->id;
                if (UnionRoomObjects_UpdateGroup(objects, i, record, palPad)) {
                    objects->slots[i].request = 2;
                } else {
                    objects->slots[i].request = 1;
                }
            }
            break;
        case 2:
            if (record == NULL) {
                UnionRoomSlots_SetRequest(objects->slots, i, 3);
            } else if (player->id != objects->slots[i].id) {
                UnionRoomSlots_SetRequest(objects->slots, i, 3);
            }
            if (UnionRoomObjects_UpdateGroup(objects, i, record, palPad) && objects->slots[i].isWandering == 1) {
                objects->slots[i].request = 3;
            }
            break;
        case 4:
            objects->slots[i].request = 0;
            break;
        }
    }
}

void UnionRoomObjects_UpdateMapObjects(UnionRoomObjects *objects, MapObjectManager *mapObjectManager) {
    int i;
    u32 x;
    u32 z;
    UnionRoomSlot *slot;
    UnionRoomObjects *cursor; // steps one slot per iteration: cursor->slots[0] is slot i
    LocalMapObject *object;

    GF_ASSERT(objects->avatar != NULL);
    x = PlayerAvatar_GetXCoord(objects->avatar);
    z = PlayerAvatar_GetZCoord(objects->avatar);
    i = 0;
    cursor = objects;
    slot = objects->slots;

    for (; i < 50; i++, cursor = (UnionRoomObjects *)((u8 *)cursor + sizeof(UnionRoomSlot)), slot++) {
        object = MapObjectManager_GetFirstActiveObjectByID(mapObjectManager, i + 1);
        GF_ASSERT(object != NULL);
        switch (cursor->slots[0].state) {
        case 0:
            if (MapObject_AreBitsSetForMovementScriptInit(object) == TRUE && (u32)(cursor->slots[0].request - 1) <= 1) {
                UnionRoomSlot_Appear(slot, object, x, z);
            }
            break;
        case 1:
            if (MapObject_AreBitsSetForMovementScriptInit(object) == TRUE) {
                if (cursor->slots[0].request == 3) {
                    cursor->slots[0].state = 0;
                    cursor->slots[0].request = 0;
                    UnionRoomSlot_DestroyEffects(slot, TRUE);
                } else {
                    MapObject_ClearHeldMovementIfActive(object);
                    MapObject_SetFlag19(object, FALSE);
                    if (cursor->slots[0].request == 1 && cursor->slots[0].isWandering == 0) {
                        MapObject_ChangeMovement(object, 3);
                        MapObject_SetXRange(object, 1);
                        MapObject_SetYRange(object, 1);
                        cursor->slots[0].isWandering = 1;
                    }
                    cursor->slots[0].state = 2;
                    cursor->slots[0].request = 0;
                }
            }
            break;
        case 2:
            if (MapObject_AreBitsSetForMovementScriptInit(object) == TRUE) {
                UnionRoomSlot_StartEmote(slot, object);
                if (cursor->slots[0].request == 3) {
                    UnionRoomSlot_Leave(slot, object);
                }
                UnionRoomSlot_TickEmote(slot);
            }
            break;
        case 3:
            if (MapObject_AreBitsSetForMovementScriptInit(object) == TRUE) {
                MapObject_ClearHeldMovementIfActive(object);
                cursor->slots[0].state = 4;
                cursor->slots[0].request = 0;
                cursor->slots[0].isWandering = 0;
                MapObject_SetVisible(object, TRUE);
                MapObject_ClearFlag18(object, FALSE);
            }
            break;
        case 4:
            cursor->slots[0].state = 0;
            break;
        }
    }

    object = PlayerAvatar_GetMapObject(objects->avatar);
    UnionRoomSlot_StartEmote(&objects->slots[50], object);
    UnionRoomSlot_TickEmote(&objects->slots[50]);
}

void UnionRoomSlot_DestroyEffects(UnionRoomSlot *slot, BOOL both) {
    if (slot->emote != NULL) {
        if (FieldEffect_IsActive(slot->emote)) {
            FieldEffect_Destroy((int)slot->emote);
        }
        slot->emote = NULL;
    }
    if (both && slot->friendEffect != NULL) {
        if (FieldEffect_IsActive(slot->friendEffect)) {
            FieldEffect_Destroy((int)slot->friendEffect);
        }
        slot->friendEffect = NULL;
    }
}

void UnionRoomSlot_TickEmote(UnionRoomSlot *slot) {
    if (slot->emoteActive != 0) {
        slot->emoteTimer--;
        if (slot->emoteTimer == 0) {
            UnionRoomSlot_DestroyEffects(slot, FALSE);
            slot->emoteActive = 0;
        }
    }
}

void UnionRoomSlot_Appear(UnionRoomSlot *slot, LocalMapObject *object, u32 x, u32 z) {
    u32 initialX = MapObject_GetInitialX(object);
    u32 initialY = MapObject_GetInitialY(object);
    u32 initialZ = MapObject_GetInitialZ(object);

    if (initialX != x || initialZ != z) {
        PlaySE(0x64E);
        MapObject_ChangeSprite(object, slot->sprite);
        UnionRoomSlot_DestroyEffects(slot, FALSE);
        MapObject_SetPositionFromXYZAndDirection(object, initialX, initialY, initialZ, TRUE);
        MapObject_SetFacingDirectionDirect(object, TRUE);
        MapObject_SetHeldMovement(object, 0x44);
        MapObject_SetVisible(object, FALSE);
        MapObject_ClearFlag18(object, TRUE);
        slot->state = 1;
        if (slot->friendship != 0) {
            if (slot->friendship == 1) {
                slot->friendEffect = FieldShadow_CreateFriendShadow(object, 1);
            } else if (slot->friendship >= 2) {
                slot->friendEffect = FieldShadow_CreateFriendShadow(object, 2);
            }
            slot->friendship = 0;
        }
    }
}

void UnionRoomSlot_Leave(UnionRoomSlot *slot, LocalMapObject *object) {
    MapObject_SetHeldMovement(object, 0x43);
    MapObject_SetFlag19(object, TRUE);
    MapObject_ChangeMovement(object, 0);
    UnionRoomSlot_DestroyEffects(slot, TRUE);
    slot->emoteActive = 0;
    slot->emoteTimer = 0;
    slot->state = 3;
}

void UnionRoomSlot_StartEmote(UnionRoomSlot *slot, LocalMapObject *object) {
    if (slot->emoteRequest == 1 && slot->emoteActive == 0) {
        slot->emote = UnionChatMarker_Spawn(object);
        slot->emoteTimer = 30;
        slot->emoteRequest = 0;
        slot->emoteActive = 1;
    }
}

void UnionRoomObjects_ShowRange(MapObjectManager *mapObjectManager, int first, int last) {
    LocalMapObject *object;
    for (; first < last; first++) {
        object = MapObjectManager_GetFirstActiveObjectByID(mapObjectManager, first);
        GF_ASSERT(object != NULL);
        MapObject_SetVisible(object, TRUE);
        MapObject_ClearFlag18(object, FALSE);
        MapObject_SetFlag19(object, TRUE);
    }
}

void UnionRoomObjects_RestoreAfterScript(MapObjectManager *mapObjectManager, UnionRoomObjects *objects) {
    LocalMapObject *object;
    int i;

    object = MapObjectManager_GetFirstActiveObjectByID(mapObjectManager, 0);
    GF_ASSERT(object != NULL);
    if (MapObject_AreBitsSetForMovementScriptInit(object) == TRUE) {
        if (CommState_IsAlternateLinked() != FALSE || CommState_GetJoinResult() != FALSE) {
            for (i = 0; i < 10; i++) {
                UnionRoomSlot *slot = &objects->slots[i];
                if (slot->state == 1) {
                    object = MapObjectManager_GetFirstActiveObjectByID(mapObjectManager, i + 1);
                    GF_ASSERT(object != NULL);
                    MapObject_ChangeSprite(object, slot->sprite);
                    MapObject_SetFacingDirectionDirect(object, TRUE);
                    MapObject_SetHeldMovement(object, 0x44);
                    MapObject_SetVisible(object, FALSE);
                    MapObject_ClearFlag18(object, TRUE);
                    slot->state = 1;
                    if (slot->friendship != 0) {
                        if (slot->friendship == 1) {
                            slot->friendEffect = FieldShadow_CreateFriendShadow(object, 1);
                        } else if (slot->friendship >= 2) {
                            slot->friendEffect = FieldShadow_CreateFriendShadow(object, 2);
                        }
                        slot->friendship = 0;
                    }
                }
            }
            UnionRoomObjects_ShowRange(mapObjectManager, 11, 51);
        } else {
            UnionRoomObjects_ShowRange(mapObjectManager, 1, 51);
        }
    }
}

void UnionRoomMessage_Init(UnionRoomMessage *message) {
    message->name = String_New(8, (enum HeapID)0x57);
    message->string1 = NULL;
    message->string2 = NULL;
    MailMsg_Init_WithBank(&message->mail, 0);
    message->unk10 = 0;
    message->unkC = 0;
}

void UnionRoomMessages_Init(UnionRoomMessage *messages) {
    int i;
    for (i = 0; i < 30; i++) {
        UnionRoomMessage_Init(&messages[i]);
    }
    *(u32 *)&messages[30] = 0;
    *((u32 *)&messages[30] + 1) = 0;
}

void UnionRoomMessage_Free(UnionRoomMessage *message) {
    Heap_Free(message->name);
    if (message->string1 != NULL) {
        String_Delete(message->string1);
    }
    if (message->string2 != NULL) {
        String_Delete(message->string2);
    }
}

void UnionRoomMessages_FreeAll(UnionRoomMessage *messages) {
    int i;
    for (i = 0; i < 30; i++) {
        UnionRoomMessage_Free(&messages[i]);
    }
}

UnionRoomMessage *UnionRoomMessages_New(int heapID) {
    UnionRoomMessage *messages = Heap_Alloc((enum HeapID)heapID, 0x350);
    UnionRoomMessages_Init(messages);
    return messages;
}

void UnionRoomMessages_Free(UnionRoomMessage *messages) {
    UnionRoomMessages_FreeAll(messages);
    Heap_Free(messages);
}
