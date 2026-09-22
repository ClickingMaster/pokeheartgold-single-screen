#include "overlay_resource_actor_capture_private.h"

void sub_02076C90(Work *work) {
    int slots[2];
    int index;
    void *mon;
    u32 mailData[6];
    switch (work->action) {
    case 13:
    case 14:
        if (Bag_GetQuantity(work->bag, 4, work->heapId) == 0) {
            break;
        }
        if (Party_GetCount(work->party) >= Party_GetMaxCount(work->party)) {
            break;
        }
        mon = AllocMonZeroed(work->heapId);
        CopyPokemonToPokemon(work->pokemon, mon);
        slots[0] = 0x124;
        SetMonData(mon, 5, &slots[0]);
        slots[0] = 4;
        SetMonData(mon, 155, &slots[0]);
        slots[0] = 0;
        SetMonData(mon, 6, &slots[0]);
        SetMonData(mon, 11, &slots[0]);
        slots[1] = 25;
        index = 25;
        do {
            SetMonData(mon, index, &slots[0]);
            index = slots[1] + 1;
            slots[1] = index;
        } while (index < 54);
        slots[1] = 78;
        index = 78;
        do {
            SetMonData(mon, index, &slots[0]);
            index = slots[1] + 1;
            slots[1] = index;
        } while (index < 110);
        slots[1] = 123;
        index = 123;
        do {
            SetMonData(mon, index, &slots[0]);
            index = slots[1] + 1;
            slots[1] = index;
        } while (index < 144);
        SetMonData(mon, 181, &slots[0]);
        SetMonData(mon, 182, &slots[0]);
        SetMonData(mon, 183, &slots[0]);
        SetMonData(mon, 184, &slots[0]);
        SetMonData(mon, 185, &slots[0]);
        SetMonData(mon, 186, &slots[0]);
        SetMonData(mon, 187, &slots[0]);
        SetMonData(mon, 0xb3, 0);
        SetMonData(mon, 0x4d, &slots[0]);
        SetMonData(mon, 0xa0, &slots[0]);
        void *mail = Mail_New(work->heapId);
        SetMonData(mon, 0xaa, mail);
        Heap_Free(mail);
        SetMonData(mon, 0xa2, &slots[0]);
        MIi_CpuClearFast(0, mailData, sizeof(mailData));
        SetMonData(mon, 0xab, mailData);
        UpdateMonAbility(mon);
        CalcMonLevelAndStats(mon);
        Party_AddMon(work->party, mon);
        Pokedex_SetMonCaughtFlag(work->pokedex, mon);
        GameStats_Inc(work->stats, 0xd);
        GameStats_AddScore(work->stats, 0x15);
        Heap_Free(mon);
        Bag_TakeItem(work->bag, 4, 1, work->heapId);
        break;
    case 6:
    case 18:
    case 19:
        slots[1] = 0;
        SetMonData(work->pokemon, 6, &slots[1]);
        break;
    }
}
