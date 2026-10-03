#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int heap;u8 pad4[0xc4];void*palette;u8 padcc[0xb4];} Owner;
typedef struct {u8 pad0[0x10];u32 mode;u8 pad14[0x30];u16 flags;u16 pad46;Owner*owner;} Work;
typedef struct {int heap;void *palette;u8 indices[128];u16 durations[128];u8 count,member,timer,state,index;} Payload;
typedef struct {u8 pad0[0x18];int done;u8 pad1c[8];Payload *data;} Callback;
extern void *Heap_Alloc(int,u32);extern void Heap_Free(void*);extern void ov07_0221DD38(void*,Callback*,int);extern u8 *GfGfxLoader_LoadFromNarc(int,int,int,int,int);extern void *SysTask_CreateOnMainQueue(void(*)(void*,Callback*),Callback*,int);extern void ov07_0221E914(void*,Callback*);extern const u8 ov07_02237870[];
int ov07_0221E9D4(Work*);
int ov07_0221E9D4(Work*w){Owner*o=w->owner;Callback*c=Heap_Alloc(o->heap,40);int value;u8*buf,*p;u16*q;u16 i;MI_CpuFill8(c,0,40);c->data=Heap_Alloc(o->heap,400);MI_CpuFill8(c->data,0,400);w->flags|=4;c->done=0;c->data->heap=o->heap;c->data->palette=o->palette;ov07_0221DD38((u8*)o+0x180,c,3);switch(w->mode){case 0:case 1:case 3:case 5:value=0;break;case 4:value=2;break;case 19:value=4;break;case 53:value=6;break;case 52:value=8;break;case 22:value=10;break;case 58:value=12;break;case 28:value=14;break;case 29:value=16;break;case 57:value=26;break;case 7:value=28;break;case 14:value=20;break;case 17:value=18;break;case 2:value=22;break;case 11:value=24;break;case 44:value=30;break;default:GF_ASSERT((u32)ov07_02237870==0);value=0;break;}buf=GfGfxLoader_LoadFromNarc(9,value,0,o->heap,1);GF_ASSERT(buf!=0);c->data->member=value+1;i=0;p=buf;{u8 byte=*p;while(byte!=255){c->data->indices[i]=byte;p++;i++;byte=*p;}}c->data->count=i;{struct {u16 word,j;} run;run.word=*(u16*)(buf+0x80);run.j=0;q=(u16*)(buf+0x80);while(run.word!=0xff98){c->data->durations[run.j]=run.word;q++;run.j++;run.word=*q;}}Heap_Free(buf);SysTask_CreateOnMainQueue(ov07_0221E914,c,0x1001);return 0;}
