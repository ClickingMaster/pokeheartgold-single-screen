#include "global.h"

extern u16 *ov74_02233F8C(void *box, u32 personality, int block);
extern u16 ov74_022344A8(void *box);
extern void ov74_02234468(void *box);
extern void ov74_02234488(void *box);

void ov74_022348B0(u8 *box, int field, const u8 *value);
void ov74_022348B0(u8 *box, int field, const u8 *value) {
    u8 *data0;
    u8 *data3;
    int i;
    data0 = NULL;
    data3 = NULL;
    if (field > 10) {
        data0 = (u8 *)ov74_02233F8C(box, *(u32 *)box, 0);
        ov74_02233F8C(box, *(u32 *)box, 1);
        ov74_02233F8C(box, *(u32 *)box, 2);
        data3 = (u8 *)ov74_02233F8C(box, *(u32 *)box, 3);
        ov74_02234468(box);
        u16 checksum = ov74_022344A8(box);
        u16 stored = *(u16 *)(box + 0x1c);
        if (checksum != stored) {
            box[0x13] = (box[0x13] & ~1) | 1;
            box[0x13] |= 4;
            *(u32 *)(data3 + 4) |= 0x40000000;
            ov74_02234488(box);
            return;
        }
    }
    switch (field) {
    case 2:
        for (i = 0; i < 10; i++) box[i + 8] = value[i];
        break;
    case 3: box[0x12] = value[0]; break;
    case 4: box[0x13] = (box[0x13] & ~1) | (value[0] & 1); break;
    case 5: box[0x13] = (box[0x13] & ~2) | ((value[0] & 1) << 1); break;
    case 6: box[0x13] = (box[0x13] & ~4) | ((value[0] & 1) << 2); break;
    case 7:
        for (i = 0; i < 7; i++) box[i + 0x14] = value[i];
        break;
    case 11:
        *(u16 *)data0 = value[0] + (value[1] << 8);
        if (*(u16 *)data0 != 0) box[0x13] |= 2;
        else box[0x13] &= ~2;
        break;
    }
    if (field > 10) {
        *(u16 *)(box + 0x1c) = ov74_022344A8(box);
        ov74_02234488(box);
    }
}
