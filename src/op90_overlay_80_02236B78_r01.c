#pragma force_active on
#include "global.h"
#include "math_util.h"
typedef struct {
    u16 species;
    u16 moves[4];
    u8 evFlags;
    u8 nature;
    u16 item;
    u16 unk_0e;
} FrontierMonSrc;
typedef struct {
    u16 unk0;
    u16 min;
    u16 max;
    u8 iv;
    u8 useAlt;
    u8 pad8[2];
    u16 altMin;
    u16 altMax;
    u8 altIv;
} FrontierRange;
extern const int ov80_0223C464[5];
extern void FrontierData_ReadMember(void *, int, int);
int ov80_02236C9C(const u16 *species, const u16 *items, int count, int num, u16 *out, enum HeapID heapId, const FrontierRange *range, u16 streak, u8 *ivs);
// @fn ov80_02236C9C
int ov80_02236C9C(const u16 *species, const u16 *items, int count, int num, u16 *out, enum HeapID heapId, const FrontierRange *range, u16 streak, u8 *ivs) {
    FrontierMonSrc mons[6];
    int i, j;
    int tier;
    int iv;
    int span;
    int id;
    FrontierMonSrc *cur;
    u16 *dst;
    GF_ASSERT(num <= 6);
    i = 0;
    span = range->max - range->min;
    tier = 0;
    if (streak != 0) {
        tier = 5;
        for (j = 0; j < NELEMS(ov80_0223C464); j++) {
            if (streak <= ov80_0223C464[j]) {
                tier = j;
                break;
            }
        }
    }
    if (num != 0) {
    cur = mons;
    dst = out;
    do {
        if (i >= num - tier && range->useAlt == 1) {
            span = range->altMax - range->altMin;
            id = range->altMax - LCRandom() % (span + 1);
            iv = range->altIv;
        } else {
            id = range->max - LCRandom() % (span + 1);
            iv = range->iv;
        }
        FrontierData_ReadMember(cur, id, 0xCD);
        for (j = 0; j < i; j++) {
            FrontierMonSrc *other = &mons[j];
            if (other->species == cur->species || other->item == cur->item) {
                break;
            }
        }
        if (j == i) {
            for (j = 0; j < count; j++) {
                if (cur->species == species[j] || cur->item == items[j]) {
                    break;
                }
            }
            if (j == count) {
                *dst = id;
                ivs[i] = iv;
                i++;
                cur++;
                dst++;
            }
        }
    } while (i != num);
    }
    return 0;
}
