#include "global.h"

extern int sub_0203769C(void);
extern int sub_02037454(void);
void *ov73_021E6BC8(void *, void *);

void *ov73_021E6BC8(void *work, void *result) {
    if (sub_0203769C() == 0) {
        sub_02037454();
    }
    if (*(u32 *)((u8 *)work + 0x4A08) == sub_02037454()) {
        *(u32 *)((u8 *)work + 0x318) = 26;
    }
    return result;
}
