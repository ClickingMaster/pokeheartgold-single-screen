#pragma force_active on
#include "global.h"
typedef struct {
    u8 pad0[0x4c];
    void *bgConfig;
    u8 pad50[0x2b0 - 0x50];
    void *plttData;
} Ov83Work;
void PaletteData_PushTransparentBuffers(void *plttData);
void DoScheduledBgGpuUpdates(void *bgConfig);
void GF_RunVramTransferTasks(void);
void OamManager_ApplyAndResetBuffers(void);
void ov83_02244488(Ov83Work *work);
// @fn ov83_02244488
void ov83_02244488(Ov83Work *work) {
    vu32 *base = (vu32 *)0x027E0000;

    if (work->plttData != NULL) {
        PaletteData_PushTransparentBuffers(work->plttData);
    }
    DoScheduledBgGpuUpdates(work->bgConfig);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    base[0x3FF8 / sizeof(u32)] |= 1;
}
