#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0757.h"
#include "wifi_plaza/wifi_plaza_internal.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"

// The plaza news (WiFiPlazaWork.news, shown by the plaza news app): the time the lobby opened, the state of every
// lobby slot and a list of events (topics) built from the lobby requests.

static const u8 sWiFiPlazaNewsMinigameResultRanks[3] = { 11, 10, 9 };

// Result message per player count: the first two games, then the third one (two players at least).
static const u16 sWiFiPlazaNewsMinigameResultMsgs[4] = { 22, 24, 25, 26 };

static const u16 sWiFiPlazaNewsMinigameResultMsgsOther[4] = { 0, 27, 28, 23 };

static const u32 sWiFiPlazaNewsMinigameEntryRanks[3] = { 4, 3, 2 };

u16 (*const gWiFiPlazaNewsEventSlotGetters[9])(WiFiPlazaNewsEvent *event, u32 index) = {
    WiFiPlazaNewsEvent_GetChatSlot,
    WiFiPlazaNewsEvent_GetTapToyTradeSlot,
    WiFiPlazaNewsEvent_GetMinigameEntrySlot,
    WiFiPlazaNewsEvent_GetFootprintStampSlot,
    WiFiPlazaNewsEvent_GetCheckingSlot,
    WiFiPlazaNewsEvent_GetCheckingSlot,
    WiFiPlazaNewsEvent_GetLobbyEventNoticeSlot,
    WiFiPlazaNewsEvent_GetSpecialVisitorSlot,
    WiFiPlazaNewsEvent_GetMinigameResultSlot,
};

// WiFiPlazaNewsEvent.type: the kind of news, an index into sWiFiPlazaNewsEventFormatters
enum WiFiPlazaNewsEventType {
    WIFI_PLAZA_NEWS_EVENT_CHAT,
    WIFI_PLAZA_NEWS_EVENT_TAP_TOY_TRADE,
    WIFI_PLAZA_NEWS_EVENT_MINIGAME_ENTRY,
    WIFI_PLAZA_NEWS_EVENT_FOOTPRINT_STAMP,
    WIFI_PLAZA_NEWS_EVENT_VISITOR_PROFILES,
    WIFI_PLAZA_NEWS_EVENT_PLAZA_NEWS,
    WIFI_PLAZA_NEWS_EVENT_LOBBY_EVENT,
    WIFI_PLAZA_NEWS_EVENT_SPECIAL_VISITOR,
    WIFI_PLAZA_NEWS_EVENT_MINIGAME_RESULT,
    WIFI_PLAZA_NEWS_EVENT_COUNT,
};

static BOOL (*const sWiFiPlazaNewsEventFormatters[])(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out, enum HeapID heapID) = {
    WiFiPlazaNewsEvent_FormatChat,
    WiFiPlazaNewsEvent_FormatTapToyTrade,
    WiFiPlazaNewsEvent_FormatMinigameEntry,
    WiFiPlazaNewsEvent_FormatFootprintStamp,
    WiFiPlazaNewsEvent_FormatChecking,
    WiFiPlazaNewsEvent_FormatChecking,
    WiFiPlazaNewsEvent_FormatLobbyEventNotice,
    WiFiPlazaNewsEvent_FormatSpecialVisitor,
    WiFiPlazaNewsEvent_FormatMinigameResult,
};

void WiFiPlazaNews_SetOpenTime(WiFiPlazaNews *news, u32 *openTime) {
    news->openTime = *openTime;
    news->isOpen = 1;
}

