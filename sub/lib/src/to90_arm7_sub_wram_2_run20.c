#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 owned,irq;} Lock;extern void OS_UnLockCartridge(u32),OS_RestoreInterrupts(u32);
void CTRDGi_UnlockByProcessor(u32 id,Lock*l);
// @fn CTRDGi_UnlockByProcessor
void CTRDGi_UnlockByProcessor(u32 id,Lock*l){if(!l->owned)OS_UnLockCartridge(id);OS_RestoreInterrupts(l->irq);}
