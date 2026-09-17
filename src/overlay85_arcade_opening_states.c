#include "overlay85_helpers_internal.h"

int ov85_021E8660(BattleArcadeWork *work);

BOOL ov85_021E5B48(BattleArcadeWork *work);
BOOL ov85_021E5B78(BattleArcadeWork *work);
BOOL ov85_021E5B98(BattleArcadeWork *work);
BOOL ov85_021E5BC8(BattleArcadeWork *work);

BOOL ov85_021E5B48(BattleArcadeWork *work) {
    u16 value = 1;
    if (sub_02096D4C((void *)work->unk_0D0, 8, &value, 2) == 1) {
        work->unk_000 = 4;
        return TRUE;
    }
    return FALSE;
}

BOOL ov85_021E5B78(BattleArcadeWork *work) {
    int expected = ov85_021E8660(work) + 1;
    if (expected != sub_02037454()) {
        return FALSE;
    }
    work->unk_000 = 5;
    return TRUE;
}

BOOL ov85_021E5B98(BattleArcadeWork *work) {
    work->unk_030 = ov85_021E8660(work) + 1;
    if (sub_02096D4C((void *)work->unk_0D0, 13, &work->unk_030, 4) == 1) {
        work->unk_014 = 0;
        work->unk_000 = 6;
    }
    return FALSE;
}

BOOL ov85_021E5BC8(BattleArcadeWork *work) {
    u16 values[2];
    int bit;
    int count;

    if (work->unk_014 == 0) {
        values[1] = 0;
        values[0] = 0;
        if (sub_02096D4C((void *)work->unk_0D0, 12, values, 4) == 1) {
            work->unk_014++;
        }
        return FALSE;
    }

    bit = 1;
    count = 1;
    for (; bit < 5; bit++) {
        if ((1 << bit) & *(u16 *)((u8 *)(void *)work->unk_0D0 + 0x42)) {
            if (count >= (int)work->unk_014) {
                values[1] = bit;
                values[0] = work->unk_014;
                if (sub_02096D4C((void *)work->unk_0D0, 12, values, 4) == 1) {
                    work->unk_014++;
                }
                break;
            }
            count++;
        }
    }
    if ((s32)work->unk_014 >= (s32)work->unk_030) {
        work->unk_000 = 7;
    }
    return FALSE;
}
