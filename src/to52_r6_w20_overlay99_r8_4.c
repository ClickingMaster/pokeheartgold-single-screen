#include "to52_r6_w20_overlay99_r8_4_private.h"

BOOL PokeathlonMedals_Main(OverlayManager *manager, int *state) {
    R8WorkW20 *work = OverlayManager_GetData(manager);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, work->heap);
        *state = 1;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            *state = 2;
        }
        break;
    case 2:
        if ((work->flags << 2) >> 31) {
            ov99_021E7794(work);
            work->flags &= 0xDFFFFFFF;
        }
        if ((work->flags << 3) >> 31) {
            if (!ov99_021E7598(work)) {
                break;
            }
            work->flags &= 0xEFFFFFFF;
            break;
        }
        if (!ov99_021E7BC8(work) && !ov98_0221EF24()) {
            break;
        }
        PlaySE(0x5DC);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, work->heap);
        *state = 3;
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            *state = 4;
        }
        break;
    case 4:
        return TRUE;
    default:
        if (&_021EA5D8 != NULL) {
            GF_AssertFail();
        }
        break;
    }
    ov98_0221E6CC(work->unk404);
    return FALSE;
}

void ov99_021E7A54(R8WorkW20 *work) {
    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(work->bg);
    vu8 *base = (vu8 *)0x027E0000;
    u32 offset = 0x3FF8;
    u32 value = *(vu32 *)(base + offset);
    *(vu32 *)(base + offset) = value | 1;
}

void ov99_021E7A78(R8WorkW20 *work) {
    work->grid = GridInputHandler_Create(ov99_021EA03C, ov99_021EA250, &ov99_021E9FA0, work, TRUE, (u8)((work->flags << 5) >> 24), work->heap);
}
