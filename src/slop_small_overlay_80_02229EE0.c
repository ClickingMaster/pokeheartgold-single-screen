#include "global.h"

extern const u16 ov80_0223B628[];
extern const u16 ov80_0223B62A[];

u16 ov80_0222A30C(u16 abilityId);

u16 ov80_0222A30C(u16 abilityId) {
    u32 i = 0;
    const u16 *key = ov80_0223B628;

    do {
        if (abilityId == *key) {
            return ov80_0223B62A[i * 2];
        }
        i++;
        key += 2;
    } while (i < 0x3f);

    return 3;
}