void WiFiPlazaNews_AddPlayer(WiFiPlazaNews *news, int slot, int state, int flags) {
    WiFiPlazaNewsSlots_AddPlayer(&news->slots, slot, state, flags);
}

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
        WiFiPlazaNewsEvent_Set(event, 0, 0, 0, req->profiles[0], req->profiles[1], NULL, NULL, req->slots[0], req->slots[1], 0, 0, 600, 8, WIFI_PLAZA_NEWS_EVENT_CHAT);
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
        WiFiPlazaNewsEvent_Set(event, req->option, 0, 0, req->profiles[0], req->profiles[1], NULL, NULL, req->slots[0], req->slots[1], 0, 0, 600, 7, WIFI_PLAZA_NEWS_EVENT_TAP_TOY_TRADE);
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
    rank = sWiFiPlazaNewsMinigameEntryRanks[type];
    event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, (u16)rank);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, type, req->count, req->flag, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 900, rank, WIFI_PLAZA_NEWS_EVENT_MINIGAME_ENTRY);
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
        WiFiPlazaNewsEvent_Set(event, req->option, req->count, 0, req->profile, NULL, NULL, NULL, req->slot, 0, 0, 0, 900, req->option + 5, WIFI_PLAZA_NEWS_EVENT_FOOTPRINT_STAMP);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_AddVisitorProfilesEvent(WiFiPlazaNews *news, WiFiPlazaGroupRequest *req) {
    WiFiPlazaNewsEvent *event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, 12);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, req->count, 0, 0, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 450, 12, WIFI_PLAZA_NEWS_EVENT_VISITOR_PROFILES);
        WiFiPlazaNewsEventList_Insert(&news->events, event);
    }
}

void WiFiPlazaNews_AddPlazaNewsEvent(WiFiPlazaNews *news, WiFiPlazaGroupRequest *req) {
    WiFiPlazaNewsEvent *event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, 13);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, req->count, 0, 0, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 450, 13, WIFI_PLAZA_NEWS_EVENT_PLAZA_NEWS);
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
        WiFiPlazaNewsEvent_Set(event, *notice->time, notice->which, 0, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 900, 14, WIFI_PLAZA_NEWS_EVENT_LOBBY_EVENT);
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
        WiFiPlazaNewsEvent_Set(event, *slot, 0, 0, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 1800, 1, WIFI_PLAZA_NEWS_EVENT_SPECIAL_VISITOR);
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
    rank = sWiFiPlazaNewsMinigameResultRanks[type];
    event = WiFiPlazaNewsEventList_GetFreeEvent(&news->events, rank);
    if (event != NULL) {
        WiFiPlazaNewsEvent_Set(event, type, req->count, 0, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 450, rank, WIFI_PLAZA_NEWS_EVENT_MINIGAME_RESULT);
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

// Formats the current event with sWiFiPlazaNewsEventFormatters[type].
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
    if ((u32)type < WIFI_PLAZA_NEWS_EVENT_COUNT) {
        result = sWiFiPlazaNewsEventFormatters[type](event, flags, format, msgData, out, heapID);
    } else {
        result = FALSE;
    }
    DestroyMsgData(msgData);
    MessageFormat_Delete(format);
    return result;
}

// The lobby slot of the index-th name of the current event (20: none), by gWiFiPlazaNewsEventSlotGetters[type].
int WiFiPlazaNews_GetEventSlot(WiFiPlazaNews *news, int unused, int index) {
    WiFiPlazaNewsEvent *event;
    int type;
    if (!WiFiPlazaNews_HasEvent(news)) {
        GF_AssertFail();
    }
    event = news->events.head.next;
    type = event->type;
    if ((u32)type < WIFI_PLAZA_NEWS_EVENT_COUNT) {
        return gWiFiPlazaNewsEventSlotGetters[type](event, index);
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
    slots->slots[slot].flags = flags;
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
    return slots->slots[slot].state;
}

BOOL WiFiPlazaNewsSlots_IsOwn(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return (slots->slots[slot].flags & 1) != 0;
}

BOOL WiFiPlazaNewsSlots_IsOldest(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return (slots->slots[slot].flags & 2) != 0;
}

int WiFiPlazaNewsSlots_GetFlags(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return slots->slots[slot].flags;
}

u16 WiFiPlazaNewsSlots_GetChange(WiFiPlazaNewsSlots *slots) {
    return slots->change;
}

// The event list of the plaza news: 8 events in a list ordered by rank (head = WiFiPlazaNewsEventList.head); a new
// event takes a free one or replaces one of at least its rank. Each event type has a message (sWiFiPlazaNewsEventFormatters) and a
// lobby slot getter (gWiFiPlazaNewsEventSlotGetters).

BOOL WiFiPlazaNewsSlots_IsLastJoined(WiFiPlazaNewsSlots *slots, u32 slot) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    return slots->lastSlot == slot;
}

