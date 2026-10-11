#pragma force_active on
#include "global.h"
#include "party.h"
#include "pokemon.h"
typedef struct {
    u8 pad0[0x10];
    u8 type;
    u8 pad11;
    u8 fainted;
    u8 pad13[0x28 - 0x13];
    Party *party;
    u8 pad2C[0x374 - 0x2C];
    u8 unk374[0x20];
    u16 maxPP[4][4];
} Ov80Work;
extern int ov80_02237B24(u8, int);
extern int ov80_02237B58(u8, int);
extern BOOL Comm_GetLocalNetId(void);
int ov80_02233020(Ov80Work *w);
// @fn ov80_02233020
int ov80_02233020(Ov80Work *w) {
    int score;
    int ppSum;
    int i, j, k;
    int start;
    int end;
    int n;
    int cnt2;
    int total;
    int diff;
    Pokemon *mon;
    u32 hp, maxHp;
    u8 counts[20];

    score = 0;
    for (i = 0; i < 20; i++) {
        counts[i] = 0;
    }
    ppSum = 0;
    w->fainted = 0;
    if (Comm_GetLocalNetId() == FALSE) {
        start = 0;
    } else {
        start = 2;
    }
    n = ov80_02237B24(w->type, 0);
    cnt2 = ov80_02237B58(w->type, 1);
    end = n + start;
    for (i = start; i < end; i++) {
        mon = Party_GetMonByIndex(w->party, i);
        if (GetMonData(mon, MON_DATA_SPECIES_EXISTS, NULL) == 0) {
            continue;
        }
        hp = GetMonData(mon, MON_DATA_HP, NULL);
        maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
        if (hp != 0) {
            counts[0]++;
            if (hp == maxHp) {
                counts[1]++;
            } else if (hp >= maxHp / 2) {
                counts[2]++;
            } else {
                counts[3]++;
            }
            if (GetMonData(mon, MON_DATA_STATUS, NULL) == 0) {
                counts[4]++;
            }
        } else {
            w->fainted = 1;
        }
        ppSum += GetMonData(mon, MON_DATA_MOVE1_PP, NULL);
        ppSum += GetMonData(mon, MON_DATA_MOVE2_PP, NULL);
        ppSum += GetMonData(mon, MON_DATA_MOVE3_PP, NULL);
        ppSum += GetMonData(mon, MON_DATA_MOVE4_PP, NULL);
    }
    for (i = 0; i < cnt2; i++) {
        if (w->unk374[i] == 1) {
            counts[8]++;
        }
    }
    total = 0;
    for (j = 0; j < n; j++) {
        for (k = 0; k < 4; k++) {
            total += w->maxPP[j][k];
        }
    }
    diff = total - ppSum;
    if (diff <= 5) {
        counts[5]++;
    } else if (diff <= 10) {
        counts[6]++;
    } else if (diff <= 15) {
        counts[7]++;
    }
    score += counts[0] * 3;
    score += counts[1] * 3;
    score += counts[2] * 2;
    score += counts[3];
    score += counts[4];
    score += counts[5] * 8;
    score += counts[6] * 6;
    score += counts[7] * 4;
    score += counts[8] * 7;
    if (score <= 0) {
        score = 1;
    }
    return score;
}
