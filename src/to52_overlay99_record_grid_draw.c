#include "global.h"

#include "sprite_system.h"

#define PTR_AT(base, offset) (*(void **)((u8 *)(base) + (offset)))

void ov99_021E8FEC(void *work, int draw);

void ov99_021E8FEC(void *work, int draw) {
    int row = 0;
    int base = 0;

    do {
        int col = 0;
        u8 *cursor;

        cursor = (u8 *)work + base * sizeof(u32);
        do {
            ManagedSprite_SetDrawFlag(PTR_AT(cursor, 0x70), draw);
            col++;
            cursor += sizeof(u32);
        } while (col < 3);
        row++;
        base += 3;
    } while (row < 5);
}
