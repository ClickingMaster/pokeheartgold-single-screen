#include "to57_new22_overlay08_b20_private.h"
void BattlePartyMenu_StoreLargeButtonTilemaps(u8 *work, const u16 *src) {
    BattlePartyMenu_CopyTilemapRect((u16 *)(work + 0xC4C), src, 0, 0, 0x1E, 0x11);
    BattlePartyMenu_CopyTilemapRect((u16 *)(work + 0x1048), src, 0, 0x11, 0x1E, 0x11);
    BattlePartyMenu_CopyTilemapRect((u16 *)(work + 0x1444), src, 0, 0x22, 0x1E, 0x11);
    BattlePartyMenu_CopyTilemapRect((u16 *)(work + 0x1840), src, 0, 0x33, 0x10, 6);
    BattlePartyMenu_CopyTilemapRect((u16 *)(work + 0x1900), src, 0x10, 0x33, 0x10, 6);
    BattlePartyMenu_CopyTilemapRect((u16 *)(work + 0x19C0), src, 0, 0x39, 0x10, 6);
    BattlePartyMenu_CopyTilemapRect((u16 *)(work + 0x1A80), src, 0x10, 0x39, 0x10, 6);
}
void BattlePartyMenu_CopyTilemapRect(u16 *dst, const u16 *src, int x, int y, u8 width, u8 height) {
    u16 row;
    u16 col;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            dst[row * width + col] = src[(y + row) * 32 + x + col];
        }
    }
}
