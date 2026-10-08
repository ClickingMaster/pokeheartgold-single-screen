#pragma force_active on
#include "global.h"
#include "pokedex.h"
#include "heap.h"
#include "msgdata.h"

typedef struct {
    u32 species;
    u32 value;
    u8 unk8[8];
} Ov40DexEntry;

typedef struct {
    u8 unk0[0x1D4];
    int seenCount;
    int count;
    u16 *list;
    MsgData *msgData;
    int unk1E4;
    Ov40DexEntry *entries;
} Ov40DexData;

typedef struct {
    u8 unk0[0x830];
    SaveData *saveData;
    u8 unk834[0x860 - 0x834];
    Ov40DexData *dexData;
} Ov40Work;

extern const u16 ov40_02245CD4[];
u16 *ov40_0222DD68(enum HeapID heapID, int a1, int *count);
void ov40_02235E34(Ov40Work *work, int idx);

// @fn ov40_02235E34
void ov40_02235E34(Ov40Work *work, int idx) {
    Ov40DexData *data = work->dexData;
    int end = ov40_02245CD4[idx + 1];
    u16 *tmp;
    Pokedex *pokedex;
    int start = ov40_02245CD4[idx];
    int i;

    data->list = ov40_0222DD68(HEAP_ID_109, 0, &data->count);
    tmp = ov40_0222DD68(HEAP_ID_109, 0, &data->count);
    pokedex = Save_Pokedex_Get(work->saveData);
    data->seenCount = 0;
    for (i = start; i < end; i++) {
        if (!Pokedex_CheckMonSeenFlag(pokedex, tmp[i])) {
            tmp[i] = 0xFFFF;
        } else {
            data->seenCount++;
        }
    }
    i = 0;
    for (; start < end; start++) {
        if (tmp[start] != 0xFFFF) {
            data->list[i] = tmp[start];
            i++;
        }
    }
    for (; i < data->count; i++) {
        data->list[i] = tmp[i];
    }
    Heap_Free(tmp);
    data->entries = Heap_Alloc(HEAP_ID_109, data->seenCount * sizeof(Ov40DexEntry));
    MI_CpuFill8(data->entries, 0, data->seenCount * sizeof(Ov40DexEntry));
    for (i = 0; i < data->seenCount; i++) {
        data->entries[i].species = data->list[i];
        data->entries[i].value = data->list[i];
    }
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 237, HEAP_ID_109);
    data->unk1E4 = 1;
}
