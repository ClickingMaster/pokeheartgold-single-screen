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
void DeleteTxFrames(u32 station);
typedef struct{u16 busy,count;u32 pad;u8*input,*frame;void(*end)(void*,u32);}W35684Queue;typedef struct{u8 pad[0x42c];W35684Queue queues[3];}W35684Root;
// @fn DeleteTxFrames
void DeleteTxFrames(u32 station){u8*buf=0,*frame=0;u32 deleted=0,i;if(CAM_GetFrameCount(station)){for(i=0;i<3;i++){buf=P(ROOT,0x194+12*i);if(buf!=(void*)~0u){do{u8*next=GetHeapBufNextAdrs(buf);frame=buf+16;if(H(frame,2)==station){if(i==1||frame==((W35684Root*)ROOT)->queues[i].frame){CAM_DecFrameCount(frame);H(frame,2)=0;CAM_IncFrameCount(frame);}else{H(frame,8)=2;CAM_DecFrameCount(frame);IssueMaDataConfirm(ROOT+0x194+12*i,buf);if(!deleted)deleted=1;}}buf=next;}while(buf!=(void*)~0u);}}}}
