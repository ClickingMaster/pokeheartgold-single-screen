#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[8];void *party;u8 gap[0x34-12];u8 *data;}Work;
extern u32 Party_GetCount(void*);
extern void *Party_GetMonByIndex(void*,int),*Mon_GetBoxMon(void*);
extern void ov14_021F2EA0(Work*,void*,int),ov14_021F2A18(void*,int,int);

void ov14_021F2F3C(Work*w);

void ov14_021F2F3C(Work*w){u32 count=Party_GetCount(w->party),i;for(i=0;i<count;i++){void*mon=Mon_GetBoxMon(Party_GetMonByIndex(w->party,i));ov14_021F2EA0(w,mon,w->data[0x40b2+i]);ov14_021F2A18(w->data,w->data[0x40b2+i],0);}}
