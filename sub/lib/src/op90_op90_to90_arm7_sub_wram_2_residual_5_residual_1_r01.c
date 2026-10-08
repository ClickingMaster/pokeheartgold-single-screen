#pragma force_active on
#include <nitro/types.h>
#include <nitro/os/system.h>
#include <nitro/os/interrupt.h>
#include <nitro/exi/ARM7/genPort.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
typedef struct {u32 prefix[8],busy;} W40764_Work;
typedef struct {u16 ime,rcnt;int restore;} W40764_Hardware;
extern u16 PMi_TriggerBL,PMi_KeyPattern[];
extern W40764_Work PMi_Work;
extern void PM_SetLEDPattern(int),PMi_SetLED(int),SND_BeginSleep(void),SND_EndSleep(void),PMi_ResetControl(u32),PMi_SetControl(u32),__VENEER_SVC_Sleep(void),PMi_SetRegister(u16,u32),PMi_SwitchUtilityProc(u32),PMi_SendPxiCommand(u32,u32,u32);
extern u8 PMi_GetRegister(u16);
void PMi_DoSleep(void);
// @fn PMi_DoSleep
void PMi_DoSleep(void){OSIntrMode irq;OSIrqMask mask;u8 control;u16 rcnt;BOOL restore;u16 ime;ime=H(0x04000208);restore=0;H(0x04000208)=0;irq=OS_DisableInterrupts();mask=OS_DisableIrqMask(~0xfe000000u);control=PMi_GetRegister(0);PM_SetLEDPattern(2);PMi_SetLED(2);PMi_SetLED(2);SND_BeginSleep();PMi_ResetControl(1);if(PMi_TriggerBL&1){H(0x04000132)=PMi_KeyPattern[0]|0x4000;OS_EnableIrqMask(0x1000);}if(PMi_TriggerBL&4)OS_EnableIrqMask(0x400000);if(PMi_TriggerBL&2){rcnt=H(0x04000134);restore=1;EXIi_SelectRcnt(EXI_GPIOIF_GPIO);EXIi_SetBitRcnt0L(0x40,0);EXIi_SetBitRcnt0L(0x100,0x100);OS_EnableIrqMask(0x80);}if(PMi_TriggerBL&8)OS_EnableIrqMask(0x100000);if(PMi_TriggerBL&16)OS_EnableIrqMask(0x2000);OS_RestoreInterrupts(irq);{u16 previous=H(0x04000208);(void)previous;H(0x04000208)=1;}__VENEER_SVC_Sleep();PMi_SetRegister(0,control);{int a=(PMi_TriggerBL&32)?6:7;int b=(PMi_TriggerBL&64)?4:5;PMi_SwitchUtilityProc(a);PMi_SwitchUtilityProc(b);}if(restore)H(0x04000134)=rcnt;PMi_SetControl(1);SND_EndSleep();PMi_Work.busy=0;PMi_SendPxiCommand(0x62,0,0);OS_DisableInterrupts();OS_SetIrqMask(mask);OS_RestoreInterrupts(irq);{u16 previous=H(0x04000208);(void)previous;H(0x04000208)=ime;}}
