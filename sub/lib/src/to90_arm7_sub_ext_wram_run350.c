#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define ROOT (*(u8 *volatile *)0x0380FFF4)
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define P(p,n) (*(u8 *volatile*)((u8*)(p)+(n)))
extern void CAM_DecFrameCount(void*),CAM_IncFrameCount(void*),IssueMaDataConfirm(void*,void*),TxqPri(u32),CAM_SetDoze(u32),WSetPowerState(u32),MoveHeapBuf(void*,void*,void*),TxqEndManCtrl(void*,u32),CAM_ClrTIMElementBitmap(u32);extern u32 CAM_GetPowerMgtMode(u32),CAM_GetFrameCount(u32);extern u32 __VENEER_OS_DisableIrqMask(u32);extern void __VENEER_OS_EnableIrqMask(u32),WClearKSID(u32),WlIntrMpEndTask(void),ClearQueuedPri(u32),MessageDeleteTx(u32,u32),ReleaseHeapBuf(void*,void*),SetFatalErr(u32),InitManHeader(void*,void*);extern void *AllocateHeapBuf(void*,u32),*GetHeapBufNextAdrs(void*);extern u32 CAM_Search(void*),CAM_GetStaState(u32);extern void CAM_SetStaState(u16,u32),ClearTxKeyData(void),DeleteTxFrames(u32);
extern const u16 Pri2QBit[];extern void SetManCtrlFrame(void*);extern u32 WCalcManRate(void),SetSSIDElement(void*),SetSupRateSet(void*),CAM_AllocateAID(u32),WL_ReadByte(void*);extern void WSetMacAdrs1(void*,void*),DMA_Write(void*,void*,u32),WL_WriteByte(void*,u32);extern void*CAM_GetMacAdrs(u32);
void UpdateGameInfoElement(void);
// @fn UpdateGameInfoElement
void UpdateGameInfoElement(void){u8*root=ROOT,*state=root+0x344;u8*beacon=P(root,0x4ac);u32 length=H(state,0xa0);u8*out=beacon+0x24+H(state,0x96);if(length){u16 odd=H(state,0xa2);u8*source=P(state,0x9c),*destination=out+10;if(odd&1){DMA_Write(destination-1,source,length+2);WL_WriteByte(out+9,(*(u16*)0x0380FFF0>>8)&255);}else DMA_Write(destination,source,length+1);}H(P(ROOT,0x4ac),10)=(u16)(H(state,0x96)+0x26+H(state,0xa0));WL_WriteByte(out+1,(H(state,0xa0)+8)&255);if(H(ROOT,0x690)&4){u8*end=(u8*)((u32)(out+13+H(state,0xa0))&~3u);H(end,0)=0xb6b8;H(end,2)=0x1d46;}H(state,0xa4)=0;}
