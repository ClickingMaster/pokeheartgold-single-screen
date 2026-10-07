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
extern void WStop(void),WShutdown(void),IssueMlmeConfirm(void*),ReleaseIntr(void),ReleaseHeapBuf(void*,void*),__VENEER_OS_ExitThread(void);
extern u32 DeleteTask(u32);extern const u16 crc16_table[],test_pattern[];
extern const struct W35502Reg {u16 offset,mask;} test_reg[];
extern void WlIntrStartTx(void),WlIntrStartRx(void),WlIntrPreTbtt(void),WlIntrTbtt(void),WlIntrActEnd(void),WlIntrRfWakeup(void),WlIntrTxErr(void),WlIntrRxCntup(void),WlIntrRxEnd(void),WlIntrCntOvf(void),WlIntrTxEnd(void),WlIntrMpEnd(int);
void ReleaseWlTask(void);
// @fn ReleaseWlTask
void ReleaseWlTask(void){u8*p=ROOT;ReleaseIntr();ReleaseHeapBuf(p+0x188,(void*)(W(ROOT,0x318)-12));ReleaseHeapBuf(p+0x188,(void*)(W(ROOT,0x3e0)-12));__VENEER_OS_ExitThread();}
