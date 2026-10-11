#include "to56_w21_private.h"

u8 BattlePartyMenu_IsSelectedMonEgg(u8 *work) {
    u8 index = (*(u8 **)work)[0x11];
    if (((Ov08Entry *)work)[index].disabled != 0) {
        return 1;
    }
    return 0;
}
