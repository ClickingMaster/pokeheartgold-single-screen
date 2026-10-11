#include "global.h"
extern u32 PokewalkerApp_StartMessageBox(void *, int, int);
extern void ov112_021EA5A4(void *, int);
extern int TextPrinterCheckActive(int);
extern void PokewalkerApp_CreateWaitingIcon(void *);
int ov112_021EE6B0(void *);
int ov112_021EE6D0(void *);
int ov112_021EE6B0(void *s) {
    *(u32 *)((u8 *)s + 0x1e524) = PokewalkerApp_StartMessageBox(s, 2, 13);
    ov112_021EA5A4(s, 0);
    return 1;
}
int ov112_021EE6D0(void *s) {
    if (!TextPrinterCheckActive(*(u32 *)((u8 *)s + 0x1e524) & 0xff)) {
        PokewalkerApp_CreateWaitingIcon(s);
        return 2;
    }
    return 1;
}
