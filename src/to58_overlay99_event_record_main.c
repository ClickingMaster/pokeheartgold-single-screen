#include "global.h"

typedef struct Ov99EventWork {
    u8 pad00[0x0C];
    u32 heapId;
    u32 unk10;
    void *spriteContext;
} Ov99EventWork;

extern char _021EA5CC[];
Ov99EventWork *OverlayManager_GetData(void *);
void BeginNormalPaletteFade(u32, u32, u32, u32, u32, u32, u32);
BOOL IsPaletteFadeFinished(void);
BOOL PokeathlonUtil_CheckExitInput(void);
void PlaySE(u32);
void GF_AssertFail(void);
void PokeathlonUtil_DrawSprites(void *);
BOOL ov99_021E6888(void *manager, int *state);

BOOL ov99_021E6888(void *manager, int *state) {
    Ov99EventWork *work = OverlayManager_GetData(manager);
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
        if (PokeathlonUtil_CheckExitInput()) {
            PlaySE(0x5DC);
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
        if (_021EA5CC != NULL) {
            GF_AssertFail();
        }
        break;
    }
    PokeathlonUtil_DrawSprites(work->spriteContext);
    return FALSE;
}
