#pragma force_active on
#include "global.h"
extern void ov07_0221DD14(void *,int);
typedef struct {u8 p[0x44];u16 first:1,second:1,third:1,other:13;u16 pad;void *data;} Work;
void ov07_0221E664(Work *w);
void ov07_0221E664(Work *w){if(!w)GF_AssertFail();if(!w->data)GF_AssertFail();if(w->first==1){ov07_0221DD14((u8*)w->data+0x180,1);return;}if(w->second==1){ov07_0221DD14((u8*)w->data+0x180,2);return;}if(w->third==1)ov07_0221DD14((u8*)w->data+0x180,3);}
