#pragma force_active on
#include "global.h"
typedef struct SaveBlock {
    u8 data[0xFF4];
    u16 number;
    u16 checksum;
    u32 magic;
    u32 value;
} SaveBlock;
extern u32 ov74_0223D33C[];
void ov74_02233AB8(int sector, SaveBlock *block);
u16 ov74_02233ACC(int number);
u16 ov74_02233A88(SaveBlock *block, u32 size);
int ov74_02233B58(int bank, SaveBlock *block, u32 *out);

// @fn ov74_02233B58
int ov74_02233B58(int bank, SaveBlock *block, u32 *out) {
    u32 flags = 0;
    u8 found = FALSE;
    int i;
    *out = 0;
    ov74_0223D33C[1] = 0;
    for (i = 0; i < 14; i++) {
        ov74_02233AB8(i % 14 + bank * 14, block);
        DC_FlushRange(block, 0x1000);
        if (block->magic == 0x08012025) {
            found = TRUE;
            if (ov74_02233A88(block, ov74_02233ACC(block->number)) == block->checksum) {
                *out = block->value;
                flags |= 1 << block->number;
            }
        }
    }
    if (found) {
        if (flags == 0x3FFF) {
            return 1;
        }
        return 0xFF;
    }
    return 0;
}
