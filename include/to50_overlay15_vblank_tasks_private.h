#ifndef TO50_OV15_R2_PRIVATE_H
#define TO50_OV15_R2_PRIVATE_H
#include "global.h"
extern void DoScheduledBgGpuUpdates(void *);
extern void GF_RunVramTransferTasks(void);
extern void SpriteSystem_TransferOam(void);
void BagApp_VBlankCallback(void **);
#endif
