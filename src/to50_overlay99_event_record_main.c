#include "to50_overlay99_event_record_main_private.h"

BOOL PokeathlonEventRecord_Main(void *manager, int *state)
{
    Ov99EventMainWork *work = OverlayManager_GetData(manager);

    switch (*state) {
    case 0:
        BeginNormalPaletteFade(0, 1, 1, 0, 6, 1, work->heapId);
        *state = 1;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            *state = 2;
        }
        break;
    case 2:
        if (work->toggleRequested != 0) {
            work->screen ^= 1;
            ov99_021E8828(work);
        }
        if (work->refreshRequested != 0) {
            ov99_021E8788(work);
            ov99_021E8C50(work);
            work->refreshRequested = 0;
        }
        if (ov98_0221EF24() || ov99_021E90F4(work)) {
            PlaySE(0x5dc);
            BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, work->heapId);
            *state = 3;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            *state = 4;
        }
        break;
    case 4:
        return TRUE;
    default:
        if (_021EA5E4 != NULL) {
            GF_AssertFail();
        }
        break;
    }
    ov98_0221E6CC(work->spriteContext);
    return FALSE;
}
