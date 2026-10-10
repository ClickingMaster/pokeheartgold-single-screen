#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 p;
    void **callback;
    u8 a[5];
    u8 count;
    u8 b[0x17 - 0xe];
    u8 flag;
    u8 c[8];
    u32 value;
    u32 values[32];
    u16 shorts[40];
    u32 words[40];
    u8 d[0x1a8 - 0x194];
    u8 active;
    u8 e[0x348 - 0x1a9];
    int counter;
} Work;

extern Work *ov00_021F955C(void);
extern int ov00_021F4304(int, u32, u32, u16, int *, int);
extern void ov00_0220D238(void *);
int ov00_021F63DC(void);

int ov00_021F63DC(void) {
    int i = 1, r;
    while (i <= ov00_021F955C()->count) {
        Work *a = ov00_021F955C(), *b = ov00_021F955C(), *c = ov00_021F955C(), *d = ov00_021F955C(), *e = ov00_021F955C();
        r = ov00_021F4304(10, a->words[i], b->values[i], c->shorts[i], &d->counter, e->counter + 1);
        if (r) {
            return r;
        }
        i++;
    }
    ov00_021F955C()->flag = 0;
    ov00_021F955C()->value = 0;
    ov00_021F955C()->active = 1;
    ov00_0220D238(*ov00_021F955C()->callback);
    ov00_021F955C()->active = 0;
    return 0;
}
