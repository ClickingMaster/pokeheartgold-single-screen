#include "global.h"

#include "bg_window.h"
#include "sprite_system.h"

void ov08_0221DB7C(u8 *work);

void ov08_0221DB7C(u8 *work) {
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)(work + 0x2020), FALSE);
    ClearWindowTilemapAndScheduleTransfer((Window *)(*(u8 **)(work + 0x2070) + 0xa0));
    ClearWindowTilemapAndScheduleTransfer((Window *)(*(u8 **)(work + 0x2070) + 0x60));
    ClearWindowTilemapAndScheduleTransfer((Window *)(*(u8 **)(work + 0x2070) + 0x70));
}
