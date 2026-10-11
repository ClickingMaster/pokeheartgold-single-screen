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
BOOL Comm_SendCommand(int cmd, void *data, int size);
int Comm_GetLocalNetId(void);
BOOL ov80_0222B448(Ov80Work *work);
void ov80_0222B52C(int netId, int size, u16 *data, Ov80Work *work);

// @fn ov80_0222B52C
void ov80_0222B52C(int netId, int size, u16 *data, Ov80Work *work) {
    int i;
    int count;
    int pos = 0;

    work->recvCount++;
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    count = ov80_02236DD4(work->mode);
    for (i = 0; i < count; i++) {
        work->recvIds[i] = data[i];
    }
    pos += count;
    for (i = 0; i < count; i++) {
        work->recvIVs[i] = data[pos + i];
    }
    pos += count;
    for (i = 0; i < count; i++) {
        work->recvPids[i] = data[pos + i];
        work->recvPids[i] |= data[count + pos + i] << 16;
    }
}
