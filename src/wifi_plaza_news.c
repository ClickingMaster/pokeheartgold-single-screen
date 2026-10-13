#include "global.h"

#include "msgdata/msg.naix"

#include "message_format.h"
#include "msgdata.h"
#include "wifi_plaza_internal.h"

// The plaza news (WiFiPlazaWork.unk_04, shown by the plaza news app): the time the lobby opened, the state of every
// lobby slot and a list of events (topics) built from the lobby requests.

void WiFiPlazaNews_SetOpenTime(WiFiPlazaNews *news, u32 *openTime) {
    news->openTime = *openTime;
    news->isOpen = 1;
}

#pragma force_active on

void WiFiPlazaNews_AddPlayer(WiFiPlazaNews *news, int slot, int state, int flags) {
    WiFiPlazaNewsSlots_AddPlayer(&news->slots, slot, state, flags);
}

#pragma force_active reset

BOOL WiFiPlazaNews_RemovePlayer(WiFiPlazaNews *news, int slot) {
    BOOL result = WiFiPlazaNewsSlots_IsOldest(&news->slots, slot);
    WiFiPlazaNewsSlots_RemovePlayer(&news->slots, slot);
    return result;
}

// Moves flag 2 to the given lobby slot.
void WiFiPlazaNews_SetOldestPlayer(WiFiPlazaNews *news, int slot) {
    int i;
    for (i = 0; i < 20; i++) {
        u32 flags = WiFiPlazaNewsSlots_GetFlags(&news->slots, i);
        if (flags & 2) {
            WiFiPlazaNewsSlots_SetFlags(&news->slots, i, flags & ~2);
        }
    }
    WiFiPlazaNewsSlots_SetFlags(&news->slots, slot, WiFiPlazaNewsSlots_GetFlags(&news->slots, slot) | 2);
}

void WiFiPlazaNews_AddChatEvent(WiFiPlazaNews *news, WiFiPlazaPairRequest *req) {
    WiFiPlazaNewsEvent *event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, 8);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, 0, 0, 0, req->profiles[0], req->profiles[1], 0, 0, req->slots[0], req->slots[1], 0, 0, 600, 8, 0);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_AddTapToyTradeEvent(WiFiPlazaNews *news, WiFiPlazaPairOptionRequest *req) {
    WiFiPlazaNewsEvent *event;
    if (req->option >= 27) {
        return;
    }
    event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, 7);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, req->option, 0, 0, req->profiles[0], req->profiles[1], 0, 0, req->slots[0], req->slots[1], 0, 0, 600, 7, 1);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_AddMinigameEntryEvent(WiFiPlazaNews *news, WiFiPlazaGroupFlagRequest *req) {
    u32 type = req->option;
    u32 rank;
    WiFiPlazaNewsEvent *event;
    if (type != 0 && type != 1 && type != 2) {
        return;
    }
    if (req->flag == 1) {
        if (req->count < 2 || req->count > 4) {
            return;
        }
    } else if (req->count < 1 || req->count > 4) {
        return;
    }
    rank = ov45_02254BDC[type];
    event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, (u16)rank);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, type, req->count, req->flag, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 900, rank, 2);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_AddFootprintStampEvent(WiFiPlazaNews *news, WiFiPlazaSingleRequest *req) {
    WiFiPlazaNewsEvent *event;
    if (req->option != 3 && req->option != 4) {
        return;
    }
    event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, (u16)(req->option + 5));
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, req->option, req->count, 0, req->profile, 0, 0, 0, req->slot, 0, 0, 0, 900, req->option + 5, 3);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_AddVisitorProfilesEvent(WiFiPlazaNews *news, WiFiPlazaGroupRequest *req) {
    WiFiPlazaNewsEvent *event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, 12);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, req->count, 0, 0, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 450, 12, 4);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_AddPlazaNewsEvent(WiFiPlazaNews *news, WiFiPlazaGroupRequest *req) {
    WiFiPlazaNewsEvent *event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, 13);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, req->count, 0, 0, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 450, 13, 5);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_AddLobbyEventNotice(WiFiPlazaNews *news, WiFiPlazaLobbyNotice *notice) {
    WiFiPlazaNewsEvent *event;
    switch (notice->which) {
    case 3:
        return;
    case 0:
    case 1:
    case 2:
    case 4:
        break;
    default:
        return;
    }
    event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, 14);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, *notice->time, notice->which, 0, 0, 0, 0, 0, 0, 0, 0, 0, 900, 14, 6);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_AddSpecialVisitorEvent(WiFiPlazaNews *news, int *slot, const u8 *flags) {
    WiFiPlazaNewsEvent *event;
    if ((u32)*slot >= 20 || flags[*slot] == 0) {
        return;
    }
    event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, 1);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, *slot, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1800, 1, 7);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_AddMinigameResultEvent(WiFiPlazaNews *news, WiFiPlazaGroupOptionRequest *req) {
    u32 type;
    u32 rank;
    WiFiPlazaNewsEvent *event;
    if ((req->option != 0 && req->option != 1 && req->option != 2) || !WiFiPlaza_IsMinigamePlayerCountValid(*(volatile u32 *)&req->option, req->count)) {
        return;
    }
    type = req->option;
    rank = ov45_02254BC8[type];
    event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, rank);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, type, req->count, 0, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 450, rank, 8);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_RemoveCurrentEvent(WiFiPlazaNews *news) {
    WiFiPlazaNewsEventList_RemoveFirst(&news->events);
}

u32 WiFiPlazaNews_GetOpenTime(WiFiPlazaNews *news, u32 *openTime) {
    *openTime = news->openTime;
    return news->isOpen;
}

