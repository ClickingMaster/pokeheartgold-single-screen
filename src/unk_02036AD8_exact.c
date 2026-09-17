#include "unk_02036AD8_exact_private.h"

void sub_02036AD8(void) {
    int trainerId;
    u8 *work;

    if (_021D4140.work == NULL) {
        return;
    }
    if (sub_02034044(sub_0203993C())) {
        return;
    }
    trainerId = sub_0203772C((u16)sub_0203993C());
    sub_02033FC4((u16)sub_0203993C());
    if (sub_02033298() != 4) {
        return;
    }
    if (!sub_020373B4(sub_0203769C())) {
        sub_0203769C();
        return;
    }
    if (_0210F900[1] == 2 || _0210F900[1] == 0) {
        if (sub_0203769C()) {
            _0210F900[1]++;
            work = _021D4140.work;
            if (!sub_02033800(work + work[0x6AF] * 64, (u16)trainerId, 14, sub_02036AA0)) {
                _0210F900[1]--;
                return;
            }
            _021D4140.work[0x6AF] = 1 - _021D4140.work[0x6AF];
            (*(u32 *)(_021D4140.work + 0x668))++;
        } else {
            if ((sub_02033250() & 0xFFFE) == 0) {
                return;
            }
            _0210F900[1]++;
            sub_02036AA0((void *)1);
            work = _021D4140.work;
            sub_0203690C(0, work + work[0x6AF] * 64, (u16)trainerId);
            _021D4140.work[0x6AF] = 1 - _021D4140.work[0x6AF];
            (*(u32 *)(_021D4140.work + 0x668))++;
        }
    }
}
