#ifndef POKEHEARTGOLD_FIELD_PLAYER_EFFECT_INTERNAL_H
#define POKEHEARTGOLD_FIELD_PLAYER_EFFECT_INTERNAL_H
#include "global.h"

#include "heap.h"
#include "mail_message.h"
#include "map_object.h"
#include "player_data.h"
#include "pm_string.h"
#include "save.h"
#include "save_palpad.h"
#include "sys_task_api.h"
#include "task.h"
#include "sound_playback.h"
#include "map_object_movement_commands.h"
#include "unk_0206793C.h"
#include "field_effects.h"

typedef struct {
    u8 unk0;
    u8 state;
    u8 effect;
    u8 trigger;
    u8 active;
    u8 pad5;
    u16 timer;
    u8 sprite;
    u8 pad9[3];
    void *unkC;
    struct UnkStruct_0206793C *effect1;
    struct UnkStruct_0206793C *effect2;
} Slot;

typedef struct {
    u8 padC[12];
    u8 unk0;
    u8 state;
    u8 effect;
    u8 trigger;
    u8 active;
    u8 pad11;
    u16 timer;
    u8 sprite;
    u8 pad15[3];
    void *unkC;
    struct UnkStruct_0206793C *effect1;
    struct UnkStruct_0206793C *effect2;
} SlotCursor;

typedef struct {
    String *string0;
    String *string1;
    String *string2;
    u32 unkC;
    u32 unk10;
    MailMessage message;
} MessageSlot;

typedef struct {
    void *fieldSystem;
    SysTask *task;
    LocalMapObject *avatar;
    Slot slots[51];
    void *remote;
    void *palPad;
    MessageSlot *messages;
    u32 enabled;
    u32 pad4E4;
} Manager;

typedef struct {
    u8 pad0[0xC];
    SaveData *saveData;
    u8 pad10[0x2C];
    MapObjectManager *objectManager;
    LocalMapObject *avatar;
} RemoteState;

extern void GF_AssertFail(void);
extern void MapObject_ChangeMovement(LocalMapObject *, u32);
extern void MapObject_ChangeSprite(LocalMapObject *, u32);
extern void *FieldShadow_CreateFriendShadow(LocalMapObject *, int);
extern void *UnionChatMarker_Spawn(LocalMapObject *);
extern BOOL CommState_IsAlternateLinked(void);
extern BOOL CommState_GetJoinResult(void);
extern RemoteState *UnionRoom_GetFieldSystem(void *);
extern void *UnionRoom_GetRecord(void *, int);
extern void UnionRoomObjects_UpdatePlayers(Manager *, void *, MapObjectManager *, void *);
extern void UnionRoomObjects_UpdateMapObjects(Manager *, MapObjectManager *);
extern BOOL UnionRoomObjects_UpdateGroup(Manager *, int, void *, void *);
extern const u16 gUnionRoomGroupFirstSlot[];

void UnionRoomSlot_DestroyEffects(Slot *, BOOL);
void UnionRoomSlot_TickEmote(Slot *);
void UnionRoomSlot_Appear(Slot *, LocalMapObject *, u32, u32);
void UnionRoomSlot_Leave(Slot *, LocalMapObject *);
void UnionRoomSlot_StartEmote(Slot *, LocalMapObject *);
void UnionRoomObjects_ShowRange(MapObjectManager *, int, int);
void UnionRoomSlots_SetRequest(u8 *, int, u8);
void UnionRoomMessage_Init(MessageSlot *);
void UnionRoomMessages_Init(MessageSlot *);
void UnionRoomMessage_Free(MessageSlot *);
void UnionRoomMessages_FreeAll(MessageSlot *);
MessageSlot *UnionRoomMessages_New(int);
void UnionRoomMessages_Free(MessageSlot *);
void UnionRoomObjects_RequestLeaveAll(Manager *);
void UnionRoomObjects_ResetSlots(Manager *);
void UnionRoomObjects_Delete(Manager *);
void UnionRoomObjects_RestoreAfterScript(MapObjectManager *, Manager *);
void UnionRoomObjects_Task(SysTask *, void *);
Manager *UnionRoomObjects_New(void *);

Manager *UnionRoomObjects_New(void *fieldSystem);
BOOL UnionRoomObjects_UpdateGroup(Manager *manager, int group, void *recordArg, void *palPad);
void UnionRoomObjects_UpdatePlayers(Manager *manager, void *fieldSystem, MapObjectManager *objectManager, void *palPad);
void UnionRoomObjects_RequestLeaveAll(Manager *manager);
void UnionRoomObjects_ResetSlots(Manager *manager);
void UnionRoomObjects_Delete(Manager *manager);
void UnionRoomObjects_Task(SysTask *task, void *data);
void UnionRoomSlot_DestroyEffects(Slot *slot, BOOL both);
void UnionRoomSlot_TickEmote(Slot *slot);
void UnionRoomSlot_Appear(Slot *slot, LocalMapObject *object, u32 x, u32 z);
void UnionRoomSlot_Leave(Slot *slot, LocalMapObject *object);
void UnionRoomSlot_StartEmote(Slot *slot, LocalMapObject *object);
void UnionRoomObjects_ShowRange(MapObjectManager *manager, int first, int last);
void UnionRoomObjects_RestoreAfterScript(MapObjectManager *objectManager, Manager *manager);
void UnionRoomSlots_SetRequest(u8 *slots, int index, u8 value);
void UnionRoomMessage_Init(MessageSlot *slot);
void UnionRoomMessages_Init(MessageSlot *slots);
void UnionRoomMessage_Free(MessageSlot *slot);
void UnionRoomMessages_FreeAll(MessageSlot *slots);
MessageSlot *UnionRoomMessages_New(int heapId);
void UnionRoomMessages_Free(MessageSlot *slots);

#endif
