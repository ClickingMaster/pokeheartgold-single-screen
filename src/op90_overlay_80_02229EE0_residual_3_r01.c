#pragma force_active on
#include "global.h"
#include "assert.h"
#include "math_util.h"

typedef struct FrontierTrainer {
    u16 unk0;
    u16 numMons;
    u16 monIds[];
} FrontierTrainer;

typedef struct FrontierMonData {
    u16 species;
    u16 unk2[5];
    u16 item;
    u16 unkE;
} FrontierMonData;

void ov80_02229EF4(FrontierMonData *mon, u16 id, int heapID);
BOOL ov80_0222A5E0(FrontierTrainer *trainer, const u16 *species, const u16 *items, int count, int num, u16 *out);

// @fn ov80_0222A5E0
BOOL ov80_0222A5E0(FrontierTrainer *trainer, const u16 *species, const u16 *items, int count, int num, u16 *out) {
    FrontierMonData mons[6];
    u16 id;
    int tries;
    int i, j;

    GF_ASSERT(num <= 6);
    i = 0;
    tries = 0;
    while (i != num) {
        id = trainer->monIds[LCRandom() % trainer->numMons];
        ov80_02229EF4(&mons[i], id, 205);
        for (j = 0; j < i; j++) {
            if (mons[j].species == mons[i].species || mons[j].item == mons[i].item) {
                break;
            }
        }
        if (j != i) {
            continue;
        }
        if (tries < 50) {
            for (j = 0; j < count; j++) {
                if (mons[i].species == species[j] || mons[i].item == items[j]) {
                    break;
                }
            }
            if (j != count) {
                tries++;
                continue;
            }
        }
        out[i] = id;
        i++;
    }
    return tries >= 50;
}
