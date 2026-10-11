#include "global.h"

#include "msgdata/msg.naix"

#include "message_format.h"
#include "msgdata.h"
#include "wifi_plaza_internal.h"

// The plaza news (WiFiPlazaWork.unk_04, shown by the plaza news app): the time the lobby opened, the state of every
// lobby slot and a list of events (topics) built from the lobby requests.

void ov45_0222D8BC(WiFiPlazaNews *news, u32 *openTime) {
    news->openTime = *openTime;
    news->isOpen = 1;
}

#pragma force_active on

void ov45_0222D8C8(WiFiPlazaNews *news, int slot, int state, int flags) {
    ov45_0222DE58(&news->slots, slot, state, flags);
}

#pragma force_active reset

BOOL ov45_0222D8D4(WiFiPlazaNews *news, int slot) {
    BOOL result = ov45_0222DF14(&news->slots, slot);
    ov45_0222DE74(&news->slots, slot);
    return result;
}

// Moves flag 2 to the given lobby slot.
void ov45_0222D8F0(WiFiPlazaNews *news, int slot) {
    int i;
    for (i = 0; i < 20; i++) {
        u32 flags = ov45_0222DF38(&news->slots, i);
        if (flags & 2) {
            ov45_0222DE8C(&news->slots, i, flags & ~2);
        }
    }
    ov45_0222DE8C(&news->slots, slot, ov45_0222DF38(&news->slots, slot) | 2);
}

void ov45_0222D940(WiFiPlazaNews *news, WiFiPlazaPairRequest *req) {
    WiFiPlazaNewsEvent *event = ov45_0222E04C(&news->events, 8);
    if (event != NULL) {
        ov45_0222E0E0(event, 0, 0, 0, req->profiles[0], req->profiles[1], 0, 0, req->slots[0], req->slots[1], 0, 0, 600, 8, 0);
        ov45_0222E0A4(&news->events, event);
    }
}

void ov45_0222D990(WiFiPlazaNews *news, WiFiPlazaPairOptionRequest *req) {
    WiFiPlazaNewsEvent *event;
    if (req->option >= 27) {
        return;
    }
    event = ov45_0222E04C(&news->events, 7);
    if (event != NULL) {
        ov45_0222E0E0(event, req->option, 0, 0, req->profiles[0], req->profiles[1], 0, 0, req->slots[0], req->slots[1], 0, 0, 600, 7, 1);
        ov45_0222E0A4(&news->events, event);
    }
}

void ov45_0222D9EC(WiFiPlazaNews *news, WiFiPlazaGroupFlagRequest *req) {
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
    event = ov45_0222E04C(&news->events, (u16)rank);
    if (event != NULL) {
        ov45_0222E0E0(event, type, req->count, req->flag, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 900, rank, 2);
        ov45_0222E0A4(&news->events, event);
    }
}

void ov45_0222DA80(WiFiPlazaNews *news, WiFiPlazaSingleRequest *req) {
    WiFiPlazaNewsEvent *event;
    if (req->option != 3 && req->option != 4) {
        return;
    }
    event = ov45_0222E04C(&news->events, (u16)(req->option + 5));
    if (event != NULL) {
        ov45_0222E0E0(event, req->option, req->count, 0, req->profile, 0, 0, 0, req->slot, 0, 0, 0, 900, req->option + 5, 3);
        ov45_0222E0A4(&news->events, event);
    }
}

void ov45_0222DAE0(WiFiPlazaNews *news, WiFiPlazaGroupRequest *req) {
    WiFiPlazaNewsEvent *event = ov45_0222E04C(&news->events, 12);
    if (event != NULL) {
        ov45_0222E0E0(event, req->count, 0, 0, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 450, 12, 4);
        ov45_0222E0A4(&news->events, event);
    }
}

void ov45_0222DB3C(WiFiPlazaNews *news, WiFiPlazaGroupRequest *req) {
    WiFiPlazaNewsEvent *event = ov45_0222E04C(&news->events, 13);
    if (event != NULL) {
        ov45_0222E0E0(event, req->count, 0, 0, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 450, 13, 5);
        ov45_0222E0A4(&news->events, event);
    }
}

void ov45_0222DB98(WiFiPlazaNews *news, WiFiPlazaLobbyNotice *notice) {
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
    event = ov45_0222E04C(&news->events, 14);
    if (event != NULL) {
        ov45_0222E0E0(event, *notice->time, notice->which, 0, 0, 0, 0, 0, 0, 0, 0, 0, 900, 14, 6);
        ov45_0222E0A4(&news->events, event);
    }
}

void ov45_0222DC08(WiFiPlazaNews *news, int *slot, const u8 *flags) {
    WiFiPlazaNewsEvent *event;
    if ((u32)*slot >= 20 || flags[*slot] == 0) {
        return;
    }
    event = ov45_0222E04C(&news->events, 1);
    if (event != NULL) {
        ov45_0222E0E0(event, *slot, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1800, 1, 7);
        ov45_0222E0A4(&news->events, event);
    }
}

