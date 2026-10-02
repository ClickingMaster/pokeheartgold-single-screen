#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *owner,*data;u8 a,b,c,d;u8 pad[4];u8 kinds[2][6],values[6],unused,flag;u16 ids[4];u8 attrs[4],flags[4],choice,p; s16 number;u16 half;u8 x,y;} State;
typedef struct {void (*callback)(void);u8 pad[0x194-4];u8 a,b;} Context;
typedef struct {u8 a,flag,values[6],kinds[2][6];u16 ids[4];u8 attrs[4],flags[4];s16 number;u16 half;u8 x,y;} Input;
extern void *Heap_Alloc(int,u32),*SysTask_CreateOnMainQueue(void (*)(void),void *,int);
void *ov12_0225A524(void *,Context *,Input *);
void *ov12_0225A524(void *owner,Context *ctx,Input *input){State *s=Heap_Alloc(5,60);int i,j;MIi_CpuClearFast(0,(u32*)s,60);s->c=0;s->d=0;s->owner=owner;s->a=input->a;s->b=ctx->a;s->choice=ctx->b;s->data=(u8*)ctx+40;s->flag=input->flag;s->number=input->number;s->half=input->half;s->x=input->x;s->y=input->y;for(i=0;i<2;i++)for(j=0;j<6;j++)s->kinds[i][j]=input->kinds[i][j];for(i=0;i<6;i++){if(input->kinds[0][i]==2)s->values[i]=0;else s->values[i]=input->values[i];}{int k;for(k=0;k<4;k++){s->ids[k]=input->ids[k];s->attrs[k]=input->attrs[k];s->flags[k]=input->flags[k];}}return SysTask_CreateOnMainQueue(ctx->callback,s,0);}
