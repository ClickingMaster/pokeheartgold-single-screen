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
void ov12_0225A674(void *bs,u8 *ctx,u8 *data){
 u8 *w=Heap_Alloc(5,52);
 int unused[2],n,i;
 u8 *p;
 w[15]=0;P(w,0)=bs;w[12]=ctx[0x94];w[13]=ctx[0x194];w[14]=ctx[0x195];H(w,48)=H(data,2);P(w,4)=ctx+40;w[50]=data[1];
 ov12_0223C1C4(bs,unused);n=BattleSystem_GetMaxBattlers(bs);BattleSystem_GetBattleType(bs);
 i=0;if(n>0){p=w;do{H(p,16)=H(data,4);H(p,18)=H(data,6);H(p,20)=H(data,8);H(p,22)=H(data,10);i++;data+=8;p+=8;}while(i<n);}
 SysTask_CreateOnMainQueue(*(void (**)(void *,void *))(ctx+8),w,0);
}
