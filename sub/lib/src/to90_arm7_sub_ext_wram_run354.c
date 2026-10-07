#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define ROOT (*(u8 *volatile *)0x0380FFF4)
#define H(p,n) (*(volatile u16*)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define P(p,n) (*(u8**)((u8*)(p)+(n)))
extern void CAM_DecFrameCount(void*),CAM_IncFrameCount(void*),IssueMaDataConfirm(void*,void*),TxqPri(u32),CAM_SetDoze(u32),WSetPowerState(u32),MoveHeapBuf(void*,void*,void*),TxqEndManCtrl(void*,u32),CAM_ClrTIMElementBitmap(u32);extern u32 CAM_GetPowerMgtMode(u32),CAM_GetFrameCount(u32);extern u32 __VENEER_OS_DisableIrqMask(u32);extern void __VENEER_OS_EnableIrqMask(u32),WClearKSID(u32),WlIntrMpEndTask(void),ClearQueuedPri(u32),MessageDeleteTx(u32,u32),ReleaseHeapBuf(void*,void*),SetFatalErr(u32),InitManHeader(void*,void*);extern void *AllocateHeapBuf(void*,u32),*GetHeapBufNextAdrs(void*);extern u32 CAM_Search(void*),CAM_GetStaState(u32);extern void CAM_SetStaState(u16,u32),ClearTxKeyData(void),DeleteTxFrames(u32);
extern const u16 Pri2QBit[];extern void SetManCtrlFrame(void*);extern u32 WCalcManRate(void),SetSSIDElement(void*),SetSupRateSet(void*),CAM_AllocateAID(u32),WL_ReadByte(void*);extern void WSetMacAdrs1(void*,void*),DMA_Write(void*,void*,u32),WL_WriteByte(void*,u32);extern void*CAM_GetMacAdrs(u32);
u8*MakeReAssReqFrame(u8*address);
// @fn MakeReAssReqFrame
u8*MakeReAssReqFrame(u8*address){u8*root=ROOT,*state=root+0x344,*buf=AllocateHeapBuf(root+0x188,0x64);u32 length;if(!buf){SetFatalErr(2);return buf;}H(buf,12)=0xffff;InitManHeader(buf+16,address);H(buf,0x3c)=H(state,0x7c);H(buf,0x3e)=H(state,0x70);WSetMacAdrs1(buf+0x40,state+0x82);length=SetSSIDElement(buf+0x46);length+=SetSupRateSet(buf+0x46+length);H(buf,0x16)=(u16)(length+10);H(buf,0x22)=(u16)(H(buf,0x16)+28);H(buf,0x24)=0x20;return buf+16;}
