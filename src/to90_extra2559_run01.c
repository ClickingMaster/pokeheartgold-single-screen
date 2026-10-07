#pragma force_active on
#include "global.h"
#include <stddef.h>
#pragma force_active on
typedef struct {void *a,*b,*c;s32 attr,x,y,z;u8 gap1c[0x43-0x1c];u8 finished;u16 tag,shape;} Slot;
typedef struct {u8 prefix[0x14];u16 count;s16 ticks;u8 delay,kind,reserved,mode;u8 tail[4];} Entry;
typedef struct {u8 prefix[0xb8];Slot slots[12];Entry entries[12];u8 gap598[0x5d0-0x598];u8 flag;u8 gap5d1[0x5e8-0x5d1];u16 timer;u8 tail[0x5f4-0x5ea];} Parent;
typedef struct {u16 groups[4],sizes[12];u32 packed;} Output;
typedef struct {u16 counts[4];u8 flags[4];} Accumulator;
void ov96_02202738(Parent *,Output *);
void ov96_02202738(Parent *s,Output *out){
 u8 flags[4]={0,0,0,0};u16 counts[4]={0,0,0,0};int i;u32 packed=0;u8 finished,mode;int shift;int w,h,group;u8 *slotBytes,*entryBytes,*outputBytes;
 for(i=0,shift=0,slotBytes=(u8 *)s,entryBytes=(u8 *)s,outputBytes=(u8 *)out;i<12;shift+=2,slotBytes+=sizeof(Slot),outputBytes+=sizeof(u16),entryBytes+=sizeof(Entry),i++){

  packed|=(u32)(*(s32 *)(slotBytes+offsetof(Parent,slots[0].attr)))<<shift;mode=(*(u8 *)(entryBytes+offsetof(Parent,entries[0].mode)));finished=(*(u8 *)(slotBytes+offsetof(Parent,slots[0].finished)))!=0?1:0;(*(u8 *)(slotBytes+offsetof(Parent,slots[0].finished)))=0;
  w=(*(s32 *)(slotBytes+offsetof(Parent,slots[0].x)))/4096;h=(*(s32 *)(slotBytes+offsetof(Parent,slots[0].z)))/4096;
  if(w<1)w=1;else if(w>64)w=64;
  if(h<1)h=1;else if(h>64)h=64;
  group=i/3;flags[group]+=(*(u16 *)(slotBytes+offsetof(Parent,slots[0].shape)))<<((i%3)*2);
  {int value=(w-1)+((h-1)<<6)+(mode<<12)+(finished<<13);value+=(*(u8 *)(entryBytes+offsetof(Parent,entries[0].kind)))<<14;*(u16 *)(outputBytes+offsetof(Output,sizes[0]))=value;}
  counts[group]+=(*(u16 *)(entryBytes+offsetof(Parent,entries[0].count)));if(counts[group]>999)counts[group]=999;
 }
 for(i=0;i<4;i++)out->groups[i]=(flags[i]<<10)+counts[i];
 out->packed=packed;out->packed=packed+((u32)s->flag<<24);
 out->packed+=((u32)((s->timer+30)/30))<<25;
}
