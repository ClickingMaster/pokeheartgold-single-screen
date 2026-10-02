#pragma force_active on
#include "global.h"
#pragma force_active on
int ov85_021E8570(void *),ov85_021E8150(void *),ov85_021E834C(void *),ov85_021E750C(void *),ov85_021E8144(void *),sub_02096D4C(void *,int,void *,int),ov85_021E5FE0(u8 *);
void ov85_021E8128(void *),ov85_021E8358(void *),ov85_021E7644(void *,int),ov85_021E8558(void *,int);
#define W(o) (*(int *)(w+(o)))
#define P(o) (*(void **)(w+(o)))
int ov85_021E5FE0(u8 *w){int scale;u16 data[4];int *p;
if(ov85_021E8570(w)==1&&!ov85_021E8150(w)){W(0)=29;return 1;}
if(W(52)&&!ov85_021E8150(w))ov85_021E8128(w);
if(!ov85_021E834C(w)&&ov85_021E750C(w)==1){if(!ov85_021E8150(w)){ov85_021E8128(w);W(56)=1;}ov85_021E8358(w);}
if(W(56)){if(sub_02096D4C(*(void **)((u8 *)P(204)+48),10,w+56,1)==1)W(56)=0;}
p=(int *)(w+212);if(W(8)<300){p[18]+=191;if(p[18]>=57344)p[18]=57344;}else{p[18]-=191;if(p[18]<16384)p[18]=16384;}
scale=p[18];if(ov85_021E8144(w)==1)ov85_021E7644(w+212,scale*2);else ov85_021E8558(w,scale);
data[1]=W(8);data[2]=W(284)/4096;data[3]=W(272)/4096;sub_02096D4C(P(208),11,data,8);return 0;}
