#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define PTR(p,o) ((void*)W(p,o))
void ov85_021E7D50(void*,void*);
void ov85_021E7D78(void*p);
void ov85_021E7D78(void*p){u8*q=(u8*)p+400;int n=W(p,48),i=0;u8*s=q+320;do{if(W(q,320))ov85_021E7D50(p,s);i++;q+=176;s+=176;}while(i<n);}