void ov45_0222DC64(WiFiPlazaNews *news, WiFiPlazaGroupOptionRequest *req) {
    u32 type;
    u32 rank;
    WiFiPlazaNewsEvent *event;
    if ((req->option != 0 && req->option != 1 && req->option != 2) || !ov45_0222E5B4(*(volatile u32 *)&req->option, req->count)) {
        return;
    }
    type = req->option;
    rank = ov45_02254BC8[type];
    event = ov45_0222E04C(&news->events, rank);
    if (event != NULL) {
        ov45_0222E0E0(event, type, req->count, 0, req->profiles[0], req->profiles[1], req->profiles[2], req->profiles[3], req->slots[0], req->slots[1], req->slots[2], req->slots[3], 450, rank, 8);
        ov45_0222E0A4(&news->events, event);
    }
}

void ov45_0222DCE8(WiFiPlazaNews *news) {
    ov45_0222E03C(&news->events);
}

u32 ov45_0222DCF4(WiFiPlazaNews *news, u32 *openTime) {
    *openTime = news->openTime;
    return news->isOpen;
}

BOOL ov45_0222DCFC(WiFiPlazaNews *news, int slot) {
    return ov45_0222DECC(&news->slots, slot);
}

int ov45_0222DD08(WiFiPlazaNews *news, int slot) {
    return ov45_0222DEE0(&news->slots, slot);
}

BOOL ov45_0222DD14(WiFiPlazaNews *news, int slot) {
    return ov45_0222DEF4(&news->slots, slot);
}

BOOL ov45_0222DD20(WiFiPlazaNews *news, int slot) {
    return ov45_0222DF14(&news->slots, slot);
}

BOOL ov45_0222DD2C(WiFiPlazaNews *news, int slot) {
    return ov45_0222DF58(&news->slots, slot);
}

u16 ov45_0222DD38(WiFiPlazaNews *news) {
    return ov45_0222DF50(&news->slots);
}

// TRUE if the event list has a current event.
BOOL ov45_0222DD44(WiFiPlazaNews *news) {
    return news->events.head.next != &news->events.head;
}

u8 ov45_0222DD5C(WiFiPlazaNews *news) {
    if (!ov45_0222DD44(news)) {
        GF_AssertFail();
    }
    return news->events.head.next->type;
}

// Formats the current event with ov45_02254C0C[type].
BOOL ov45_0222DD78(WiFiPlazaNews *news, const u8 *flags, int unused, String *out, enum HeapID heapID) {
    MessageFormat *format;
    MsgData *msgData;
    BOOL result;
    WiFiPlazaNewsEvent *event;
    int type;
    if (!ov45_0222DD44(news)) {
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
int ov45_0222DDE4(WiFiPlazaNews *news, int unused, int index) {
    WiFiPlazaNewsEvent *event;
    int type;
    if (!ov45_0222DD44(news)) {
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

void ov45_0222DE1C(WiFiPlazaNewsSlots *slots) {
    int i;
    for (i = 0; i < 20; i++) {
        ov45_0222DE3C(slots, i, 2, 0);
    }
}

void ov45_0222DE3C(WiFiPlazaNewsSlots *slots, int slot, int state, int flags) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    slots->slots[slot].state = state;
    slots->slots[slot].flags = flags;
}

void ov45_0222DE58(WiFiPlazaNewsSlots *slots, int slot, int state, int flags) {
    ov45_0222DE3C(slots, slot, state, flags);
    ov45_0222DEA4(slots, 1);
    ov45_0222DEB8(slots, slot);
}

void ov45_0222DE74(WiFiPlazaNewsSlots *slots, int slot) {
    ov45_0222DE3C(slots, slot, 2, 0);
    ov45_0222DEA4(slots, 2);
}

void ov45_0222DE8C(WiFiPlazaNewsSlots *slots, int slot, int flags) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    *(u16 *)((u8 *)slots + slot * 4 + 2) = flags;
}

// 1: a player came, 2: a player left.
void ov45_0222DEA4(WiFiPlazaNewsSlots *slots, int change) {
    if (change > 2) {
        GF_AssertFail();
    }
    slots->change = change;
}

void ov45_0222DEB8(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    slots->lastSlot = slot;
}

BOOL ov45_0222DECC(WiFiPlazaNewsSlots *slots, int slot) {
    return ov45_0222DEE0(slots, slot) != 2;
}

int ov45_0222DEE0(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return *(u16 *)((u8 *)slots + slot * 4);
}

BOOL ov45_0222DEF4(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return (*(u16 *)((u8 *)slots + slot * 4 + 2) & 1) != 0;
}

BOOL ov45_0222DF14(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return (*(u16 *)((u8 *)slots + slot * 4 + 2) & 2) != 0;
}

int ov45_0222DF38(WiFiPlazaNewsSlots *slots, int slot) {
    if ((u32)slot >= 20) {
        GF_AssertFail();
    }
    return *(u16 *)((u8 *)slots + slot * 4 + 2);
}

u16 ov45_0222DF50(WiFiPlazaNewsSlots *slots) {
    return slots->change;
}
