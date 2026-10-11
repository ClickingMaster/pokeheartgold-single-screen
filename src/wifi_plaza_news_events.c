#include "global.h"

#include "heap.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "wifi_plaza_internal.h"

// The event list of the plaza news: 8 events in a list ordered by rank (head = WiFiPlazaNewsEventList.head); a new
// event takes a free one or replaces one of at least its rank. Each event type has a message (ov45_02254C0C) and a
// lobby slot getter (ov45_02254BE8).

BOOL ov45_0222DF58(WiFiPlazaNewsSlots *slots, u32 slot) {
    if (slot >= 20) {
        GF_AssertFail();
    }
    return slots->lastSlot == slot;
}

void ov45_0222DF78(WiFiPlazaNewsEventList *list, enum HeapID heapID) {
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

void ov45_0222DFD0(WiFiPlazaNewsEventList *list) {
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
void ov45_0222E000(WiFiPlazaNewsEventList *list) {
    WiFiPlazaNewsEvent *event = list->head.next;
    WiFiPlazaNewsEvent *head = &list->head;
    while (event != head) {
        WiFiPlazaNewsEvent *next = event->next;
        event->rank = 0;
        if (event->timer - 1 > 0) {
            event->timer--;
        } else {
            ov45_0222E0CC(list, event);
        }
        event = next;
    }
}

// Removes the current event.
void ov45_0222E03C(WiFiPlazaNewsEventList *list) {
    ov45_0222E0CC(list, list->head.next);
}

// A free event, or one of at least rank (taken out of the list); NULL if there is none.
WiFiPlazaNewsEvent *ov45_0222E04C(WiFiPlazaNewsEventList *list, u16 rank) {
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
                ov45_0222E0CC(list, event);
                break;
            }
        }
    }
    return event;
}

void ov45_0222E094(WiFiPlazaNewsEvent *at, WiFiPlazaNewsEvent *event) {
    event->next = at->next;
    event->prev = at;
    at->next = event;
    event->next->prev = event;
}

// Inserts the event after the last one of a lower rank.
void ov45_0222E0A4(WiFiPlazaNewsEventList *list, WiFiPlazaNewsEvent *event) {
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
    ov45_0222E094(at, event);
}

void ov45_0222E0CC(WiFiPlazaNewsEventList *list, WiFiPlazaNewsEvent *event) {
    event->prev->next = event->next;
    event->next->prev = event->prev;
    event->prev = NULL;
    event->next = NULL;
}

