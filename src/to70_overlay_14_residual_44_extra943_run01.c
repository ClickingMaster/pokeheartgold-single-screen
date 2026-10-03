#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x34];void*ctx;}Work;extern void ov14_021E84A4(void*),ov14_021E8328(void*),ov14_021E884C(void*),ov14_021F40E8(Work*,int),ov14_021F0234(Work*,void*,int);extern u8 ov14_021EA130[];

void ov14_021F27D4(Work*w);

void ov14_021F27D4(Work*w){ov14_021E84A4(*(void**)((u8*)w->ctx+0x2f0));ov14_021E8328(*(void**)((u8*)w->ctx+0x2f0));ov14_021E884C(w->ctx);ov14_021F40E8(w,0);ov14_021F0234(w,ov14_021EA130,89);}
