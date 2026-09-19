#include "to52_overlay89_r5_private.h"

void ov89_02259E50(u32 column, void *saveData, Ov89PartyEntry *entries) {
    void *party;
    void *mon;
    int count;
    int i;

    MI_CpuFill8(entries, 0, 0x48);
    party = SaveArray_Party_Get(saveData);
    count = Party_GetCount(party);
    i = 0;
    if (count > 0) {
        do {
            mon = Party_GetMonByIndex(party, i);
            entries->species = GetMonData(mon, 0xAE, NULL);
            entries->personality = GetMonData(mon, 0, NULL);
            entries->form = GetMonData(mon, 0x70, NULL);
            entries->value = ov89_0225C830(column, GetMonData(mon, 7, NULL));
            i++;
            entries++;
        } while (i < count);
    }
}

BOOL ov89_02259EC4(u8 *context, Ov89PartyEntry *entry, void *network) {
    BOOL result;
    int index;
    u8 *sprites;
    u32 spriteOffset;

    result = ov89_0225A49C(context, context + 0x194, entry, *(void **)(context + 0x160), *(void **)(context + 0x164), *(void **)(context + 0x19E0));
    if (result == TRUE) {
        index = ov45_0222EC7C(network);
        if (index != -1) {
            if (entry->species != ((u16 *)(context + 0x99C))[index] || entry->value != ((u16 *)(context + 0x9AC))[index]) {
                ((u16 *)(context + 0x99C))[index] = entry->species;
                ((u16 *)(context + 0x9AC))[index] = entry->value;
                sprites = context + 0x970;
                spriteOffset = index * 4;
                ov89_02259A3C(entry->species, entry->form, *(void **)(sprites + spriteOffset), *(void **)(context + 0x160), *(void **)(context + 0x164), 2, *(void **)(context + 0x19E0));
                ManagedSprite_SetDrawFlag(*(void **)(sprites + spriteOffset), TRUE);
                PaletteData_FillPaletteInBuffer(*(void **)(context + 0xC), 1, 2, entry->value, index + 0x21, index + 0x22);
            }
        }
        PlaySE(0x5EB);
    }
    return result;
}
