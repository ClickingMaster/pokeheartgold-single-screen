#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int PXI_SendWordByFifo(int,u32,int);
void AlarmHandler(void *arg);
// @fn AlarmHandler
void AlarmHandler(void *arg){while(PXI_SendWordByFifo(7,(u32)arg,0)<0){}}
