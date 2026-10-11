#pragma force_active on
#include "global.h"
typedef struct CommManager3667C {
    u8 initialized;
    u8 updateLock;
    u8 pad02[2];
    void *allocation;
    u8 *work;
} CommManager3667C;
extern CommManager3667C _021D4140;
extern volatile u8 _0210F900[2];
int sub_0203993C(void);
int sub_02033FC4(u16 value);
int sub_02034044(int value);
int Comm_IsNetIdConnected(u16 slot);
int sub_02035FF0(void);
BOOL sub_02036438(int slot);
int sub_02033298(void);
void sub_0203772C(u16);
BOOL sub_02033800(const void *, int, int, void *);
void sub_020367B0(int unused, const u8 *data, int size);
void sub_02036ABC(void);
void sub_02036508(void);
// @fn sub_02036508
void sub_02036508(void) {
    int count, i;
    if (_021D4140.work == NULL) {
        return;
    }
    if (sub_02034044(sub_0203993C())) {
        return;
    }
    sub_0203772C(sub_0203993C());
    count = sub_02033FC4(sub_0203993C()) + 1;
    if (_0210F900[0] != 2 && _0210F900[0] != 0) {
        return;
    }
    _0210F900[0]++;
    if (sub_02035FF0() == 1) {
        if (!_021D4140.initialized) {
            sub_02036438(_021D4140.work[0x6B0]);
            _021D4140.initialized = 1;
        }
    }
    if (sub_02033298() == 4) {
        if (!sub_02033800(_021D4140.work + 0x80 + _021D4140.work[0x6B0] * 0xC0, 0xC0, 14, sub_02036ABC)) {
            _0210F900[0]--;
        }
    }
    if (_0210F900[0] == 1 || _0210F900[0] == 3) {
        _021D4140.initialized = 0;
        for (i = 0; i < count; i++) {
            if (Comm_IsNetIdConnected(i)) {
                (*(int *)(_021D4140.work + i * 4 + 0x66C))++;
            }
        }
        sub_020367B0(0, _021D4140.work + 0x80 + _021D4140.work[0x6B0] * 0xC0, 0xC0);
        _021D4140.work[0x6B0] = 1 - _021D4140.work[0x6B0];
    }
    if (sub_02033298() != 4) {
        _0210F900[0]++;
    }
}
