#include "global.h"

extern void WirelessStatusIcon_Destroy(void);
extern void sub_0203946C(void);
extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);
int ov73_021E9D14(void *);

int ov73_021E9D14(void *work) {
    WirelessStatusIcon_Destroy();
    sub_0203946C();
    BeginNormalPaletteFade(0, 0, 0, 0, 8, 1, 0x96);
    *(u32 *)((u8 *)work + 0x1c) = 0;
    return 4;
}
