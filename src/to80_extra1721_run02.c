#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct W {
    u8 p[0xd];
    u8 count;
    u8 pe[7];
    u8 mode;
    u8 p16[0xce];
    void *list;
    u8 pe8[12];
    int slots[1];
} W;

extern W *ov00_021F955C(void);
extern const u8 ov00_02217AAC[];
extern const u8 ov00_02217AD8[];
extern const int ov00_02216040[];
extern int ov00_022139C8(void *);
extern int ov00_02212DB4(void *, const void *, int);
extern void *ov00_022139B8(void *, int);
extern void ov00_0221390C(void *, void *);
extern void ov00_02212D10(void *, const void *, int);
extern void ov00_022139D8(void *, int, const void *, int);
extern u32 ov00_021ED5E4(u32);
int ov00_021F89D0(int);
void ov00_021F8BA8(void);

static inline u32 combine(u32 q, u32 p) {
    return q + p;
}

void ov00_021F8BA8(void) {
    u32 random;
    int cumulative[6];
    int largest = 0, sum = 0, i;
    u32 prior;
    if (ov00_022139C8(ov00_021F955C()->list) <= 1) {
        return;
    }
    for (i = 0; i < ov00_022139C8(ov00_021F955C()->list); i++) {
        int value = ov00_02212DB4(ov00_022139B8(ov00_021F955C()->list, i), ov00_02217AD8, -1);
        if (value > largest) {
            largest = value;
        }
        sum += ov00_02216040[i];
    }
    random = ov00_021ED5E4(100);
    for (i = 0; i < ov00_022139C8(ov00_021F955C()->list); i++) {
        if (i == ov00_022139C8(ov00_021F955C()->list) - 1) {
            break;
        }
        if (i > 0) {
            prior = cumulative[i - 1];
        } else {
            prior = 0;
        }
        cumulative[i] = combine(ov00_02216040[i] * 100 / sum, prior);
        if (random < (u32)cumulative[i]) {
            break;
        }
    }
    if (largest < 0x7fffffff) {
        largest++;
    }
    ov00_02212D10(ov00_022139B8(ov00_021F955C()->list, i), ov00_02217AD8, largest);
    ov00_022139D8(ov00_021F955C()->list, 0, ov00_02217AD8, 0);
}
