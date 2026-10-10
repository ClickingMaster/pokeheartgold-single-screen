#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 p[0xd];
    u8 mode, count;
    u8 p2[6];
    u8 type, number;
    u8 p3[0x2b9];
    u8 order[32];
    u32 mask;
} Work;

extern Work *ov00_021F955C(void);
extern u8 ov00_0221B080[];
extern char ov00_02217A64[];
extern char ov00_02217AA4[];
extern char ov00_02217AA8[];
extern char ov00_02217A70[];
extern void ov00_021ED470(const char *, const char *, void *, int);
extern void ov00_021ED4A8(const char *, const char *, void *, int);
extern int ov00_021F04A0(int, void *, int);
int ov00_021F3618(u8 **), ov00_021F36BC(void);

int ov00_021F3618(u8 **out) {
    int i;
    if (!ov00_021F955C()) {
        return 0;
    }
    MI_CpuFill8(ov00_0221B080, 0, 32);
    for (i = 0; i <= ov00_021F955C()->count; i++) {
        Work *w = ov00_021F955C();
        if (!(w->mask & (1 << ov00_021F955C()->order[i]))) {
            break;
        }
        ov00_0221B080[i] = ov00_021F955C()->order[i];
    }
    *out = ov00_0221B080;
    return ov00_021F955C()->count + 1;
}

int ov00_021F36BC(void) {
    char name[12];
    u8 buffer[32];
    if (ov00_021F955C()->type != 2) {
        return 0;
    }
    OS_SNPrintf(name, 12, ov00_02217A64, ov00_021F955C()->number + 1);
    ov00_021ED470(ov00_02217AA4, name, buffer, 47);
    OS_SNPrintf(name, 12, ov00_02217A64, ov00_021F955C()->mode + 1);
    ov00_021ED4A8(ov00_02217AA8, name, buffer, 47);
    OS_SNPrintf(name, 12, ov00_02217A64, 3);
    ov00_021ED4A8(ov00_02217A70, name, buffer, 47);
    return ov00_021F04A0(6, buffer, 0);
}