void WiFiPlazaNewsEventList_Init(WiFiPlazaNewsEventList *list, enum HeapID heapID) {
    s32 i, j;
    WiFiPlazaNewsEvent *event;
    memset(list, 0, sizeof(WiFiPlazaNewsEventList));
    for (i = 0, event = list->events; i < 8; event++, i++) {
        for (j = 0; j < 4; j++) {
            event->names[j] = String_New(8, heapID);
        }
    }
    list->head.next = &list->head;
    list->head.prev = &list->head;
}

void WiFiPlazaNewsEventList_Free(WiFiPlazaNewsEventList *list) {
    s32 i, j;
    WiFiPlazaNewsEvent *event;
    for (i = 0, event = list->events; i < 8; i++, event++) {
        for (j = 0; j < 4; j++) {
            String_Delete(event->names[j]);
        }
    }
    memset(list, 0, sizeof(WiFiPlazaNewsEventList));
}

// Counts the timers of the events down; an event goes when its timer runs out.
void WiFiPlazaNewsEventList_Update(WiFiPlazaNewsEventList *list) {
    WiFiPlazaNewsEvent *event = list->head.next;
    WiFiPlazaNewsEvent *head = &list->head;
    while (event != head) {
        WiFiPlazaNewsEvent *next = event->next;
        event->rank = 0;
        if (event->timer - 1 > 0) {
            event->timer--;
        } else {
            WiFiPlazaNewsEventList_Remove(list, event);
        }
        event = next;
    }
}

// Removes the current event.
void WiFiPlazaNewsEventList_RemoveFirst(WiFiPlazaNewsEventList *list) {
    WiFiPlazaNewsEventList_Remove(list, list->head.next);
}

// A free event, or one of at least rank (taken out of the list); NULL if there is none.
WiFiPlazaNewsEvent *WiFiPlazaNewsEventList_GetFreeEvent(WiFiPlazaNewsEventList *list, u16 rank) {
    s32 i;
    WiFiPlazaNewsEvent *event = NULL;
    for (i = 0; i < 8; i++) {
        if (list->events[i].next == NULL) {
            event = &list->events[i];
            break;
        }
    }
    if (event == NULL) {
        for (i = 0; i < 8; i++) {
            if (list->events[i].rank >= rank) {
                event = &list->events[i];
                WiFiPlazaNewsEventList_Remove(list, event);
                break;
            }
        }
    }
    return event;
}

void WiFiPlazaNewsEvent_InsertAfter(WiFiPlazaNewsEvent *at, WiFiPlazaNewsEvent *event) {
    event->next = at->next;
    event->prev = at;
    at->next = event;
    event->next->prev = event;
}

// Inserts the event after the last one of a lower rank.
void WiFiPlazaNewsEventList_Insert(WiFiPlazaNewsEventList *list, WiFiPlazaNewsEvent *event) {
    WiFiPlazaNewsEvent *at = list->head.prev;
    WiFiPlazaNewsEvent *head = &list->head;
    if (at != head) {
        u16 rank = event->rank;
        do {
            if (at->rank < rank) {
                break;
            }
            at = at->prev;
        } while (at != head);
    }
    WiFiPlazaNewsEvent_InsertAfter(at, event);
}

void WiFiPlazaNewsEventList_Remove(WiFiPlazaNewsEventList *list, WiFiPlazaNewsEvent *event) {
    event->prev->next = event->next;
    event->next->prev = event->prev;
    event->prev = NULL;
    event->next = NULL;
}

