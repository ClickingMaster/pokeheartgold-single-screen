#include "global.h"

extern u16 *ov74_02233F8C(void *box, u32 personality, int block);

u16 ov74_022344A8(void *box);
u16 ov74_022344A8(void *box) {
    u16 *block2;
    u16 *block1;
    u16 *block0;
    u16 *block3;
    u16 sum = 0;
    int i;

    block0 = ov74_02233F8C(box, *(u32 *)box, 0);
    block1 = ov74_02233F8C(box, *(u32 *)box, 1);
    block2 = ov74_02233F8C(box, *(u32 *)box, 2);
    block3 = ov74_02233F8C(box, *(u32 *)box, 3);
    for (i = 0; i < 6; i++) sum += block0[i];
    for (i = 0; i < 6; i++) sum += block1[i];
    for (i = 0; i < 6; i++) sum += *block2++;
    for (i = 0; i < 6; i++) sum += *block3++;
    return sum;
}
