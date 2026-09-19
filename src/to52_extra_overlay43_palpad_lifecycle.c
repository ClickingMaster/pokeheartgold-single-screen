#include "to52_extra_overlay43_palpad_lifecycle_internal.h"

BOOL PalPad_Init(OverlayManager *man, int *state) {
    u8 *work;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_42), OVY_LOAD_ASYNC);
    Heap_Create(HEAP_ID_3, HEAP_ID_51, 7 << 16);
    work = OverlayManager_CreateAndGetData(man, 0x5a4, HEAP_ID_51);
    memset(work, 0, 0x5a4);
    *(void **)(work + 0x27c) = OverlayManager_GetArgs(man);
    ov43_0222A290(work + 0x214, *(void **)(work + 0x27c), HEAP_ID_51);
    ov43_0222A48C(work + 4, work + 0x214, HEAP_ID_51);
    ov43_0222AE5C(work + 0x280, work + 0x214, work + 4, HEAP_ID_51);
    ov43_0222B5A8(work + 0x2c0, work + 0x214, work + 4, HEAP_ID_51);
    ov43_0222CBAC(work + 0x3c0, work + 0x214, work + 4, HEAP_ID_51);
    ov43_0222CD90(work + 0x3cc, work + 0x214, work + 4, HEAP_ID_51);
    ov43_0222D134(work + 0x408, work + 0x214, work + 4, HEAP_ID_51);
    Main_SetVBlankIntrCB(ov43_0222A148, work);
    HBlankInterruptDisable();
    return TRUE;
}

BOOL PalPad_Main(OverlayManager *man, int *state) {
    u8 *work = OverlayManager_GetData(man);
    int result;

    switch (*state) {
    case 0:
        switch (U32_AT(work, 0)) {
        case 0:
            result = ov43_0222AE64(work + 0x280, work + 0x214, work + 4, HEAP_ID_51);
            break;
        case 1:
            result = ov43_0222B5D0(work + 0x2c0, work + 0x214, work + 4, HEAP_ID_51);
            break;
        case 2:
            result = ov43_0222CBB4(work + 0x3c0, work, work + 0x214, work + 4, HEAP_ID_51);
            break;
        case 3:
            result = ov43_0222CD98(work + 0x3cc, work + 0x214, work + 4);
            break;
        case 4:
            result = ov43_0222D15C(work + 0x408, work + 0x214, work + 4, HEAP_ID_51);
            break;
        case 5:
            return TRUE;
        }
        if (result == 1) {
            U32_AT(work, 0) = U8_AT(work, 0x21d);
            U8_AT(work, 0x21c) = U8_AT(work, 0x21e);
            U32_AT(work, 0x5a0) = 4;
            *state = 1;
        }
        break;
    case 1:
        S32_AT(work, 0x5a0)
        --;
        if (S32_AT(work, 0x5a0) <= 0) {
            *state = 0;
        }
        break;
    }
    ov43_0222A500(work + 4);
    return FALSE;
}

BOOL PalPad_Exit(OverlayManager *man, int *state) {
    u8 *work;
    OverlayManager *saved = man;

    work = OverlayManager_GetData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    ov43_0222B098(work + 0x280, work + 0x214, work + 4);
    ov43_0222B93C(work + 0x2c0, work + 0x214, work + 4);
    ov43_0222CD34(work + 0x3c0, work + 0x214, work + 4);
    ov43_0222CE0C(work + 0x3cc, work + 0x214, work + 4);
    ov43_0222D228(work + 0x408, work + 0x214, work + 4);
    ov43_0222A520(work + 4);
    ov43_0222A2F0(work + 0x214, *(void **)(work + 0x27c));
    OverlayManager_FreeData(saved);
    Heap_Destroy(HEAP_ID_51);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_42));
    return TRUE;
}
