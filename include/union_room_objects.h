#ifndef POKEHEARTGOLD_UNION_ROOM_OBJECTS_H
#define POKEHEARTGOLD_UNION_ROOM_OBJECTS_H

#include "mail_message.h"
#include "player_avatar.h"
#include "pm_string.h"
#include "save_palpad.h"
#include "script.h"
#include "sys_task_api.h"
#include "unk_0206793C.h"

// One union room map object: slots 0-9 are the other players (object ids 1-10), slots 10-49 the members of their
// groups (4 per player, first slot from a table), slot 50 is the player's own avatar.
typedef struct UnionRoomSlot {
    u8 request;      // 0x00, 1 = appear alone, 2 = appear in a group, 3 = leave
    u8 state;        // 0x01, 0 hidden, 1 appearing, 2 shown, 3 leaving, 4 left
    u8 friendship;   // 0x02, Pal Pad friend level, shown as an effect when the object appears
    u8 emoteRequest; // 0x03
    u8 emoteActive;  // 0x04
    u8 unk_05;
    u16 emoteTimer; // 0x06
    u8 sprite;      // 0x08
    u8 isWandering; // 0x09, set when a lone player starts walking around
    u8 unk_0A[2];
    u32 id;                           // 0x0C, the player record's id when the object appeared
    UnkStruct_0206793C *emote;        // 0x10
    UnkStruct_0206793C *friendEffect; // 0x14
} UnionRoomSlot;

typedef struct UnionRoomMessage {
    String *name;
    String *string1;
    String *string2;
    u32 unkC;
    u32 unk10;
    MailMessage mail;
} UnionRoomMessage;

typedef struct UnionRoomObjects {
    struct UnionRoom *unionRoom;
    SysTask *task;
    PlayerAvatar *avatar;
    UnionRoomSlot slots[51];
    FieldSystem *fieldSystem;
    SavePalPad *palPad;
    UnionRoomMessage *messages; // 30 entries + 2 words
    u32 enabled;
    u32 unk4E4;
} UnionRoomObjects;

UnionRoomObjects *UnionRoomObjects_New(struct UnionRoom *unionRoom);
void UnionRoomObjects_RequestLeaveAll(UnionRoomObjects *objects);
void UnionRoomObjects_Delete(UnionRoomObjects *objects);
void UnionRoomObjects_RestoreAfterScript(MapObjectManager *mapObjectManager, UnionRoomObjects *objects);

#endif // POKEHEARTGOLD_UNION_ROOM_OBJECTS_H
