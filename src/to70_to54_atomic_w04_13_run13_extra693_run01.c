#pragma force_active on
#include "global.h"
typedef struct {u32 v[22];} Args;
typedef struct {void *sys,*ctx,*pic,*other;Args args;u8 a,b,c,d;u32 e;} Work;
typedef struct {u8 p[0x20];void *pic;u8 q[0x94-0x24];u8 a;u8 r[0x194-0x95];u8 b;u8 s[0x1a0-0x195];u32 flag;} Ctx;
extern void *Heap_Alloc(int,int);extern void *ov12_0223A8DC(void*);extern void ov12_02261D30(u8*,u32*,u32,u32,u16);extern void ov12_0225FD14(void*,void*);extern void *SysTask_CreateOnMainQueue(void(*)(void*,void*),void*,int);
void ov12_0225A914(void*,Ctx*,Args*);
void ov12_0225A914(void *sys,Ctx *ctx,Args *args){Work *w=Heap_Alloc(5,0x70);w->c=0;w->sys=sys;w->ctx=ctx;w->a=ctx->a;w->b=ctx->b;w->other=ov12_0223A8DC(sys);w->args=*args;w->pic=ctx->pic;if(args->v[19]==1&&args->v[20]==25)ctx->flag=1;if(args->v[19]==1&&args->v[20]==26)ctx->flag=0;ov12_02261D30(&w->d,&w->e,args->v[19],args->v[20],((u16*)args)[1]);SysTask_CreateOnMainQueue(ov12_0225FD14,w,0);}
