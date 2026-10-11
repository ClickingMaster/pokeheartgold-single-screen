#include "global.h"

void *BattlePartyMenu_GetButtonTilemap(u8 *work, int type, int row, int alternate);
void *BattlePartyMenu_GetButtonTilemap(u8 *work, int type, int row, int alternate) {
    switch (type) {
    case 0: case 1: case 2: case 3: case 4: case 5:
        if (alternate == 0) return work + 0x1EC + row * 0xC0;
        return work + 0x4EC + row * 0xC0;
    case 6: return work + 0xB84 + row * 0x32;
    case 7: return work + 0xC4C + row * 0x3FC;
    case 8: case 9: case 10: case 11: return work + 0x7EC + row * 0x82;
    case 12: return work + 0x9F4 + row * 0x32;
    case 13: return work + 0xABC + row * 0x32;
    case 14: case 15: case 16: case 17: case 19: case 20: case 21: case 22:
    case 23: case 24: case 25: case 26: case 27:
        return work + 0x1840 + row * 0xC0;
    case 28: case 29: return work + 0x1B40 + row * 0x104;
    case 18: return work + 0x1E4C + row * 0x48;
    case 30: case 31: case 32: case 33: return work + 0x1F24 + row * 0x14;
    default: return 0;
    }
}
