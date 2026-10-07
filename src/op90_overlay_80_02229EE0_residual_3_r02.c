#pragma force_active on
#include "global.h"
#include "heap.h"

typedef struct FrontierTrainer {
    u8 data[0x30];
} FrontierTrainer;

typedef struct FrontierMonData {
    u16 species;
    u16 unk2[5];
    u16 item;
    u16 unkE;
} FrontierMonData;

void *ov80_02229F04(FrontierTrainer *trainer, int trainerId, enum HeapID heapID, int arcId);
void ov80_02229EF4(FrontierMonData *mon, u16 id, int arcId);
int ov80_0222A5A4(u32 value);
BOOL ov80_0222A5E0(void *trainer, const u16 *species, const u16 *items, int count, int num, u16 *out, enum HeapID heapID);
void ov80_0222A52C(void *dest, u16 *ids, u8 *ivs, void *unk, int unk2, int num, enum HeapID heapID, int arcId);
void ov80_0222A6B8(u8 num, int trainerA, int trainerB, u16 *ids, void *dest, u8 *ivs, int unk, u8 split);

// @fn ov80_0222A6B8
void ov80_0222A6B8(u8 num, int trainerA, int trainerB, u16 *ids, void *dest, u8 *ivs, int unk, u8 split) {
    FrontierTrainer trainer;
    FrontierMonData mon;
    u16 species[4];
    u16 items[4];
    void *data;
    int i;

    data = ov80_02229F04(&trainer, trainerA, HEAP_ID_FIELD2, 0xCC);
    for (i = 0; i < num; i++) {
        ivs[i] = ov80_0222A5A4(trainerA);
    }
    if (split == FALSE) {
        ov80_0222A5E0(data, species, items, 0, num, ids, HEAP_ID_FIELD2);
    } else {
        ov80_0222A5E0(data, species, items, 0, num / 2, ids, HEAP_ID_FIELD2);
        for (i = 0; i < num / 2; i++) {
            ov80_02229EF4(&mon, ids[i], 0xCD);
            species[i] = mon.species;
            items[i] = mon.item;
        }
        Heap_Free(data);
        data = ov80_02229F04(&trainer, trainerB, HEAP_ID_FIELD2, 0xCC);
        ov80_0222A5E0(data, species, items, num / 2, num / 2, ids + num / 2, HEAP_ID_FIELD2);
        for (i = 0; i < num / 2; i++) {
            ivs[num / 2 + i] = ov80_0222A5A4(trainerB);
        }
    }
    Heap_Free(data);
    ov80_0222A52C(dest, ids, ivs, NULL, unk, num, HEAP_ID_FIELD2, 0xCD);
}
