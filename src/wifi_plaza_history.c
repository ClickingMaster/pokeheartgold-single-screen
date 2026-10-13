#include "global.h"

#include "wifi_plaza_internal.h"

// The last activities of the own player (WiFiPlazaPlayer.slots / slotData), a ring of 13 entries kept with a CRC.

void WiFiPlazaHistory_Init(WiFiPlazaHistory *history, SaveData *save) {
    int i;

    for (i = 0; i < 13; i++) {
        history->ids[i] = 24;
        history->values[i] = -1;
    }
    history->oldest = 0;
    history->next = 0;
    history->save = save;
    WiFiPlazaHistory_UpdateChecksum(history);
}

// Adds an entry; the oldest one goes when the ring is full.
void WiFiPlazaHistory_Add(WiFiPlazaHistory *history, int id, s32 value) {
    WiFiPlazaHistory_CheckChecksum(history);
    if ((history->next + 1) % 13 == history->oldest) {
        WiFiPlazaHistory_RemoveOldest(history);
    }
    history->ids[history->next] = id;
    history->values[history->next] = value;
    history->next = (history->next + 1) % 13;
    WiFiPlazaHistory_UpdateChecksum(history);
}

// Replaces every value oldValue with newValue; TRUE if there was one.
BOOL WiFiPlazaHistory_ReplaceValue(WiFiPlazaHistory *history, s32 oldValue, s32 newValue) {
    int i;
    BOOL found = FALSE;

    WiFiPlazaHistory_CheckChecksum(history);
    for (i = 0; i < 13; i++) {
        if (history->values[i] == oldValue) {
            history->values[i] = newValue;
            found = TRUE;
        }
    }
    WiFiPlazaHistory_UpdateChecksum(history);
    return found;
}

// Removes the oldest entry and returns its id (24 if there is none).
u8 WiFiPlazaHistory_RemoveOldest(WiFiPlazaHistory *history) {
    u8 id;

    WiFiPlazaHistory_CheckChecksum(history);
    if (history->next == history->oldest) {
        return 24;
    }
    id = history->ids[history->oldest];
    history->ids[history->oldest] = 24;
    history->values[history->next] = -1;
    history->oldest = (history->oldest + 1) % 13;
    WiFiPlazaHistory_UpdateChecksum(history);
    return id;
}

// The id of the index-th newest entry.
u8 WiFiPlazaHistory_GetId(WiFiPlazaHistory *history, u32 index) {
    int slot;

    if (index >= 12) {
        GF_AssertFail();
    }
    WiFiPlazaHistory_CheckChecksum(history);
    slot = history->next - (index + 1);
    if (slot < 0) {
        slot += 13;
    }
    return history->ids[slot];
}

// The value of the index-th newest entry.
s32 WiFiPlazaHistory_GetValue(WiFiPlazaHistory *history, u32 index) {
    int slot;

    if (index >= 12) {
        GF_AssertFail();
    }
    WiFiPlazaHistory_CheckChecksum(history);
    slot = history->next - (index + 1);
    if (slot < 0) {
        slot += 13;
    }
    return history->values[slot];
}

void WiFiPlazaHistory_UpdateChecksum(WiFiPlazaHistory *history) {
    history->crc = SaveArray_CalcCRC16(history->save, history, offsetof(WiFiPlazaHistory, save));
}

void WiFiPlazaHistory_CheckChecksum(WiFiPlazaHistory *history) {
    u16 crc = SaveArray_CalcCRC16(history->save, history, offsetof(WiFiPlazaHistory, save));

    if (crc != history->crc) {
        GF_AssertFail();
        history->corrupt = TRUE;
    }
}

u16 WiFiPlazaHistory_IsCorrupt(WiFiPlazaHistory *history) {
    return history->corrupt;
}
