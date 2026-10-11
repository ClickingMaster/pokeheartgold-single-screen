#pragma force_active on
#include "global.h"
#include "heap.h"

typedef struct Ov80MonTemplate {
    u16 species : 11;
    u16 form : 5;
    u16 item;
    u32 data[13];
} Ov80MonTemplate;

typedef struct Ov80LevelRange Ov80LevelRange;

const Ov80LevelRange *ov80_02236C2C(u16 a0, u8 a1);
int ov80_02236C9C(const u16 *species, const u16 *items, int count, int num, u16 *out, enum HeapID heapID, const Ov80LevelRange *range, u16 unk, u8 *ivs);
void FrontierMon_GenerateList(void *dest, u16 *ids, u8 *ivs, void *unk, void *unk2, int num, enum HeapID heapID, int arcId);
void ov80_02236E90(u8 num, u16 a1, u8 a2, const Ov80MonTemplate *templates, u16 *ids, void *dest, u8 *ivs, void *unk, int count);

// @fn ov80_02236E90
void ov80_02236E90(u8 num, u16 a1, u8 a2, const Ov80MonTemplate *templates, u16 *ids, void *dest, u8 *ivs, void *unk, int count) {
    Ov80MonTemplate mon;
    u16 species[12];
    u16 items[12];
    int i;
    const Ov80LevelRange *range = ov80_02236C2C(a1, a2);

    for (i = 0; i < count; i++) {
        mon = templates[i];
        species[i] = mon.species;
        items[i] = mon.item;
    }
    ov80_02236C9C(species, items, count, num, ids, HEAP_ID_FIELD2, range, 0, ivs);
    FrontierMon_GenerateList(dest, ids, ivs, NULL, unk, num, HEAP_ID_FIELD2, 0xCD);
}
