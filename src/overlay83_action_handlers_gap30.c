#include "overlay83_action_handlers_gap30_internal.h"
typedef struct FlagBits {
    u8 active : 1;
    u8 rest : 7;
} FlagBits;
#define ACTIVE(p) (((FlagBits *)&(p)[0xf])->active)
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
int ov83_02244E24(u8 *work, int unused, u32 state) {
    int idx = ov83_02247768(work[0x14], unused);
    Party_GetMonByIndex(PTR(work, 0x55c), idx);
    switch (state) {
    case 0:
        work[0xf] &= ~1;
        return 1;
    case 1:
        if (ACTIVE(work) == 0) {
            ACTIVE(work) = 1;
            int seq = work[0x12] == 1 ? 9 : 10;
            int base = ov80_02237D8C(work[9]) == 0 ? 0x50 : 0x30;
            s16 x = base + idx * 0x40;
            PTR(work, 0x504) = ov83_02247454(work + 0x2c8, 0, 0, 0, seq, x, 0x32, 0, 0);
        }
        if (ov83_02247624(PTR(work, 0x504)) == 0) {
            ov83_0224753C(PTR(work, 0x504));
            PTR(work, 0x504) = NULL;
            work[0xf] &= ~1;
            return 1;
        }
        break;
    case 3:
        if (ov80_02237D8C(work[9]) == 0) {
            ov83_02246114(work, 0);
            work[0xf] &= ~1;
            return 1;
        }
        work[0xf] &= ~1;
        return 1;
    case 4:
        if (ov80_02237D8C(work[9]) == 0) {
            ov83_02246114(work, 0);
            work[0xf] &= ~1;
            return 1;
        }
        work[0xf] &= ~1;
        return 1;
    case 5:
        work[0xf] &= ~1;
        return 1;
    }
    return 0;
}
int ov83_02244F60(u8 *work, int unused, int state) {
    u8 limit = work[0x15];
    int idx = ov83_02247768(work[0x14], unused);
    Party_GetMonByIndex(PTR(work, 0x55c), idx);
    switch (state) {
    case 0:
    case 1:
        work[0xf] &= ~1;
        return 1;
    case 5:
        if (ACTIVE(work) == 0) {
            ov83_02246114(work, 0);
        }
        work[0xf] &= ~1;
        return 1;
    case 3:
        if (ov80_02237D8C(work[9]) == 1) {
            if (sub_0203769C() == 0) {
                if (work[0x11] >= limit) {
                    work[0xf] &= ~1;
                    return 1;
                }
            } else if (work[0x11] < limit) {
                work[0xf] &= ~1;
                return 1;
            }
        }
        if (ACTIVE(work) == 0) {
            work[0xf] = (work[0xf] & ~0xf8) | 0x18;
            work[0xf] &= ~1;
            return 1;
        }
        break;
    case 4:
        if (ov80_02237D8C(work[9]) == 1) {
            if (sub_0203769C() == 0) {
                if (work[0x11] >= limit) {
                    work[0xf] &= ~1;
                    return 1;
                }
            } else if (work[0x11] < limit) {
                work[0xf] &= ~1;
                return 1;
            }
        }
        if (ACTIVE(work) == 0) {
            work[0xf] = (work[0xf] & ~0xf8) | 0x18;
            work[0xf] &= ~1;
            return 1;
        }
        break;
    }
    return 0;
}
