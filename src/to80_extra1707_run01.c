#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
typedef struct Vec {int x,y,z;} Vec;
typedef union Block {int v[9];u8 bytes[36];} Block;
typedef struct State {u8 pad[0x138];int value;u8 pad2[4];u8 selected,count;u8 pad3[0x2a];Vec pos;} State;
extern void *PokeathlonCourse_GetDataCopyArea(void *);
extern State *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern int System_GetTouchNew(void);
extern int System_GetTouchHeld(void);
extern int ov96_021E5F24(void *);
extern void ov96_021E8228(void *,u8,int,int,int);
extern u32 ov96_021F5F68(State *,int,int);
extern void ov96_021F5F34(u8,u8,Vec *,Vec *);
extern void ov96_021F6088(State *,int,Vec *);
extern u8 *ov96_021E8A20(void *ptr);
extern void ov96_021F7740(void *,int);
void ov96_021F5BA0(void *);
void ov96_021F5BA0(void *arg){u8 *data=PokeathlonCourse_GetDataCopyArea(arg);State *s=PokeathlonCourse_GetHeapAllocPtr4(arg);u32 selected;
 if(System_GetTouchNew()){
 ov96_021E8228(arg,(u8)ov96_021E5F24(arg),3,0,1);selected=ov96_021F5F68(s,gSystem.touchX,gSystem.touchY);
 if(selected<3){s->selected=selected;s->pos.x=gSystem.touchX<<12;s->pos.y=(gSystem.touchY<<12)+0xc0000;s->pos.z=0;}
 }else if(System_GetTouchHeld()){
 if(s->selected<3){if(s->count<7)s->count++;else{Vec v={0,0,0};ov96_021F5F34((u8)gSystem.touchX,(u8)gSystem.touchY,&s->pos,&v);ov96_021F6088(s,s->selected,&v);s->selected=3;s->count=0;}}
 }else{
 if(s->selected<3&&s->count<7){Vec v={0,0,0};ov96_021F5F34((u8)gSystem.touchX,(u8)gSystem.touchY,&s->pos,&v);ov96_021F6088(s,s->selected,&v);}s->selected=3;s->count=0;
 }
 if(ov96_021E5F24(arg)==0){Block *first=(Block *)ov96_021E8A20(data+0x28);Block *dest=(Block *)ov96_021E8A20(data+0x50);*dest=*(Block *)ov96_021E8A20(data);ov96_021F7740(arg,s->value);{u8 values[4];int i;i=0;data+=0x50;for(;i<4;i++){values[i]=((Block *)ov96_021E8A20(data))->v[0];data+=0x28;}for(i=0;i<4;i++)first->bytes[i]=values[i];}}
}
