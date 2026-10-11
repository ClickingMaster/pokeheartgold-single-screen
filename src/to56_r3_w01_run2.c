#include "to56_r3_w01_private.h"
void ov08_022217C8(u8 *work) {
    BattleDpadCursor_SetActive(*(void **)(work + 0x2088), 0);
    BattleDpadCursor_ResetPosition(*(void **)(work + 0x2088));
    BattleCursor_Disable(BattleDpadCursor_GetBattleCursor(*(void **)(work + 0x2088)));
}
