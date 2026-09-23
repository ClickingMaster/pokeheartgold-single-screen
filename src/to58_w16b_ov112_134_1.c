#include "global.h"
extern int (*ov112_021FF8F8[])(void *);
int ov112_021EEDFC(void *);
int ov112_021EEDFC(void *s) {
    int result = ov112_021FF8F8[*(int *)((u8 *)s + 8)](s);
    *(int *)((u8 *)s + 8) = result;
    if (result == 11) {
        *(int *)((u8 *)s + 8) = 0;
        return 3;
    }
    return 2;
}
