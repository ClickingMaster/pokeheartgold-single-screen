#pragma force_active on
#include "global.h"

typedef struct Ov74SaveBlock {
    u8 data[0xFF4];
    u16 kind;
    u16 checksum;
    u32 magic;
    u32 value;
} Ov74SaveBlock;

typedef struct Ov74SaveState {
    u32 page;
    u32 unk4;
    Ov74SaveBlock *buffer;
    u32 selected;
    u32 value;
} Ov74SaveState;

extern u32 ov74_0223D338;
extern Ov74SaveState ov74_0223D33C;
extern u32 ov74_0223D34C;

// The original file owned ov74_0223D33C (its literal pool holds it twice); the second entry is reached through the
// preceding 4-byte label.
#define SAVE_STATE ((Ov74SaveState *)(&ov74_0223D338 + 1))

extern int ov74_02233C08(Ov74SaveBlock *buffer, Ov74SaveState *state, u32 *out);
extern void ov74_02233AB8(int sector, Ov74SaveBlock *buffer);
extern u32 ov74_02233ACC(u16 kind);
extern u16 ov74_02233A88(Ov74SaveBlock *block, u32 size);
extern void *ov74_02233B04(u16 kind);

int ov74_02233CE4(void);

// @fn ov74_02233CE4
int ov74_02233CE4(void)
{
    int result = ov74_02233C08(ov74_0223D33C.buffer, SAVE_STATE, &ov74_0223D34C);
    Ov74SaveState *state;

    if (result != 1) {
        switch (result) {
        case 0xFF:
            return 5;
        case 0:
            return 7;
        case 2:
            return 6;
        }
    }
    state = &ov74_0223D33C;
    if (state->page <= 1) {
        int i;
        Ov74SaveBlock *block = state->buffer;
        u32 mask = 0;
        for (i = 0; i < 14; i++) {
            ov74_02233AB8(i + state->page * 14, block);
            if (block->magic == 0x08012025 && block->checksum == ov74_02233A88(block, ov74_02233ACC(block->kind))) {
                u32 size;
                state->value = block->value;
                mask |= 1 << block->kind;
                size = ov74_02233ACC(block->kind);
                MIi_CpuCopy32((const u32 *)block, (u32 *)ov74_02233B04(block->kind), size);
                if (block->kind == 0) {
                    state->selected = i;
                }
            }
        }
        if (mask == 0x3FFF) {
            return 0;
        }
        return 4;
    }
    return 6;
}
