#include "to58_overlay91_small_private.h"
void ov91_0225CC4C(u8 *base, u8 *arg) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    if (*(void **)base != NULL) {
        ov90_02258938(*(void **)base);
        *(void **)base = NULL;
    }
    if (*(void **)(base + 0x34) != NULL) {
        if (*(u16 *)(base + 0x44) == 0) {
            ov91_0225CCEC(*(void **)(base + 0x30));
        }
        ov91_0225CE80(*(void **)(base + 0x34));
        GF_DestroyVramTransferManager();
    }
    if (arg[0x38] != 0) {
        ov00_021E6A4C();
    }
    if (*(void **)(base + 4) != NULL) {
        ov90_02258A04(*(void **)(base + 4));
        *(void **)(base + 4) = NULL;
    }
}
