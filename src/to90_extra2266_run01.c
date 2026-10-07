#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int character,palette,member;} Resource;
typedef struct{u8 pad[0xb0];Resource*resources[4];} Assets;
typedef struct{void*bg;u8 a,b;u16 pad;void*pic,*task;} Work;
typedef struct{int heap;u8 pad[0x18-4];u32*script;u8 pad2[0xc0-0x1c];Assets*assets;void*bg,*palette;u8 pad3[0x178-0xcc];Work*work;u8 pad4[0x1b0-0x17c];void*narc;} State;
extern int ov07_0221D3CC(State*,u32),Pokepic_GetAttr(void*,int),ov07_0221FAE8(void*);
extern void*ov07_0221FA48(State*,int),*BgGetCharPtr(int),*Heap_Alloc(int,u32);
extern void MI_CpuFill8(void*,u8,u32),ToggleBgLayer(int,int),BG_LoadCharTilesData(void*,int,int,int,int),PaletteData_LoadNarc(void*,int,int,int,int,int,int),GfGfxLoader_LoadScrnDataFromOpenNarc(void*,int,void*,int,int,int,int,int),BgSetPosTextAndCommit(void*,int,int,int),SetBgPriority(int,u8),ov07_0221D4FC(void*,void*);
extern void*SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);
void ov07_0221D5B0(State*);
void ov07_0221D5B0(State*s){u32 arg,flag;int index,character,palette,member;s16 x,y;s->script++;arg=*s->script;s->script++;flag=*s->script;s->script++;index=ov07_0221D3CC(s,arg);character=s->assets->resources[index]->character;palette=s->assets->resources[index]->palette;member=s->assets->resources[index]->member;MI_CpuFill8(BgGetCharPtr(2),0,0x1900);ToggleBgLayer(2,0);BG_LoadCharTilesData(s->bg,2,character,0xc80,0);PaletteData_LoadNarc(s->palette,palette,member,s->heap,0,0,0x80);GfGfxLoader_LoadScrnDataFromOpenNarc(s->narc,268,s->bg,2,0,0,0,s->heap);if(flag==1){s->work=Heap_Alloc(s->heap,16);s->work->bg=s->bg;s->work->pic=ov07_0221FA48(s,index);s->work->a=0;s->work->b=0;s->work->task=SysTask_CreateOnMainQueue(ov07_0221D4FC,s->work,0x1001);}x=Pokepic_GetAttr(ov07_0221FA48(s,index),0);y=Pokepic_GetAttr(ov07_0221FA48(s,index),1);y-=Pokepic_GetAttr(ov07_0221FA48(s,index),41);BgSetPosTextAndCommit(s->bg,2,0,-(x-40));BgSetPosTextAndCommit(s->bg,2,3,-(y-40));ToggleBgLayer(2,1);SetBgPriority(2,ov07_0221FAE8(s));}
