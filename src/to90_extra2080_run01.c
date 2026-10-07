#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *owner,*data,*sprite;int active;} Entry;
typedef struct {u8 pad[0x18];u32 *script;u8 pad2[0x30];Entry entries[14];u8 pad3[12];void *data;void *sprites[8];} Work;
extern void ManagedSprite_SetDrawFlag(void *,int),ManagedSprite_SetDrawPriority(void *,int);
extern int ov07_0221FAB0(Work *),ov07_0221C468(Work *),ov07_0221C470(Work *),ov07_02231924(Work *,int),ov07_0221D3CC(Work *,int),Pokepic_GetAttr(void *,int);
extern void *ov07_0221FA48(Work *,int);
extern void ov07_0221DAD0(void *,void *);
extern void *SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,u32);
void ov07_0221DAF4(Work *);
void ov07_0221DAF4(Work *w) {
Entry *entry;int a,type,index,slot,b,hidden;void *pic;
w->script++;type=*w->script;w->script++;index=*w->script;w->script++;slot=*w->script;w->script++;
entry=w->entries;entry[index].owner=w;w->entries[index].data=w->data;w->entries[index].sprite=w->sprites[slot];w->entries[index].active=1;
ManagedSprite_SetDrawFlag(w->entries[index].sprite,0);
if(ov07_0221FAB0(w)==1){
a=ov07_02231924(w,ov07_0221C468(w));b=ov07_02231924(w,ov07_0221C470(w));
pic=ov07_0221FA48(w,ov07_0221D3CC(w,type));hidden=pic?Pokepic_GetAttr(pic,6):0;
if(hidden==1)ManagedSprite_SetDrawFlag(w->entries[index].sprite,0);else ManagedSprite_SetDrawFlag(w->entries[index].sprite,1);
switch(type){
case 0:if(a==3||a==4)ManagedSprite_SetDrawPriority(w->entries[index].sprite,1);else {ManagedSprite_SetDrawFlag(w->entries[index].sprite,0);ManagedSprite_SetDrawPriority(w->entries[index].sprite,255);}break;
case 2:if(a==5||a==2)ManagedSprite_SetDrawPriority(w->entries[index].sprite,1);else {ManagedSprite_SetDrawFlag(w->entries[index].sprite,0);ManagedSprite_SetDrawPriority(w->entries[index].sprite,255);}break;
case 1:switch(b){case 2:ManagedSprite_SetDrawPriority(w->entries[index].sprite,255);break;case 3:ManagedSprite_SetDrawPriority(w->entries[index].sprite,1);break;case 4:ManagedSprite_SetDrawPriority(w->entries[index].sprite,1);break;case 5:ManagedSprite_SetDrawPriority(w->entries[index].sprite,255);break;}break;
case 3:switch(b){case 2:ManagedSprite_SetDrawPriority(w->entries[index].sprite,1);break;case 3:ManagedSprite_SetDrawPriority(w->entries[index].sprite,255);break;case 4:ManagedSprite_SetDrawPriority(w->entries[index].sprite,255);break;case 5:ManagedSprite_SetDrawPriority(w->entries[index].sprite,1);break;}break;
}
SysTask_CreateOnMainQueue(ov07_0221DAD0,&entry[index],4096);
}
}
