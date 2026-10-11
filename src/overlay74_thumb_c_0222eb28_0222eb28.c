#include "overlay74_thumb_w07_private.h"

void ov74_0222EB28(void *data, int *state, int value) {
    MysteryGiftComm_Free();
    WirelessStatusIcon_Destroy();
    U32AT(data, 0x2c40) = value;
    *state = 0x17;
}
