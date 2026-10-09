#pragma force_active on
#include "global.h"
#include "system.h"
#include "heap.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"
typedef struct {
    enum HeapID heapId;
    BgConfig *bgConfig;
    void *x8;
    int timer;
} Work;
void ov96_021EEECC(void);
void ov96_021EEEEC(void *arg);
void *PokeathlonCourse_AllocPtr4FromHeap(void *course, u32 size);
void ov96_021EEF0C(BgConfig *bgConfig, enum HeapID heapId);
void *ov96_021EE740(enum HeapID heapId);
int PokeathlonCourse_GetMode(void *course);
void *PokeathlonCourse_GetSaveData(void *course);
void ov96_021EE75C(void *a, BgConfig *bgConfig, int b, int mode, void *saveData);
void ov96_021EEFAC(void *course);
#pragma opt_propagation off
// @fn ov96_021EEFAC
void ov96_021EEFAC(void *course) {
    Work *work;
    enum HeapID heap = (enum HeapID)159;
    Heap_Create((enum HeapID)92, heap, 0x40000);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= ~0x1F00;
    reg_GXS_DB_DISPCNT &= ~0x1F00;
    ov96_021EEECC();
    work = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(Work));
    MI_CpuFill8(work, 0, sizeof(Work));
    work->heapId = (enum HeapID)159;
    work->bgConfig = BgConfig_Alloc((enum HeapID)159);
    ov96_021EEF0C(work->bgConfig, work->heapId);
    work->x8 = ov96_021EE740(work->heapId);
    ov96_021EE75C(work->x8, work->bgConfig, 0, PokeathlonCourse_GetMode(course), PokeathlonCourse_GetSaveData(course));
    Main_SetVBlankIntrCB(ov96_021EEEEC, work);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
}
