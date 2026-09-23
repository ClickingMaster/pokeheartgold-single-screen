#include "global.h"
extern u32 ov112_021EA08C(void *, int, int);
extern void ov112_021EA5A4(void *, int);
extern int TextPrinterCheckActive(int);
extern int sub_02006B84(void);
extern int IsFanfarePlaying(void);
extern void ov112_021EA17C(void *);
int ov112_021EEE2C(void *);
int ov112_021EEE4C(void *);
int ov112_021EEE2C(void *s) {
    *(u32 *)((u8 *)s + 0x1e524) = ov112_021EA08C(s, 2, 13);
    ov112_021EA5A4(s, 0);
    return 1;
}
int ov112_021EEE4C(void *s) {
    if (!TextPrinterCheckActive(*(u32 *)((u8 *)s + 0x1e524) & 0xff) &&
        *(u16 *)((u8 *)s + 0x1f2d6) == 31 && !sub_02006B84()) {
        IsFanfarePlaying();
        ov112_021EA17C(s);
        return 2;
    }
    return 1;
}
