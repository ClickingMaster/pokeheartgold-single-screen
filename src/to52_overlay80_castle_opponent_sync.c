#include "global.h"

typedef struct Ov80CastleWork {
    u8 pad000[0x26C];
    u16 monSetIDs[4];
    u8 opponentMonIVs[4];
    u32 opponentMonPersonalities[4];
    u8 pad288[0x3C0 - 0x288];
    u16 commBuffer[40];
    u8 pad410[0xA10 - 0x410];
    u8 unkA10;
    u8 unkA11;
    u16 unkA12;
    u16 padA14;
    u16 unkA16;
    u8 unkA18;
    u8 unkA19;
    u8 msgsReceived;
    u8 unkA1B;
    u16 unkA1C;
    u16 padA1E;
    u16 *unkA20;
} Ov80CastleWork;

extern u32 sub_0203769C(void);
extern BOOL sub_02037030(u32, void *, u32);

void ov80_0222B740(int, int, void *, void *);
BOOL ov80_0222B7E4(Ov80CastleWork *);
void ov80_0222B860(int, int, void *, void *);

void ov80_0222B740(int netID, int unused, void *param2, void *work) {
    Ov80CastleWork *battleCastle_dupe = work;
    const u16 *param2_dupe = param2;

    battleCastle_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleCastle_dupe->unkA19 = param2_dupe[0];

    if (sub_0203769C() == 0) {
        if (battleCastle_dupe->unkA1B != 0) {
            if (battleCastle_dupe->unkA1B == 4) {
                if (battleCastle_dupe->unkA19 != 4) {
                    battleCastle_dupe->unkA18 = battleCastle_dupe->unkA19 + 6;
                    battleCastle_dupe->unkA1B = battleCastle_dupe->unkA19 + 6;
                }
            }
        } else {
            battleCastle_dupe->unkA1B = battleCastle_dupe->unkA19 + 6;

            if (battleCastle_dupe->unkA19 != 4) {
                *battleCastle_dupe->unkA20 = 0xEEDD;
            }
        }
    } else {
        battleCastle_dupe->unkA1B = param2_dupe[1];

        if (battleCastle_dupe->unkA1B != 4) {
            *battleCastle_dupe->unkA20 = 0xEEDD;
        }

        if (battleCastle_dupe->unkA19 == 4) {
            if (battleCastle_dupe->unkA18 != 0) {
                if (battleCastle_dupe->unkA18 != 4) {
                    battleCastle_dupe->unkA1B = battleCastle_dupe->unkA18 + 6;
                }
            }
        }
    }
}

BOOL ov80_0222B7E4(Ov80CastleWork *battleCastle) {
    int i, offset, result, dataSize;

    offset = 0;
    dataSize = 40;

    for (i = 0; i < 4; i++) {
        battleCastle->commBuffer[i] = battleCastle->monSetIDs[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleCastle->commBuffer[i + offset] = battleCastle->opponentMonIVs[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleCastle->commBuffer[i + offset] = (battleCastle->opponentMonPersonalities[i] & 0xFFFF);
        battleCastle->commBuffer[i + offset + 4] = ((battleCastle->opponentMonPersonalities[i] >> 16) & 0xFFFF);
    }

    offset += (4 * 2);

    if (sub_02037030(0x2D, battleCastle->commBuffer, dataSize) == 1) {
        result = TRUE;
    } else {
        result = FALSE;
    }

    return result;
}

void ov80_0222B860(int netID, int unused, void *monData, void *work) {
    int i, offset;
    Ov80CastleWork *battleCastle_dupe = work;
    const u16 *monData_dupe = monData;

    offset = 0;
    battleCastle_dupe->msgsReceived++;

    if (sub_0203769C() == netID || sub_0203769C() == 0) {
        return;
    }

    for (i = 0; i < 4; i++) {
        battleCastle_dupe->monSetIDs[i] = monData_dupe[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleCastle_dupe->opponentMonIVs[i] = monData_dupe[i + offset];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleCastle_dupe->opponentMonPersonalities[i] = monData_dupe[i + offset];
        battleCastle_dupe->opponentMonPersonalities[i] |= (monData_dupe[i + offset + 4] << 16);
    }

    offset += (4 * 2);
}
