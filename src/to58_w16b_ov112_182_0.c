#include "global.h"
extern void ov112_021F2000(void *, void *, void *, int, int, int, int);
extern void ov112_021F1F3C(void *, int);
void ov112_021F1F80(void *);
void ov112_021F1F80(void *s) {
    ov112_021F2000((u8 *)s + 0x98, *(void **)((u8 *)s + 0x68), *(void **)((u8 *)s + 0x6c), 0x68, 0x48, 0, 0);
    ov112_021F1F3C((u8 *)s + 0x98, 3);
    ov112_021F2000((u8 *)s + 0xac, *(void **)((u8 *)s + 0x68), *(void **)((u8 *)s + 0x6c), 0x98, 0x48, 0, 1);
    ov112_021F1F3C((u8 *)s + 0xac, 1);
}
