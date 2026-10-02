#pragma force_active on
#include "global.h"
#pragma force_active on
void *Heap_Alloc(int,u32);
void *SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,int);
void ov12_0223C1C4(void *,void *);
int BattleSystem_GetMaxBattlers(void *),BattleSystem_GetBattleType(void *);
void *SaveArray_Party_Alloc(int);
void ov12_0225A674(void *,u8 *,u8 *);
void ov12_0225A700(void *,u8 *,u8 *);
void ov12_0225A7AC(void *,u8 *,u8 *);
#define W(p,o) (*(int *)((p)+(o)))
#define P(p,o) (*(void **)((p)+(o)))
#define H(p,o) (*(u16 *)((p)+(o)))
void ov12_0225A7AC(void *bs,u8 *ctx,u8 *data){
 u8 *w=Heap_Alloc(5,52);
 int i,j;
 u8 *q,*p;
 w[10]=0;P(w,0)=bs;q=data;p=w;w[8]=data[0];w[9]=data[1];w[11]=data[2];W(w,16)=W(data,32);w[22]=data[3];H(w,20)=0;w[24]=data[36];
 for(i=0;i<4;i++,q+=6,p+=6){w[12+i]=data[4+i];for(j=0;j<6;j++)p[28+j]=q[8+j];}
 SysTask_CreateOnMainQueue(*(void (**)(void *,void *))(ctx+16),w,0);
}
