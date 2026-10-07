#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u8 sCommandMesgQueue[],sCommandMesgBuffer[];
extern void *SNDi_SharedWork;
extern void OS_InitMessageQueue(void *,void *,int),PxiFifoCallback(int,u32,int),PXI_SetFifoRecvCallback(int,void (*)(int,u32,int));
void SND_CommandInit(void);
// @fn SND_CommandInit
void SND_CommandInit(void){OS_InitMessageQueue(sCommandMesgQueue,sCommandMesgBuffer,8);PXI_SetFifoRecvCallback(7,PxiFifoCallback);SNDi_SharedWork=0;}
