#pragma force_active on
#include "global.h"
#include "party.h"
#include "pokemon.h"

typedef struct Ov80Work {
    u8 pad0[4];
    u8 mode;
    u8 unk5;
    u8 unk6;
    u8 pad7[0x18 - 7];
    u16 unk18[(0x254 - 0x18) / 2];
    u16 unk254[(0x3D2 - 0x254) / 2];
    u16 unk3D2[4];
    u8 unk3DA[6];
    u32 unk3E0[4];
    u8 unk3F0[0x4D4 - 0x3F0];
    Party *party1;
    Party *party2;
} Ov80Work;

typedef struct Ov80LevelRange Ov80LevelRange;

u8 ov80_02236DF8(u8 mode, int unk);
const Ov80LevelRange *ov80_02236C2C(u16 a0, u8 a1);
int ov80_02236C9C(const u16 *species, const u16 *items, int count, int num, u16 *out, enum HeapID heapID, const Ov80LevelRange *range, u16 unk, u8 *ivs);
void FrontierMon_GenerateList(void *dest, u16 *ids, u8 *ivs, void *unk, void *unk2, int num, enum HeapID heapID, int arcId);
void ov80_022308C4(Ov80Work *work);

// @fn ov80_022308C4
void ov80_022308C4(Ov80Work *work) {
    u16 species[8];
    u16 items[8];
    int i;
    int count1;
    u8 num;
    Pokemon *mon;
    u32 count2;

    for (i = 0; i < 8; i++) {
        species[i] = 0;
        items[i] = 0;
    }
    num = ov80_02236DF8(work->mode, 1);
    count1 = Party_GetCount(work->party1);
    for (i = 0; i < count1; i++) {
        mon = Party_GetMonByIndex(work->party1, i);
        species[i] = GetMonData(mon, MON_DATA_SPECIES, NULL);
        items[i] = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    }
    count2 = Party_GetCount(work->party2);
    for (i = 0; i < (int)count2; i++) {
        Pokemon *mon2 = Party_GetMonByIndex(work->party2, i);
        species[count1 + i] = GetMonData(mon2, MON_DATA_SPECIES, NULL);
        items[count1 + i] = GetMonData(mon2, MON_DATA_HELD_ITEM, NULL);
        work->unk254[i] = work->unk3D2[i];
    }
    ov80_02236C9C(species, items, count1 + count2, num, work->unk3D2, HEAP_ID_FIELD2, ov80_02236C2C(work->unk18[work->unk6], work->unk5), 0, work->unk3DA);
    FrontierMon_GenerateList(work->unk3F0, work->unk3D2, work->unk3DA, NULL, work->unk3E0, num, HEAP_ID_FIELD2, 0xCD);
}
