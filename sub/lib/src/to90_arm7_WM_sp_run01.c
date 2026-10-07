#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_common_subs off
extern u8 wmspW[],wmspIndicateThread[],wmspRequestThread[];
#define W ((u8 *)(wmspW+0x1000))
#define P (*(u8 **)(W+0x550))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
extern BOOL __VENEER_OS_SendMessage(void *,void *,int),__VENEER_OS_ReceiveMessage(void *,void *,int);
extern u16 *__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void *),__VENEER_SND_BeginSleep(void),__VENEER_OS_Terminate(void);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern BOOL __VENEER_OS_DisableScheduler(void),__VENEER_OS_EnableScheduler(void),__VENEER_OS_SetThreadPriority(void *,u32);
extern void *WL_GetThreadStruct(void);
extern u32 WMSP_GetLinkLevel(u32);
extern void __VENEER_OS_InitMessageQueue(void *,void *,int),__VENEER_OS_InitMutex(void *),__VENEER_OS_CreateThread(void *,void (*)(void *),void *,void *,u32,u32),__VENEER_OS_WakeupThreadDirect(void *),__VENEER_OS_InitVAlarm(void),__VENEER_PXI_Init(void),__VENEER_PXI_SetFifoRecvCallback(int,void (*)(int,u16 *,BOOL));
extern BOOL __VENEER_OS_IsVAlarmAvailable(void);
extern void WMSP_IndicateThread(void *),WMSP_RequestThread(void *),WmspPxiCallback(int,u16 *,BOOL),WL_InitDriver(void *);
void WMSP_Init(u8 *,u32 *);
// @fn WMSP_Init
void WMSP_Init(u8 *driver,u32 *config){int i;U(W,0x540)=config[0];U(W,0x544)=U(driver,0x24);U(W,0x548)=U(driver,0x28);U(W,0x54c)=0;U(W,0x550)=0;__VENEER_OS_InitMessageQueue(wmspW,wmspW+0x20,2);__VENEER_OS_InitMessageQueue(wmspW+0x28,wmspW+0x48,4);__VENEER_OS_InitMessageQueue(wmspW+0x58,wmspW+0x78,4);__VENEER_OS_InitMessageQueue(wmspW+0x88,wmspW+0xa8,32);*(void **)(driver+0x10)=wmspW;*(void **)(driver+0x14)=wmspW+0x28;U(W,0x578)=config[2];U(W,0x57c)=config[6];U(W,0x580)=config[4];U(W,0x584)=config[1];U(W,0x588)=config[5];U(W,0x58c)=config[3];__VENEER_OS_InitMutex(wmspW+0x1528);__VENEER_OS_CreateThread(wmspIndicateThread,WMSP_IndicateThread,0,wmspW+0x1528,0x400,config[1]);__VENEER_OS_WakeupThreadDirect(wmspIndicateThread);__VENEER_OS_CreateThread(wmspRequestThread,WMSP_RequestThread,0,wmspW+0x1128,0x1000,config[3]);__VENEER_OS_WakeupThreadDirect(wmspRequestThread);for(i=0;i<32;i++)wmspW[0x1554+i]=0;U(W,0x574)=0;if(!__VENEER_OS_IsVAlarmAvailable())__VENEER_OS_InitVAlarm();__VENEER_PXI_Init();__VENEER_PXI_SetFifoRecvCallback(10,WmspPxiCallback);U(driver,0x18)=2;U(driver,0xc)=config[5];WL_InitDriver(driver);}
