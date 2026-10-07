#pragma force_active on
#include "global.h"
#include "assert.h"
#include "save.h"

typedef struct Ov39Record {
    u8 unk0[0x80];
    u8 data[0x58];
    u64 id;
    u16 crc;
    u8 unkE2[2];
} Ov39Record;

typedef struct Ov39Entry {
    u32 unk0;
    u64 id;
    Ov39Record record;
} Ov39Entry;

typedef struct Ov39List {
    int count;
    Ov39Entry entries[1];
} Ov39List;

typedef struct Ov39Blk {
    u8 unk0[4];
    Ov39List list;
} Ov39Blk;

Ov39Blk *ov39_0222A2B4(SaveData **saveData);
int ov39_0222801C(SaveData **saveData, Ov39Record **out, int max);

// @fn ov39_0222801C
int ov39_0222801C(SaveData **saveData, Ov39Record **out, int max) {
    Ov39List *list = &ov39_0222A2B4(saveData)->list;
    int count = list->count;
    Ov39Entry *entry;
    int i;

    if (count > max) {
        count = max;
        GF_AssertFail();
    }
    entry = list->entries;
    for (i = 0; i < count; i++) {
        out[i] = &entry->record;
        if (out[i]->id != entry->id) {
            out[i]->id = entry->id;
            out[i]->crc = SaveArray_CalcCRC16(*saveData, out[i]->data, sizeof(out[i]->data));
        }
        entry++;
    }
    for (; i < max; i++) {
        out[i] = NULL;
    }
    return count;
}
