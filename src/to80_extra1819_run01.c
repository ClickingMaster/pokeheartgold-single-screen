#pragma force_active on
#include "global.h"
static inline void setIrq(u32*base){base[0xFFE]|=1;}
void DoScheduledBgGpuUpdates(void*);void GF_RunVramTransferTasks(void);void SpriteSystem_TransferOam(void);void ov112_021E79A8(u32*);
void ov112_021E79A8(u32*p){DoScheduledBgGpuUpdates((void*)p[6]);GF_RunVramTransferTasks();SpriteSystem_TransferOam();setIrq((u32*)0x027E0000);}
