#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#define ROOT (*(u8**)0x0380fff4)
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define V(o) (*(volatile u16*)(o))
extern void __VENEER_MIi_CpuCopy16(const void*,void*,u32),MIi_CpuCopy16(const void*,void*,u32);
extern u32 __VENEER_OS_DisableIrqMask(u32),__VENEER_OS_EnableIrqMask(u32);
extern void WaitMacStop(void),CopyTxFrmToMacBuf(void*,void*),RestoreTxFrame(void*),MakeBeaconFrame(void),MakePsPollFrame(u16),AddTask(u32,u32);
extern int WCheckTxBufIdBeforeFrame(void*);extern void*AllocateHeapBuf(void*,u32);extern void SendMessageToWmDirect(void*,void*);
extern void WStop(void),WShutdown(void),IssueMlmeConfirm(void),ReleaseIntr(void),ReleaseHeapBuf(void*,void*),__VENEER_OS_ExitThread(void);
extern u32 DeleteTask(u32);extern const u16 crc16_table[],test_pattern[];
extern const struct W35501Reg {u16 offset,mask;} test_reg[];
extern void WlIntrStartTx(void),WlIntrStartRx(void),WlIntrPreTbtt(void),WlIntrTbtt(void),WlIntrActEnd(void),WlIntrRfWakeup(void),WlIntrTxErr(void),WlIntrRxCntup(void),WlIntrRxEnd(void),WlIntrCntOvf(void),WlIntrTxEnd(void),WlIntrMpEnd(int);
void TerminateWlTask(void);
// @fn TerminateWlTask
void TerminateWlTask(void){H(ROOT,0x33e)|=0x8000;if(H(ROOT,0x34c)){WStop();if(H(ROOT,0x404)){H(ROOT,0x404)=0;u8*msg=*(u8**)(ROOT+0x420);H(msg,4)=6;IssueMlmeConfirm();}WShutdown();}while(DeleteTask(3)!=0xffff){}AddTask(3,23);}
