#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern int ArrayLength(void *);
extern int ov00_02212E78(void *);
extern int ov00_02212EA4(void *);
extern void **ov00_021FE6EC(void *, int);
int ov00_02213CFC(void *, int, int);

int ov00_02213CFC(void *w, int a, int b) {
    int n = ArrayLength(PTR(w, 4));
    void *p;
    int i;
    for (i = 0; i < n; i++) {
        p = *ov00_021FE6EC(PTR(w, 4), i);
        if (a == ov00_02212E78(p) && b == ov00_02212EA4(p)) {
            return i;
        }
    }
    return -1;
}
