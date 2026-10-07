#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int PXI_SendWordByFifo(u32,u32,int);extern void VENEER_SVC_WaitByLoop(u32);
void CTRDGi_SendtoPxi(u32 data);
// @fn CTRDGi_SendtoPxi
void CTRDGi_SendtoPxi(u32 data){while(PXI_SendWordByFifo(13,data,0))VENEER_SVC_WaitByLoop(1);}
