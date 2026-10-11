#include "overlay59_apricorn_lifecycle_private.h"

u32 ov59_0223ABEC(Ov59Work *work) {
    switch (U16_(work, 0x40)) {
    case 0:
        if (ov59_0223AF9C(work)) {
            if (U8_(work, 0x3E) != 0) {
                U16_(work, 0x40) = 0;
                U8_(work, 0x36) = 0;
                return 5;
            }
            U16_(work, 0x40)
            ++;
        }
        break;
    case 1:
        if (ov59_0223B11C(work)) {
            U16_(work, 0x40) = 0;
            return 1;
        }
        break;
    }
    return 4;
}

u32 ov59_0223AC48(Ov59Work *work) {
    switch (U16_(work, 0x40)) {
    case 0:
        while (U8_(work, 0x36) < 5) {
            if (S8_(work + U8_(work, 0x36), 0x39) != 0) {
                break;
            }
            U8_(work, 0x36)
            ++;
        }
        U16_(work, 0x40)
        ++;
        // fallthrough
    case 1:
        if (ov59_0223B164(work)) {
            U16_(work, 0x40) = 0;
            U8_(work, 0x36)
            ++;
            U8_(work, 0x3E)
            --;
            if (U8_(work, 0x3E) == 0) {
                return 1;
            }
        }
        break;
    }
    return 5;
}

void ov59_0223ACD0(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *(u32 *)0x04000000 &= 0xFFFFE0FF;
    *(u32 *)0x04001000 &= 0xFFFFE0FF;
    SetMasterBrightnessByColor(0, 0);
    SetMasterBrightnessByColor(1, 0);
    ResetVisibleHardwareWindows(0);
    ResetVisibleHardwareWindows(1);
}

void ov59_0223AD20(Ov59Work *work) {
    ov59_0223B434(work);
    ov59_0223B590(work);
    ov59_0223B68C(work);
    ov59_0223B740(work);
    ov59_0223B8B0(work);
    PTR_(work, 0x78) = ov59_0223C298(PTR_(work, 0));
    Main_SetVBlankIntrCB(ov59_0223B3DC, work);
}