void ov45_0222E0E0(WiFiPlazaNewsEvent *event, u32 a, u32 b, u32 c, const PlayerProfile *player0, const PlayerProfile *player1, const PlayerProfile *player2, const PlayerProfile *player3, u16 slot0, u16 slot1, u16 slot2, u16 slot3, u32 timer, u32 rank, u32 type) {
    if (type >= 9) {
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
BOOL ov45_0222E14C(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out) {
    String *string;
    BufferString(format, 0, event->names[0], 0, 1, gGameLanguage);
    BufferString(format, 1, event->names[1], 0, 1, gGameLanguage);
    string = NewString_ReadMsgData(msgData, 7);
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

// Type 1: two players and an instrument (a).
BOOL ov45_0222E1A0(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out) {
    String *string;
    BufferString(format, 0, event->names[0], 0, 1, gGameLanguage);
    BufferString(format, 1, event->names[1], 0, 1, gGameLanguage);
    BufferWiFiPlazaInstrumentName(format, 2, event->a);
    string = NewString_ReadMsgData(msgData, 8);
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

// Type 2: an activity (a) with b players; c = 1: all of them are named.
BOOL ov45_0222E200(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out) {
    String *string;
    u32 msgNo;
    BufferWiFiPlazaActivityName(format, 0, event->a);
    if (event->c == 1) {
        switch (event->b) {
        case 3:
            BufferString(format, 1, event->names[0], 0, 1, gGameLanguage);
            BufferString(format, 2, event->names[1], 0, 1, gGameLanguage);
            BufferString(format, 3, event->names[2], 0, 1, gGameLanguage);
            msgNo = 0x12;
            break;
        case 2:
            BufferString(format, 1, event->names[0], 0, 1, gGameLanguage);
            BufferString(format, 2, event->names[1], 0, 1, gGameLanguage);
            msgNo = 0x13;
            break;
        case 4:
            BufferString(format, 1, event->names[0], 0, 1, gGameLanguage);
            BufferString(format, 2, event->names[1], 0, 1, gGameLanguage);
            BufferString(format, 3, event->names[2], 0, 1, gGameLanguage);
            BufferString(format, 4, event->names[3], 0, 1, gGameLanguage);
            msgNo = 6;
            break;
        default:
            return FALSE;
        }
        string = NewString_ReadMsgData(msgData, msgNo);
    } else {
        BufferString(format, 1, event->names[0], 0, 1, gGameLanguage);
        BufferIntegerAsString(format, 2, 4 - event->b, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        string = NewString_ReadMsgData(msgData, 5);
    }
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

// Type 3: an activity (a), one named player and the others.
BOOL ov45_0222E33C(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out) {
    String *string;
    BufferWiFiPlazaActivityName(format, 0, event->a);
    BufferString(format, 1, event->names[0], 0, 1, gGameLanguage);
    BufferIntegerAsString(format, 2, 8 - event->b, 1, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    string = NewString_ReadMsgData(msgData, 9);
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

// Types 4 and 5: a players (WiFiPlazaGroupRequest).
BOOL ov45_0222E39C(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out) {
    String *string;
    u32 i;
    for (i = 0; i < event->a; i++) {
        BufferString(format, i, event->names[i], 0, 1, gGameLanguage);
    }
    if (event->type == 4) {
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
BOOL ov45_0222E414(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out) {
    String *string;
    u32 msgNo;
    switch (event->b) {
    case 0:
        BufferWiFiPlazaEventName(format, 0, 0);
        msgNo = 0xf;
        break;
    case 1:
        BufferWiFiPlazaEventName(format, 0, 1);
        msgNo = 0x14;
        break;
    case 2:
        BufferWiFiPlazaEventName(format, 0, 2);
        msgNo = 0x10;
        break;
    case 4:
        msgNo = 0x11;
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
BOOL ov45_0222E484(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out) {
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
BOOL ov45_0222E4A8(WiFiPlazaNewsEvent *event, const u8 *flags, MessageFormat *format, MsgData *msgData, String *out) {
    String *string;
    u32 i;
    if (!ov45_0222E5B4(event->a, event->b)) {
        return FALSE;
    }
    BufferWiFiPlazaActivityName(format, 0, event->a);
    for (i = 0; i < event->b; i++) {
        BufferString(format, i + 1, event->names[i], 0, 1, gGameLanguage);
    }
    if (event->a <= 1) {
        string = NewString_ReadMsgData(msgData, ov45_02254BCA[event->b]);
    } else {
        string = NewString_ReadMsgData(msgData, ov45_02254BD2[event->b]);
    }
    StringExpandPlaceholders(format, out, string);
    String_Delete(string);
    return TRUE;
}

u16 ov45_0222E540(WiFiPlazaNewsEvent *event, u32 index) {
    if (index >= 2) {
        return 20;
    }
    return event->slots[index];
}

u16 ov45_0222E550(WiFiPlazaNewsEvent *event, u32 index) {
    if (index >= 2) {
        return 20;
    }
    return event->slots[index];
}

u16 ov45_0222E560(WiFiPlazaNewsEvent *event, u32 index) {
    if (event->b <= index) {
        return 20;
    }
    return event->slots[index];
}

u16 ov45_0222E574(WiFiPlazaNewsEvent *event, u32 index) {
    if (index >= 1) {
        return 20;
    }
    return event->slots[index];
}

u16 ov45_0222E584(WiFiPlazaNewsEvent *event, u32 index) {
    if (event->a <= index) {
        return 20;
    }
    return event->slots[index];
}

u16 ov45_0222E598(WiFiPlazaNewsEvent *event, u32 index) {
    return 20;
}

u16 ov45_0222E59C(WiFiPlazaNewsEvent *event, u32 index) {
    return 20;
}
