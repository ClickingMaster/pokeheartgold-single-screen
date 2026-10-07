#pragma force_active on
#include "global.h"
#include "party.h"
#include "pokemon.h"

typedef struct Ov80Work {
    u8 pad0[0x10];
    u8 type;
} Ov80Work;

extern const u8 ov80_0223BE90[];
extern const u8 ov80_0223BE88[];
BOOL BattleArcade_MultiplayerCheck(u8 type);
int ov80_02234894(Ov80Work *work, Party *party1, Party *party2, u8 count);

// @fn ov80_02234894
int ov80_02234894(Ov80Work *work, Party *party1, Party *party2, u8 count) {
    u8 fainted, statused;
    int result;
    int i;
    Pokemon *mon;

    result = 0;
    fainted = 0;
    statused = 0;
    for (i = 0; i < count; i++) {
        mon = Party_GetMonByIndex(party1, i);
        if (GetMonData(mon, MON_DATA_SPECIES_EXISTS, NULL)) {
            if (GetMonData(mon, MON_DATA_HP, NULL) == 0) {
                fainted++;
            }
            if (GetMonData(mon, MON_DATA_STATUS, NULL)) {
                statused++;
            }
        }
    }
    if (BattleArcade_MultiplayerCheck(work->type) == TRUE) {
        for (i = 0; i < count; i++) {
            mon = Party_GetMonByIndex(party2, i);
            if (GetMonData(mon, MON_DATA_SPECIES_EXISTS, NULL)) {
                if (GetMonData(mon, MON_DATA_HP, NULL) == 0) {
                    fainted++;
                }
                if (GetMonData(mon, MON_DATA_STATUS, NULL)) {
                    statused++;
                }
            }
        }
    }
    result += ov80_0223BE90[statused];
    result += ov80_0223BE88[fainted];
    return result;
}
