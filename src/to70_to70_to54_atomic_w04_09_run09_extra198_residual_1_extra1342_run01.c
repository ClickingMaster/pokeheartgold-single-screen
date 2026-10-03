#pragma force_active on
#include "global.h"
typedef struct{void *party;u8 rest[52];} Nested;
typedef struct{u32 a;Nested *nested;u8 p[4];u8 values[4];} Mid;
typedef struct{void *ctx;u32 b;Mid *mid;u8 a,b1,c,d;u32 e;u8 f[4],array[4][6],tail[4];} Work;
extern void *Heap_Alloc(int,int),*SaveArray_Party_Alloc(int),*SysTask_CreateOnMainQueue(void *,void *,int);
typedef struct{u8 a,f[3],values[4],array[4][6],tail[4];} Input;
void *ov12_0225A700(void *,u8 *,Input *);
void *ov12_0225A700(void *ctx,u8 *config,Input *input){Work *w=Heap_Alloc(5,52);int i,j;w->mid=Heap_Alloc(5,52);w->mid->nested=Heap_Alloc(5,56);w->mid->nested->party=SaveArray_Party_Alloc(5);w->c=0;w->ctx=ctx;w->a=input->a;w->b1=config[0x194];w->d=config[0x195];w->f[0]=input->f[0];w->f[1]=input->f[1];w->f[2]=input->f[2];for(i=0;i<4;i++){w->mid->values[i]=input->values[i];for(j=0;j<6;j++)w->array[i][j]=input->array[i][j];w->tail[i]=input->tail[i];}return SysTask_CreateOnMainQueue(*(void **)(config+12),w,0);}