void WiFiPlazaNewsEvent_Set(WiFiPlazaNewsEvent *event, u32 a, u32 b, u32 c, const PlayerProfile *player0, const PlayerProfile *player1, const PlayerProfile *player2, const PlayerProfile *player3, u16 slot0, u16 slot1, u16 slot2, u16 slot3, u32 timer, u32 rank, u32 type) {
    if (type >= WIFI_PLAZA_NEWS_EVENT_COUNT) {
        GF_AssertFail();
    }
    event->type = type;
    event->timer = timer;
    event->rank = rank;
    event->a = a;
    event->b = b;
    event->c = c;
    event->slots[0] = slot0;
    event->slots[1] = slot1;
    event->slots[2] = slot2;
    event->slots[3] = slot3;
    if (player0 != NULL) {
        PlayerName_FlatToString((PlayerProfile *)player0, event->names[0]);
    }
    if (player1 != NULL) {
        PlayerName_FlatToString((PlayerProfile *)player1, event->names[1]);
    }
    if (player2 != NULL) {
        PlayerName_FlatToString((PlayerProfile *)player2, event->names[2]);
    }
    if (player3 != NULL) {
        PlayerName_FlatToString((PlayerProfile *)player3, event->names[3]);
    }
}

// Type 0: two players (WiFiPlazaPairRequest).
BOOL WiFiPlazaNewsEvent_FormatChat(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out, enum HeapID heapID) {
    String *string;
    BufferString(format, 0, event->names[0], 0, 1, gGameLanguage);
    BufferString(format, 1, event->names[1], 0, 1, gGameLanguage);
    string = NewString_ReadMsgData(msgData, msg_0757_00007);
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

// Type 1: two players and an instrument (a).
BOOL WiFiPlazaNewsEvent_FormatTapToyTrade(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out, enum HeapID heapID) {
    String *string;
    BufferString(format, 0, event->names[0], 0, 1, gGameLanguage);
    BufferString(format, 1, event->names[1], 0, 1, gGameLanguage);
    BufferWiFiPlazaInstrumentName(format, 2, event->a);
    string = NewString_ReadMsgData(msgData, msg_0757_00008);
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

// Type 2: an activity (a) with b players; c = 1: all of them are named.
BOOL WiFiPlazaNewsEvent_FormatMinigameEntry(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out, enum HeapID heapID) {
    String *string;
    u32 msgNo;
    BufferWiFiPlazaActivityName(format, 0, event->a);
    if (event->c == 1) {
        switch (event->b) {
        case 3:
            BufferString(format, 1, event->names[0], 0, 1, gGameLanguage);
            BufferString(format, 2, event->names[1], 0, 1, gGameLanguage);
            BufferString(format, 3, event->names[2], 0, 1, gGameLanguage);
            msgNo = msg_0757_00018;
            break;
        case 2:
            BufferString(format, 1, event->names[0], 0, 1, gGameLanguage);
            BufferString(format, 2, event->names[1], 0, 1, gGameLanguage);
            msgNo = msg_0757_00019;
            break;
        case 4:
            BufferString(format, 1, event->names[0], 0, 1, gGameLanguage);
            BufferString(format, 2, event->names[1], 0, 1, gGameLanguage);
            BufferString(format, 3, event->names[2], 0, 1, gGameLanguage);
            BufferString(format, 4, event->names[3], 0, 1, gGameLanguage);
            msgNo = msg_0757_00006;
            break;
        default:
            return FALSE;
        }
        string = NewString_ReadMsgData(msgData, msgNo);
    } else {
        BufferString(format, 1, event->names[0], 0, 1, gGameLanguage);
        BufferIntegerAsString(format, 2, 4 - event->b, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        string = NewString_ReadMsgData(msgData, msg_0757_00005);
    }
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

// Type 3: an activity (a), one named player and the others.
BOOL WiFiPlazaNewsEvent_FormatFootprintStamp(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out, enum HeapID heapID) {
    String *string;
    BufferWiFiPlazaActivityName(format, 0, event->a);
    BufferString(format, 1, event->names[0], 0, 1, gGameLanguage);
    BufferIntegerAsString(format, 2, 8 - event->b, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    string = NewString_ReadMsgData(msgData, msg_0757_00009);
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

// Types 4 and 5: a players (WiFiPlazaGroupRequest).
BOOL WiFiPlazaNewsEvent_FormatChecking(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out, enum HeapID heapID) {
    String *string;
    u32 i;
    for (i = 0; i < event->a; i++) {
        BufferString(format, i, event->names[i], 0, 1, gGameLanguage);
    }
    if (event->type == WIFI_PLAZA_NEWS_EVENT_VISITOR_PROFILES) {
        BufferWiFiPlazaActivityName(format, event->a, 5);
    } else {
        BufferWiFiPlazaActivityName(format, event->a, 6);
    }
    string = NewString_ReadMsgData(msgData, 0xe - event->a);
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

// Type 6: a lobby event (b = WiFiPlazaLobbyNotice.which).
BOOL WiFiPlazaNewsEvent_FormatLobbyEventNotice(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out, enum HeapID heapID) {
    String *string;
    u32 msgNo;
    switch (event->b) {
    case 0:
        BufferWiFiPlazaEventName(format, 0, 0);
        msgNo = msg_0757_00015;
        break;
    case 1:
        BufferWiFiPlazaEventName(format, 0, 1);
        msgNo = msg_0757_00020;
        break;
    case 2:
        BufferWiFiPlazaEventName(format, 0, 2);
        msgNo = msg_0757_00016;
        break;
    case 4:
        msgNo = msg_0757_00017;
        break;
    default:
        return FALSE;
    }
    string = NewString_ReadMsgData(msgData, msgNo);
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

// Type 7: the lobby slot a, if flags[a] is set.
BOOL WiFiPlazaNewsEvent_FormatSpecialVisitor(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out, enum HeapID heapID) {
    u32 slot = event->a;
    if (slot >= 20) {
        return FALSE;
    }
    if (flags[slot] == 0) {
        return FALSE;
    }
    ReadMsgDataIntoString(msgData, 0x15, out);
    return TRUE;
}

// Type 8: an activity (a) with b players.
BOOL WiFiPlazaNewsEvent_FormatMinigameResult(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out, enum HeapID heapID) {
    String *string;
    u32 i;
    if (!WiFiPlaza_IsMinigamePlayerCountValid(event->a, event->b)) {
        return FALSE;
    }
    BufferWiFiPlazaActivityName(format, 0, event->a);
    for (i = 0; i < event->b; i++) {
        BufferString(format, i + 1, event->names[i], 0, 1, gGameLanguage);
    }
    if (event->a <= 1) {
        string = NewString_ReadMsgData(msgData, sWiFiPlazaNewsMinigameResultMsgs[event->b - 1]);
    } else {
        string = NewString_ReadMsgData(msgData, sWiFiPlazaNewsMinigameResultMsgsOther[event->b - 1]);
    }
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

u16 WiFiPlazaNewsEvent_GetChatSlot(WiFiPlazaNewsEvent *event, u32 index) {
    if (index >= 2) {
        return 20;
    }
    return event->slots[index];
}

u16 WiFiPlazaNewsEvent_GetTapToyTradeSlot(WiFiPlazaNewsEvent *event, u32 index) {
    if (index >= 2) {
        return 20;
    }
    return event->slots[index];
}

u16 WiFiPlazaNewsEvent_GetMinigameEntrySlot(WiFiPlazaNewsEvent *event, u32 index) {
    if (event->b <= index) {
        return 20;
    }
    return event->slots[index];
}

u16 WiFiPlazaNewsEvent_GetFootprintStampSlot(WiFiPlazaNewsEvent *event, u32 index) {
    if (index >= 1) {
        return 20;
    }
    return event->slots[index];
}

u16 WiFiPlazaNewsEvent_GetCheckingSlot(WiFiPlazaNewsEvent *event, u32 index) {
    if (event->a <= index) {
        return 20;
    }
    return event->slots[index];
}

u16 WiFiPlazaNewsEvent_GetLobbyEventNoticeSlot(WiFiPlazaNewsEvent *event, u32 index) {
    return 20;
}

u16 WiFiPlazaNewsEvent_GetSpecialVisitorSlot(WiFiPlazaNewsEvent *event, u32 index) {
    return 20;
}
