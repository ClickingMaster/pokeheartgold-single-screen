#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x190];u8 base[0x140];u32 active;u8 gap[8];u32 bit;} Entry;
typedef struct{u8 p[0x20];u32 mask;u8 p24[12];int count;u8 p34[0x15c];} Work;
extern void ov85_021E8428(Work *,void *);
void ov85_021E8454(Work *);
void ov85_021E8454(Work *w){u32 mask=w->mask;int count=w->count,i=0;u8 *cursor=(u8*)w+0x190,*data;if(count>0){data=cursor+0x140;do {if(*(u32*)(cursor+0x140)&&((1u<<*(u32*)(cursor+0x14c))&mask))ov85_021E8428(w,data);i++;cursor+=0xb0;data+=0xb0;}while(i<count);}}
