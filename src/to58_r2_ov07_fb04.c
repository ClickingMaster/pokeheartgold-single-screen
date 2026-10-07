#include "global.h"

extern int ov07_0221FAE8(void *);
int ov07_0221FB04(u32, u32);

int ov07_0221FB04(u32 value, u32 kind) {
    switch (kind) {
    case 0: return 0;
    case 1: return 1;
    case 2: return 3;
    case 3: return ov07_0221FAE8((void *)value);
    }
    return value;
}
