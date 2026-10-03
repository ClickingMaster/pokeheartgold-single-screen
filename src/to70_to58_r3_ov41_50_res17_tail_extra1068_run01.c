#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x23c];s32 x,y,z,w;u16 rotation,pad;s32 scale[3];u8 p26c[0xffff];} Work;
typedef struct{u8 p[0x60];s32 a,b;} Arg;
extern void ov41_0224B5D8(Work *,s32,s32);
void ov41_0224B888(Work *,Arg *);
void ov41_0224B888(Work *w,Arg *arg){w->x=72;w->y=56;w->z=56;w->w=64;w->scale[0]=4096;w->scale[1]=4096;w->scale[2]=4096;w->rotation=0;ov41_0224B5D8(w,arg->a,arg->b);*(u16*)((u8*)w+0x25c)=32767;}
