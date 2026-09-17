#include "to50_overlay15_vblank_tasks_private.h"
void ov15_021F995C(void **work) {
    DoScheduledBgGpuUpdates(*work);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
    vu32 *base = (vu32 *)0x027E0000;
    base[0x3FF8 / sizeof(u32)] |= 1;
}
