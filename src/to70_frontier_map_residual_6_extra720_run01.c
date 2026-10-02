#pragma force_active on
#include "global.h"
typedef struct {u16 kind,id,number;s16 x,y;u8 a,b;} Entry;
typedef struct {u8 pad[8];void *objects;u8 pad2[8];void *manager;u8 pad3[8];void *graphics;} W;
typedef struct {u32 active;u8 pad[0x38];} Slot;
extern Slot *sub_02096868(void *);
extern void *ov42_022280B8(void *,u16 *),*ov42_0222903C(void *,void *,int,int);
extern void ov42_02229200(void *,u8),ov80_02239914(void *,int,void *,void *,Entry *);
void *ov80_02239510(W *,Entry *,int);
void *ov80_02239510(W *w,Entry *e,int index){Slot *s=sub_02096868(w->objects);u16 args[6];void *obj,*sprite;if(index==-1){for(index=0;index<32;index++,s++)if(!s->active)break;if(index==32)GF_AssertFail();}args[0]=e->x;args[1]=e->y;args[2]=e->number;args[3]=e->id;args[4]=e->a;args[5]=e->kind;obj=ov42_022280B8(w->manager,args);sprite=ov42_0222903C(w->graphics,obj,0,101);ov42_02229200(sprite,e->b);ov80_02239914(w->objects,index,obj,sprite,e);return obj;}
