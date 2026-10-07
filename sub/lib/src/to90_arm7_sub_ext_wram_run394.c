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
void TxqEndBroadCast(void*arg,u32 next);
typedef struct{u16 pad0:2;u16 type:2;u16 pad1:3;u16 bit7:1;u16 bit8:1;u16 pad2:4;u16 bit13:1;u16 bit14:1;u16 pad3:1;} W35677Flags;
// @fn TxqEndBroadCast
void TxqEndBroadCast(void*arg,u32 next){u8*frame=(u8*)arg;u8*root=ROOT,*heap=root+0x17c;W(root,0x550)++;if(((volatile W35677Flags*)(frame+0x14))->type==0){CAM_IncFrameCount(frame);MoveHeapBuf(heap+0x30,heap+0x24,frame-16);TxqEndManCtrl(frame,0);}else IssueMaDataConfirm(heap+0x30,frame-16);H(ROOT,0x454)=0;if(!(((volatile W35677Flags*)(P(ROOT,0x45c)+12))->bit13)){*(volatile u16*)0x048080AC=8;*(volatile u16*)0x048080AE=5;if(next){if(H(heap,0x2c))TxqPri(1);if(H(heap,0x20))TxqPri(0);}}if(H(heap,0x38)){if(next)TxqPri(2);}else CAM_ClrTIMElementBitmap(0);}
