#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int PXI_SendWordByFifo(u32,u32,int);extern void VENEER_SVC_WaitByLoop(u32),OS_Terminate(void);
void CTRDGi_CallbackForSetPhi(u32 tag,u32 data,int error);
// @fn CTRDGi_CallbackForSetPhi
void CTRDGi_CallbackForSetPhi(u32 tag,u32 data,int error){if((data&63)==3){{u32 reg=*(volatile u16*)0x4000204;data=(data&0x1ffffc0)>>6;reg&=~96;*(volatile u16*)0x4000204=reg|(data<<5);}while(PXI_SendWordByFifo(17,18,0))VENEER_SVC_WaitByLoop(1);}else OS_Terminate();}
