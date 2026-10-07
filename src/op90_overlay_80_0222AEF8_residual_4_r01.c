#pragma force_active on
#include "global.h"
#include "heap.h"
#include "party.h"
#include "pokemon.h"

typedef struct Ov80Work {
    u8 pad0[0x10];
    u8 mode;
    u8 pad11[0x28 - 0x11];
    Party *party;
    u8 pad2C[0xA1A - 0x2C];
    u8 recvCount;
} Ov80Work;

int sub_0203769C(void);
int ov80_02237B24(u8 mode, int unk);
void ov80_0222B9CC(int netId, int size, void *data, Ov80Work *work);

// @fn ov80_0222B9CC
void ov80_0222B9CC(int netId, int size, void *data, Ov80Work *work) {
    int i;
    int count;
    int monSize;
    Pokemon *mon;

    work->recvCount++;
    if (netId == sub_0203769C()) {
        return;
    }
    count = ov80_02237B24(work->mode, 0);
    monSize = SizeOfStructPokemon();
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < count; i++) {
        MI_CpuCopy8((u8 *)data + i * monSize, mon, monSize);
        Party_AddMon(work->party, mon);
    }
    Heap_Free(mon);
    if (sub_0203769C() != 0) {
        Party_SwapSlots(work->party, 0, 2);
        Party_SwapSlots(work->party, 1, 3);
    }
}
