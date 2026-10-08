#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct{u8 flags:4,aid:4;u8 bytes[21];}ReceiverChild;
typedef struct{u8 prefix[0x14];u32 id;u8 capacity;} ReceiverFile;
typedef struct{ReceiverFile*file;u32 reserved4;u16 pending,next,active,participants,recipients;u8 available;u8 trailing[0x5d4-0x13];} ReceiverSlot;
typedef struct{u8 prefix[0x1340];ReceiverChild children[15];u16 channels[15];u32 ids[15];u8 gap14e4[4];u32 states[15];u8 gap1524[2];u8 entries[15],count;u16 mask;u8 gap1538[0x1754-0x1538];u16 pending[15];u8 gap1772[0x1d40-0x1772];ReceiverSlot files[16];} ReceiverParent;
typedef union{struct{u8 command,reserved;u16 block;u8 rest[16];}transfer;struct{u8 command,reserved,piece;u8 bytes[17];}fragment;}ReceiverReceive;
typedef struct{u32 id;ReceiverChild child;u16 channel;u8 entry;} ReceiverRequest;
typedef struct{u8 prefix[0x1d40];ReceiverSlot selected;}ReceiverSlotView;
typedef char ReceiverSlotSize[(sizeof(ReceiverSlot)==0x5d4)?1:-1];
typedef char RequestLayout[(offsetof(ReceiverRequest,channel)==26 && offsetof(ReceiverRequest,entry)==28 && sizeof(ReceiverReceive)==20)?1:-1];
typedef char ReceiverFileOffset[(offsetof(ReceiverParent,files)==0x1d40)?1:-1];
extern ReceiverParent *mbc;extern u32 _02247480;
extern u8*MBi_SetRecvBufferFromChild(u8*,ReceiverReceive*,int);
extern void MBi_CommChangeParentState(u32,u32,void*);extern u32 MBi_calc_nextsendblock(u32,u32);
void MBi_CommParentRecvDataPerChild(void*,u16);
#pragma push
#pragma opt_rotateloops off
#pragma opt_common_subs off
#pragma opt_loop_invariants off
// @fn MBi_CommParentRecvDataPerChild
void MBi_CommParentRecvDataPerChild(void*incoming,u16 child){
 struct{ReceiverRequest request;ReceiverReceive receive;}buffers;
 ReceiverParent *initial;u32 state;ReceiverRequest *payload;u8 entry;u8 count;
 if(!child || child>15)return;
 payload=(ReceiverRequest*)MBi_SetRecvBufferFromChild((u8*)incoming+10,&buffers.receive,child);
 initial=mbc;state=initial->states[child-1];
 switch(buffers.receive.transfer.command){
 case 7:
  if(state==2){ReceiverChild *record;
   if(!payload)return;MI_CpuCopy8(payload,&buffers.request,29);
   mbc->ids[child-1]=buffers.request.id;
   mbc->channels[child-1]=buffers.request.channel;
   MI_CpuCopy8(&buffers.request.child,&mbc->children[child-1],22);
   record=&mbc->children[child-1];record->aid=child;
   MBi_CommChangeParentState(child,10,&buffers.request.child);
  }
  if(state!=10)return;entry=payload->entry;
  count=0;if(entry>=16 || !mbc->files[entry].available || mbc->ids[child-1]!=mbc->files[entry].file->id){mbc->pending[child-1]=4;}
  else{u8 i;
   for(i=0;i<16;i++)if(mbc->files[entry].participants&(1u<<i))count++;
   if(count>=mbc->files[entry].file->capacity){mbc->pending[child-1]=0;MBi_CommChangeParentState(child,11,0);return;}
  }
  {ReceiverParent *parent=mbc;switch(parent->pending[child-1]){
   case 3:
    if(parent->mask&(1u<<child))return;
    parent->count++;
    {ReceiverParent *maskParent=mbc;maskParent->mask|=1u<<child;}mbc->entries[child-1]=entry;
    mbc->files[entry].participants|=1u<<child;
    mbc->files[entry].recipients|=1u<<child;
    mbc->pending[child-1]=0;MBi_CommChangeParentState(child,5,0);return;
   case 4:parent->pending[child-1]=0;MBi_CommChangeParentState(child,4,0);return;
   default:return;
  }}
 case 8:
  if(state==5){MBi_CommChangeParentState(child,14,0);return;}
  if(state!=14)return;if(initial->pending[child-1]!=2)return;
  entry=initial->entries[child-1];initial->files[entry].active|=1u<<child;
  mbc->files[entry].pending=0;
  mbc->pending[child-1]=0;MBi_CommChangeParentState(child,6,0);return;
 case 9:
  if(state!=6)return;entry=initial->entries[child-1];if(entry==255)return;
  {u32 next=MBi_calc_nextsendblock(initial->files[entry].next,buffers.receive.transfer.block);
   mbc->files[entry].next=next;_02247480|=1u<<entry;}return;
 case 10:
  if(state==6){entry=initial->entries[child-1];if(entry==255)return;
   initial->files[entry].active&=~(1u<<child);MBi_CommChangeParentState(child,7,0);return;}
  if(state!=7)return;if(initial->pending[child-1]!=5)return;
  initial->pending[child-1]=0;MBi_CommChangeParentState(child,8,0);break;
 case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 11:default:break;
 }
}
#pragma pop
