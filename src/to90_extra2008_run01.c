#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x690];void *sprites[16];u8 pad2[0x11fa];s8 selected;u8 flags;u8 pad3[0x30];void **objects;int count;} State;
extern int ov18_021E8ACC(State *,int,int),ov18_021E8AB0(State *,int);
extern void ManagedSprite_SetPaletteOverride(void *,int),ov18_021F11C0(State *,int,int),ov18_021F4620(State *);
extern void *ov18_021E8B18(void *);
extern void ov18_021F47C0(void *,u32 *),ov18_021F48AC(State *,u32,u32,int,int);
void ov18_021F463C(State *);
void ov18_021F463C(State *s){
 u32 all=0,disabled=0;u16 i;
 if(s->selected==0){
  for(i=1;i<s->count;i++){
   if(ov18_021E8ACC(s,i,((u32)s->flags<<25)>>31)){
    ManagedSprite_SetPaletteOverride(s->sprites[i],(((u32)s->flags<<25)>>31)+4);ov18_021F11C0(s,i+8,1);
   }else {
    ov18_021F11C0(s,i+8,0);
    if(ov18_021E8AB0(s,i))ov18_021F47C0(ov18_021E8B18(s->objects[i]),&disabled);
   }
   if(ov18_021E8AB0(s,i))ov18_021F47C0(ov18_021E8B18(s->objects[i]),&all);
  }
  ov18_021F48AC(s,all,disabled,i+8,(((u32)s->flags<<25)>>31)+4);
 }else{
  ov18_021F4620(s);
  if(ov18_021E8ACC(s,s->selected,((u32)s->flags<<25)>>31)){
   ManagedSprite_SetPaletteOverride(s->sprites[1],(((u32)s->flags<<25)>>31)+4);ov18_021F11C0(s,9,1);
  }else{
   ov18_021F11C0(s,9,0);
   if(ov18_021E8AB0(s,s->selected))ov18_021F47C0(ov18_021E8B18(s->objects[s->selected]),&disabled);
  }
  if(ov18_021E8AB0(s,s->selected))ov18_021F47C0(ov18_021E8B18(s->objects[s->selected]),&all);
  ov18_021F48AC(s,all,disabled,10,(((u32)s->flags<<25)>>31)+4);
 }
}
