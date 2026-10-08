#pragma force_active on
#include "global.h"
#include "player_data.h"
typedef union TrainerKey {
    struct {
        u32 id;
        u32 valid;
    } s;
    u64 raw;
} TrainerKey;
typedef struct ProfileSlot {
    PlayerProfile *cur;
    PlayerProfile *prev;
} ProfileSlot;
typedef struct KeySlot {
    TrainerKey cur;
    TrainerKey prev;
} KeySlot;
typedef struct Ov73Work {
    u8 unk000[0x338];
    ProfileSlot profiles[5];
    int states[5];
    u8 unk374[0x46C8];
    KeySlot keys[5];
} Ov73Work;
PlayerProfile *sub_02034818(int id);
BOOL ov73_021E7490(Ov73Work *work);

// @fn ov73_021E7490
BOOL ov73_021E7490(Ov73Work *work) {
    int i;
    BOOL changed = FALSE;
    for (i = 0; i < 5; i++) {
        work->profiles[i].prev = work->profiles[i].cur;
        work->profiles[i].cur = sub_02034818(i);
        work->keys[i].prev = work->keys[i].cur;
        if (work->profiles[i].cur != NULL) {
            work->keys[i].cur.s.id = PlayerProfile_GetTrainerID(work->profiles[i].cur);
            work->keys[i].cur.s.valid = TRUE;
        } else {
            work->keys[i].cur.s.id = 0;
            work->keys[i].cur.s.valid = FALSE;
        }
    }
    for (i = 0; i < 5; i++) {
        if (work->profiles[i].prev != work->profiles[i].cur) {
            changed = TRUE;
            if (work->profiles[i].cur == NULL) {
                work->states[i] = 3;
            } else {
                work->states[i] = 1;
            }
        } else if (work->keys[i].cur.raw != work->keys[i].prev.raw) {
            changed = TRUE;
            if (work->keys[i].cur.s.valid == FALSE) {
                work->states[i] = 3;
            } else {
                work->states[i] = 1;
            }
        }
    }
    return changed;
}
