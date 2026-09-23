#include "global.h"
extern int sub_0203263C(void *);
int ov112_021EE718(void *);
int ov112_021EE718(void *s) {
    if (sub_0203263C(*(void **)((u8 *)s + 0x1e440)) == 1) return 5;
    *(int *)((u8 *)s + 4) = 10;
    *(int *)((u8 *)s + 0x10) = 3;
    return 7;
}
