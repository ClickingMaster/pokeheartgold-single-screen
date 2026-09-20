#include "global.h"

extern u8 ov13_0224DF30[];
extern u8 ov13_02245A58[];
extern int ov13_02224F3C(void);
extern void ov13_02224100(void);
extern void ov13_02226F3C(void);

void ov13_02225320(void *arg);

void ov13_02225320(void *arg) {
    int result = ov13_02224F3C();

    *(int *)(ov13_0224DF30 + 0x78) = result;
    ov13_02224100();
    if (result == 1) {
        *(int *)(ov13_0224DF30 + 0x38) = 6;
    } else {
        *(int *)(ov13_0224DF30 + 0x38) = 7;
    }
    *(int *)(ov13_02245A58 + 0xc) = -1;
    ov13_02226F3C();
}
