#pragma force_active on
#include "global.h"
typedef struct {int state;u8 p[0x10];int value;u8 q[2];u16 count;u8 r[0x74];void *obj[2];u8 s[0x52];volatile u8 flag;} W;
extern void ov96_021EB52C(void*,int,int);extern void ov96_021F4790(W*);extern void GfGfx_EngineBTogglePlanes(int,int);
static inline void updateControl(volatile u8*p) {*(volatile u32*)0x4001018=((u32)*p<<16)&0x1ff0000;}
void ov96_021F4724(W*);
void ov96_021F4724(W*w) {int i;W*p;w->state=0;w->count=0;p=w;for(i=0;i<2;i++,p=(W*)((u8*)p+4))ov96_021EB52C(p->obj[0],1,0);*(volatile u16*)0x4001050=0;ov96_021F4790(w);GfGfx_EngineBTogglePlanes(8,0);GfGfx_EngineBTogglePlanes(1,0);w->value=0;w->flag=0;updateControl(&w->flag);}
