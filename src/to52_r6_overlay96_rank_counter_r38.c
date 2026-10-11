#include "to52_r6_overlay96_rank_counter_r38_private.h"

int ov96_021ECC58(const void *a, const void *b) {
    u16 right = *(const u32 *)b;
    u16 left = *(const u32 *)a;

    if (left == right) {
        return 0;
    }
    if (left < right) {
        return 1;
    }
    return -1;
}

void ov96_021ECC7C(void *source, Ov96RankEntryR38 *values, const u8 *ranks) {
    int i;
    int group = 0;

    for (i = 0; i < 4; i++) {
        values[i].sourceIndex = i;
        values[i].count = ov96_021ECC4C(source, (u8)i);
        values[i].inputRank = ranks[i];
    }

    MATH_QSort(values, 4, sizeof(Ov96RankEntryR38), ov96_021ECC58, NULL);

    for (i = 0; i < 4; i++) {
        values[i].group = group;
        if (i != 3 && values[i].count != values[i + 1].count) {
            group = i + 1;
        }
    }
}

void ov96_021ECD24(Ov96CounterDisplayR38 *display) {
    u32 current;
    u16 hundreds;
    u16 tens;
    u16 ones;
    int i;

    if (display->active != 0) {
        current = display->current + 1;
        if (current > 999) {
            current = 999;
        }
        display->current = current;
        if (!IsSEPlaying(0x8E3)) {
            PlaySE(0x8E3);
        }

        hundreds = PokeathlonUtil_GetDecimalDigit(display->current, 3);
        tens = PokeathlonUtil_GetDecimalDigit(display->current, 2);
        ones = PokeathlonUtil_GetDecimalDigit(display->current, 1);
        ManagedSprite_SetAnim(display->sprites[1], hundreds + 1);
        ManagedSprite_SetAnim(display->sprites[2], tens + 1);
        ManagedSprite_SetAnim(display->sprites[3], ones + 1);

        if (display->current % 20 == 0) {
            for (i = 0; i < 4; i++) {
                ManagedSprite_OffsetPositionXY(display->sprites[i], 0, -1);
            }
        }
        if (display->current >= display->limit) {
            display->active = 0;
        }
    }
}
