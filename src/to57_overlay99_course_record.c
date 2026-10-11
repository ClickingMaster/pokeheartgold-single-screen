#include "to57_overlay99_course_record_private.h"

BOOL PokeathlonCourseRecord_Init(OverlayManager *manager, int *state) {
    CourseRecordWork *work;
    void *args;
    SpriteCounts6 counts;

    HandleLoadOverlay((u32)&SDK_OVERLAY_OVY_98_ID, 2);
    args = OverlayManager_GetArgs(manager);
    if (args == NULL) {
        GF_AssertFail();
    }
    Heap_Create((enum HeapID)3, (enum HeapID)0x84, 0x30000);
    work = OverlayManager_CreateAndGetData(manager, sizeof(CourseRecordWork), (enum HeapID)0x84);
    MI_CpuFill8(work, 0, sizeof(CourseRecordWork));
    work->heapId = 0x84;
    work->updatePending = 0;
    work->applyPending = 1;
    work->unk84 = 1;
    ov99_021E5B54(work, args);
    PokeathlonUtil_ResetDisplay();
    ov99_021E5C88(work);
    ov99_021E5D58(work);
    work->textContext = PokeathlonUtil_CreateTextContext(work->heapId, work->bgConfig, 0x11, ov99_021E96D8, 0x13a);
    counts = ov99_021E95A4;
    work->spriteContext = PokeathlonUtil_CreateSpriteContext(work->heapId, &counts, 0x1a);
    ov99_021E6274(work);
    ov99_021E6438(work);
    ov99_021E64E0(work, 0);
    ov99_021E5FE8(work, 0);
    ov99_021E6050(work, 0);
    ov99_021E6530(work, 0);
    ov99_021E6018(work);
    SetKeyRepeatTimers(2, 4);
    ResetVisibleHardwareWindows(0);
    ResetVisibleHardwareWindows(1);
    Main_SetVBlankIntrCB((void (*)(void *))ov99_021E6250, work);
    return TRUE;
}

BOOL PokeathlonCourseRecord_Exit(OverlayManager *manager, int *state) {
    CourseRecordWork *work = OverlayManager_GetData(manager);
    SetKeyRepeatTimers(4, 8);
    ov99_021E5B74(work->alloc);
    OverlayManager_FreeData(manager);
    Heap_Destroy((enum HeapID)0x84);
    UnloadOverlayByID((u32)&SDK_OVERLAY_OVY_98_ID);
    return TRUE;
}

BOOL PokeathlonCourseRecord_Main(OverlayManager *manager, int *state) {
    CourseRecordWork *work = OverlayManager_GetData(manager);
    ov99_021E5F74(work);
    if (work->applyPending == 1) {
        ov99_021E5FE8(work, work->requestedPage);
        ov99_021E6050(work, work->requestedPage);
        ov99_021E6530(work, work->requestedPage);
        work->applyPending = 0;
        work->updatePending = 1;
    }
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
        if (ov99_021E6638(work) && work->updatePending == 0) {
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
        PokeathlonUtil_FreeTextContext(work->textContext, 0x11);
        ov99_021E5D2C(work);
        PokeathlonUtil_FreeSpriteContext(work->spriteContext, work->sprites, 0x1a);
        PokeathlonUtil_DisableDisplay();
        return TRUE;
    default:
        if (_021EA5C0 != NULL) {
            GF_AssertFail();
        }
        break;
    }
    PokeathlonUtil_DrawSprites(work->spriteContext);
    return FALSE;
}
