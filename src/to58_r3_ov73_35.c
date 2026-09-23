#include "global.h"

extern int TextPrinterCheckActive(u8);
int ov73_021E9ECC(void *);

int ov73_021E9ECC(void *work) {
    if (!TextPrinterCheckActive((u8)*(u32 *)((u8 *)work + 0xBF4))) {
        if (*(s32 *)((u8 *)work + 0xF20) > 30) {
            *(u32 *)((u8 *)work + 0x1c) = *(u32 *)((u8 *)work + 0x20);
        }
        ++*(u32 *)((u8 *)work + 0xF20);
    }
    return 3;
}
