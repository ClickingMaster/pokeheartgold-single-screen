#include "global.h"
extern u32 ov112_021EA08C(void *, int, int);
extern void ov112_021EA5A4(void *, int);
extern int TextPrinterCheckActive(int);
extern void ov112_021EA17C(void *);
int ov112_021EE6B0(void *);
int ov112_021EE6D0(void *);
int ov112_021EE6B0(void *s) {
    *(u32 *)((u8 *)s + 0x1e524) = ov112_021EA08C(s, 2, 13);
    ov112_021EA5A4(s, 0);
    return 1;
}
int ov112_021EE6D0(void *s) {
    if (!TextPrinterCheckActive(*(u32 *)((u8 *)s + 0x1e524) & 0xff)) {
        ov112_021EA17C(s);
        return 2;
    }
    return 1;
}
