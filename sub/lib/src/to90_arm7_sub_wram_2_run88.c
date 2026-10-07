#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct{u16 id,pad;u32 data[4];u32 initialized;} W;extern W lock_id;extern u8 pulse_edge_alarm[];extern void OS_InitTick(void),OS_InitAlarm(void),OS_CreateAlarm(void*),CTRDGi_InitCommon(void),PXI_Init(void),CTRDGi_InitModuleInfo(void);extern int OS_GetLockID(void);typedef void(*CB)(u32,u32,int);extern void PXI_SetFifoRecvCallback(u32,CB);extern void CTRDGi_CallbackForInitModuleInfo(u32,u32,int),CTRDGi_CallbackForPulledOut(u32,u32,int),CTRDGi_CallbackForCTREx(u32,u32,int),CTRDGi_CallbackForSetPhi(u32,u32,int);
void CTRDG_Init(void);
// @fn CTRDG_Init
void CTRDG_Init(void){int id;OS_InitTick();OS_InitAlarm();OS_CreateAlarm(pulse_edge_alarm);if(lock_id.initialized)return;lock_id.initialized=1;CTRDGi_InitCommon();id=OS_GetLockID();if(id==-3)return;lock_id.id=id;PXI_Init();PXI_SetFifoRecvCallback(13,CTRDGi_CallbackForInitModuleInfo);CTRDGi_InitModuleInfo();PXI_SetFifoRecvCallback(13,CTRDGi_CallbackForPulledOut);PXI_SetFifoRecvCallback(16,CTRDGi_CallbackForCTREx);PXI_SetFifoRecvCallback(17,CTRDGi_CallbackForSetPhi);}
