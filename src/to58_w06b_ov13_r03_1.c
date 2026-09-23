#include "global.h"
extern char ov13_02245A14[];
extern char ov13_02245A20[];
extern int ov13_02222A9C(const char *);
extern void ov13_02222968(void *, const void *, u32);
extern int ov13_02222948(const u8 *, const u8 *, int);
int ov13_022216E0(void *src);
int ov13_0222175C(void *out);
int ov13_0222175C(void *out) {
    u8 *p = out;
    *(u32 *)p = ov13_02222A9C(ov13_02245A14);
    ov13_02222968(p + 4, ov13_02245A14, *(u32 *)p);
    *(u32 *)(p + 0x24) = 1;
    *(u32 *)(p + 0x28) = ov13_02222A9C(ov13_02245A20);
    if (*(u32 *)(p + 0x28) > 13) return -1;
    ov13_02222968(p + 0x2C, ov13_02245A20, *(u32 *)(p + 0x28));
    return 0;
}
