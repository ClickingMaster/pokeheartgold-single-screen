#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define ROOT (*(u8 **)0x0380FFF4)
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define V(p,n) (*(volatile u16*)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
extern u32 AdjustRingPointer(u32);
typedef struct HB HB;struct HB{HB*prev,*next;u16 owner,magic;};typedef struct{HB*first,*last;u16 count;u16 id;}HM;
extern int MoveHeapBuf(HM*,HM*,HB*),ReleaseHeapBuf(HM*,HB*);
extern void WUpdateCounter(void),SetFatalErr(u32),AddTask(u32,u32),RxBeaconFrame(void*),RxKeyDataFrame(void*);struct W40746Frame;extern struct W40746Frame*TakeoutRxFrame(void*,u32);extern u32 RxMpFrame(void*),RxMpAckFrame(void*);
typedef struct{u16 pad:10;u16 more:1;u16 rest:5;}W40746Flags;
typedef struct{u16 value:4;u16 rest:12;}W40746Nibble;
typedef struct W40746Frame {u16 pad[4],status,pad1[5],flags,pad2[10],sequence;} W40746Frame;
void WlIntrRxEndTask(void);

#pragma force_active off
static inline void QueueRxPacket(HM*from,HM*to,HB*packet,u32 priority,u32 task){MoveHeapBuf(from,to,packet);AddTask(priority,task);}
#pragma force_active on
// @fn WlIntrRxEndTask
void WlIntrRxEndTask(void){u8*root=ROOT,*state=root+0x4dc;for(;;){/*D*/u32 idx;u32 next;u32 rel;u16*raw;u32 status;W40746Frame *frame;/*E*/idx=V((void*)0x0480805A,0);if(idx==H(state,4))return;if(idx>=0x8c6)WUpdateCounter();raw=((u16*)0x04804000)+idx;next=*(u16*)AdjustRingPointer((u32)(raw+1));if(*raw==0xffff){V((void*)0x0480805A,0)=next;continue;}frame=TakeoutRxFrame(raw,*(u16*)AdjustRingPointer((u32)(raw+4)));V((void*)0x0480805A,0)=next;if(!frame){if((*raw&15)==12)SetFatalErr(16);else SetFatalErr(8);continue;}if((H(ROOT,0x690)&8)&&(frame->flags&0x4000))H(state,2)=0;rel=1;status=frame->status;if(status&0x200){if(((W40746Flags*)((u8 *)frame+0x14))->more==1||((W40746Nibble*)((u8 *)frame+0x2a))->value!=0){rel=0;QueueRxPacket((HM*)(root+0x188),(HM*)(root+0x1e8),(HB*)((u8*)frame-16),2,9);}}else{switch(status&15){case 8:if((frame->flags&15)==8){rel=0;QueueRxPacket((HM*)(root+0x188),(HM*)(root+0x1c4),(HB*)((u8*)frame-16),2,6);}break;case 1:if(frame->flags==0x80)RxBeaconFrame(frame);break;case 0:if(!(frame->flags&15)){rel=0;QueueRxPacket((HM*)(root+0x188),(HM*)(root+0x1dc),(HB*)((u8*)frame-16),1,7);}break;case 5:if((frame->flags&0xe7ff)==0xa4){rel=0;QueueRxPacket((HM*)(root+0x188),(HM*)(root+0x1dc),(HB*)((u8*)frame-16),1,7);}break;case 14:case 15:if((frame->flags&0xe7bf)==0x118)RxKeyDataFrame(frame);break;case 12:if((frame->flags&0xe7ff)==0x228){if(H(ROOT,0x354)==0)V((void*)0x0480803C,0)=1;W(ROOT,0x5ac)++;rel=RxMpFrame(frame);}break;case 13:if((frame->flags&0xe7ff)==0x218){W(ROOT,0x5b0)++;rel=RxMpAckFrame(frame);}break;}}if(rel)ReleaseHeapBuf((HM*)(root+0x188),(HB*)((u8*)frame-16));if(H(ROOT,0x690)&1){u32 i;for(i=0;i<7;i++){if((u32)raw>=0x04805f60)raw=(u16*)((u8*)raw-H(ROOT,0x3de));*raw=0xffff;raw++;}}}}
