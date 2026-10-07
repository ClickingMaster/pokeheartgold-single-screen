#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct{u16 toDs:1;u16 pad1:7;u16 fromDs:1;u16 pad2:3;u16 power:1;u16 pad3:3;}W40750Flags;
typedef struct HB HB;struct HB{HB*prev,*next;u16 owner,magic;};
typedef struct{HB*first,*last;volatile u16 count;u16 id;}HM;
typedef struct {HB header;u16 type,words,pad10,station,pad14,length,rxFlags;u8 pad1a[6];u16 total,rssi;union {u16 word;W40750Flags bits;} flags;u16 pad26,addr1[3],addr2[3],addr3[3],sequence;} W40750Frame;
typedef struct {u8 prefix[0x20];u16 count1;u8 gap22[10];u16 count2;u8 gap2e[0x48-0x2e];HM heap;} W40750Queue;
typedef struct {u8 prefix[8];u16 state,padA,mode,power;u8 gap10[0x88-0x10];u16 station,pad8a,pad8c,awake;} W40750State;
typedef struct {u8 prefix[0x17c];W40750Queue queue;u8 gap[0x344-0x17c-sizeof(W40750Queue)];W40750State state;u8 gap2[0x560-0x344-sizeof(W40750State)];u32 total,unicast,multicast;u8 gap3[12];u32 duplicate;}W40750Root;
#define ROOT (*(W40750Root**)0x0380fff4)
extern int ReleaseHeapBuf(HM*,HB*);
extern u32 CAM_Search(u16*);
extern u16 CAM_GetStaState(u32);
extern int IsExistManFrame(const void*,u16);
extern u8* MakeDisAssFrame(u8*,u32);
extern u8* MakeDeAuthFrame(const void*,u16,u32);
extern void TxManCtrlFrame(void*);
extern void CAM_SetPowerMgtMode(u32,u16);
extern u16 CAM_GetLastSeqCtrl(u32);
extern void WSetMacAdrs1(u16*,const u16*);
extern int WSetPowerState(u32);
extern void CAM_SetRSSI(u32,u32);
extern void CAM_SetLastSeqCtrl(u32,u16);
extern void CAM_UpdateLifeTime(u32);
extern void SendMessageToWmDirect(void*,void*);
extern void AddTask(int,u32);
void RxDataFrameTask(void);
#pragma opt_common_subs off
// @fn RxDataFrameTask
void RxDataFrameTask(void){
W40750Root*base=ROOT;W40750Frame*frame;W40750State*state;W40750Queue*q=&base->queue;frame=(W40750Frame*)q->heap.first;state=&base->state;u32 index;u32 drop;u32 mode;
if((u32)frame==0xffffffffu)return;
if(state->state!=0x40){ReleaseHeapBuf(&q->heap,&frame->header);return;}
u16 dest;
switch(frame->flags.bits.fromDs) {
default:
 dest=frame->addr3[0];
 if(dest&1)base->multicast++;else base->unicast++;
 break;
case 0:
 dest=frame->addr1[0];
 if(dest&1)base->multicast++;else base->unicast++;
 break;
}
drop=1;
base->total+=(frame->rxFlags&0xf0)/16-1;
mode=state->mode;switch(mode){
case 1:
if(frame->flags.word&1)break;
index=CAM_Search(frame->addr2);
if(index==255||CAM_GetStaState(index)!=0x40){void*reply;
if(CAM_GetStaState(index)==0x30){if(IsExistManFrame(frame->addr2,0xa0))break;reply=MakeDisAssFrame((u8*)frame->addr2,7);}
else{if(IsExistManFrame(frame->addr2,0xc0))break;reply=MakeDeAuthFrame(frame->addr2,7,1);}
if(reply)TxManCtrlFrame(reply);break;
}
CAM_SetPowerMgtMode((u16)index,frame->flags.bits.power);
if(frame->sequence==CAM_GetLastSeqCtrl(index)){base->duplicate++;break;}
WSetMacAdrs1(frame->addr1,frame->addr3);drop=0;break;
case 2:case 3:{u16 flags=frame->flags.word;
if(flags&1)break;
if(state->power&&!(flags&0x2000)){
if(frame->addr1[0]&1)state->awake&=~1;
else if(mode!=3)state->awake&=~2;
if(!state->awake&&!q->count1&&!q->count2)WSetPowerState(1);
}
index=state->station;
if(frame->sequence==CAM_GetLastSeqCtrl(index)){base->duplicate++;break;}
WSetMacAdrs1(frame->addr2,frame->addr3);drop=0;break;}
}
if(!drop){u16 cam=(u16)index;
frame->station=cam;CAM_SetRSSI(cam,frame->rssi&255);CAM_SetLastSeqCtrl(index,frame->sequence);CAM_UpdateLifeTime(index);
frame->length=frame->total-24;frame->type=0x180;frame->words=(frame->length+45u)>>1;SendMessageToWmDirect(&q->heap,frame);
}else ReleaseHeapBuf(&q->heap,&frame->header);
if(q->heap.count)AddTask(2,6);
}
