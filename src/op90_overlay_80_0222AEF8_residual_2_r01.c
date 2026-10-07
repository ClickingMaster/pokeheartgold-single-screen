#pragma force_active on
#include "global.h"
#include "party.h"
#include "pokemon.h"

typedef struct Ov80Work {
    u8 pad0[4];
    u8 mode;
    u8 pad5[0x4D4 - 5];
    Party *party;
    u8 pad4D8[0x4E8 - 0x4D8];
    u16 ids[14];
    u16 sendBuf[64];
    u16 recvIds[6];
    u8 recvIVs[8];
    u32 recvPids[4];
    u8 pad5A8[0x702 - 0x5A8];
    u8 recvCount;
} Ov80Work;

int ov80_02236DD4(u8 mode);
BOOL sub_02037030(int cmd, void *data, int size);
BOOL ov80_0222B448(Ov80Work *work);

// @fn ov80_0222B448
BOOL ov80_0222B448(Ov80Work *work) {
    u32 pids[2];
    u8 ivs[4];
    int i;
    int pos;
    u16 *buf;
    Pokemon *mon;
    int count;

    buf = work->sendBuf;
    pos = 0;

    count = ov80_02236DD4(work->mode);
    for (i = 0; i < count; i++) {
        mon = Party_GetMonByIndex(work->party, i);
        ivs[i] = GetMonData(mon, MON_DATA_ATK_IV, NULL);
        pids[i] = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
    }
    for (i = 0; i < count; i++) {
        buf[i] = work->ids[i];
    }
    pos += count;
    for (i = 0; i < count; i++) {
        buf[pos + i] = ivs[i];
    }
    pos += count;
    for (i = 0; i < count; i++) {
        buf[pos + i] = pids[i];
        buf[count + pos + i] = pids[i] >> 16;
    }
    if (sub_02037030(0x1C, buf, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}