BOOL WiFiPlazaNews_IsSlotOccupied(WiFiPlazaNews *news, int slot) {
    return WiFiPlazaNewsSlots_IsOccupied(&news->slots, slot);
}

int WiFiPlazaNews_GetSlotGender(WiFiPlazaNews *news, int slot) {
    return WiFiPlazaNewsSlots_GetGender(&news->slots, slot);
}

BOOL WiFiPlazaNews_IsOwnSlot(WiFiPlazaNews *news, int slot) {
    return WiFiPlazaNewsSlots_IsOwn(&news->slots, slot);
}

BOOL WiFiPlazaNews_IsOldestSlot(WiFiPlazaNews *news, int slot) {
    return WiFiPlazaNewsSlots_IsOldest(&news->slots, slot);
}

BOOL WiFiPlazaNews_IsLastJoinedSlot(WiFiPlazaNews *news, int slot) {
    return WiFiPlazaNewsSlots_IsLastJoined(&news->slots, slot);
}

u16 WiFiPlazaNews_GetSlotChange(WiFiPlazaNews *news) {
    return WiFiPlazaNewsSlots_GetChange(&news->slots);
}

// TRUE if the event list has a current event.
BOOL WiFiPlazaNews_HasEvent(WiFiPlazaNews *news) {
    return news->events.head.next != &news->events.head;
}

u8 WiFiPlazaNews_GetEventType(WiFiPlazaNews *news) {
    if (!WiFiPlazaNews_HasEvent(news)) {
        GF_AssertFail();
    }
    return news->events.head.next->type;
}

// Formats the current event with ov45_02254C0C[type].
BOOL WiFiPlazaNews_FormatEvent(WiFiPlazaNews *news, const u8 *flags, int unused, String *out, enum HeapID heapID) {
    MessageFormat *format;
    MsgData *msgData;
    BOOL result;
    WiFiPlazaNewsEvent *event;
    int type;
    if (!WiFiPlazaNews_HasEvent(news)) {
        GF_AssertFail();
    }
    event = news->events.head.next;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0757_bin, heapID);
    format = MessageFormat_New(heapID);
    type = event->type;
    if ((u32)type < 9) {
        result = ov45_02254C0C[type](event, flags, format, msgData, out, heapID);
    } else {
        result = FALSE;
    }
    DestroyMsgData(msgData);
    MessageFormat_Delete(format);
    return result;
}

// The lobby slot of the index-th name of the current event (20: none), by ov45_02254BE8[type].
int WiFiPlazaNews_GetEventSlot(WiFiPlazaNews *news, int unused, int index) {
    WiFiPlazaNewsEvent *event;
    int type;
    if (!WiFiPlazaNews_HasEvent(news)) {
        GF_AssertFail();
    }
    event = news->events.head.next;
    type = event->type;
    if ((u32)type < 9) {
        return ov45_02254BE8[type](event, index);
    }
    GF_AssertFail();
    return 20;
}

void WiFiPlazaNewsSlots_Init(WiFiPlazaNewsSlots *slots) {
    int i;
    for (i = 0; i < 20; i++) {
        WiFiPlazaNewsSlots_Set(slots, i, 2, 0);
    }
}

void WiFiPlazaNewsSlots_Set(WiFiPlazaNewsSlots *slots, int slot, int state, int flags) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    slots->slots[slot].state = state;
    slots->slots[slot].flags = flags;
}

void WiFiPlazaNewsSlots_AddPlayer(WiFiPlazaNewsSlots *slots, int slot, int state, int flags) {
    WiFiPlazaNewsSlots_Set(slots, slot, state, flags);
    WiFiPlazaNewsSlots_SetChange(slots, 1);
    WiFiPlazaNewsSlots_SetLastJoined(slots, slot);
}

void WiFiPlazaNewsSlots_RemovePlayer(WiFiPlazaNewsSlots *slots, int slot) {
    WiFiPlazaNewsSlots_Set(slots, slot, 2, 0);
    WiFiPlazaNewsSlots_SetChange(slots, 2);
}

void WiFiPlazaNewsSlots_SetFlags(WiFiPlazaNewsSlots *slots, int slot, int flags) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    *(u16 *)((u8 *)slots + slot * 4 + 2) = flags;
}

// 1: a player came, 2: a player left.
void WiFiPlazaNewsSlots_SetChange(WiFiPlazaNewsSlots *slots, int change) {
    if (change > 2) {
        GF_AssertFail();
    }
    slots->change = change;
}

void WiFiPlazaNewsSlots_SetLastJoined(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    slots->lastSlot = slot;
}

BOOL WiFiPlazaNewsSlots_IsOccupied(WiFiPlazaNewsSlots *slots, int slot) {
    return WiFiPlazaNewsSlots_GetGender(slots, slot) != 2;
}

int WiFiPlazaNewsSlots_GetGender(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return *(u16 *)((u8 *)slots + slot * 4);
}

BOOL WiFiPlazaNewsSlots_IsOwn(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return (*(u16 *)((u8 *)slots + slot * 4 + 2) & 1) != 0;
}

BOOL WiFiPlazaNewsSlots_IsOldest(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return (*(u16 *)((u8 *)slots + slot * 4 + 2) & 2) != 0;
}

int WiFiPlazaNewsSlots_GetFlags(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return *(u16 *)((u8 *)slots + slot * 4 + 2);
}

u16 WiFiPlazaNewsSlots_GetChange(WiFiPlazaNewsSlots *slots) {
    return slots->change;
}
