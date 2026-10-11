#include "to53_overlay112_w01_private.h"


void ov112_021E77E4(void *state) {
    ov112_021EA1F8(state);
    ov112_021E9C10(state, 1);
    ov112_021E9C10(state, 0);
    ov112_021E7AC8(*(void **)((u8 *)state + 0x18));
    GF_TouchpadStopSampling();
    GF_DestroyVramTransferManager();
    ov112_021E9E30(state);
    ov112_021E7C6C(state);
    FontID_Release(4);
    Main_SetVBlankIntrCB(NULL, NULL);
    *(void **)((u8 *)state + 0x1e52c) = NULL;
}
