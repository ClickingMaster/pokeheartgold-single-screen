#pragma force_active on
#include "global.h"
typedef struct{u8 yes;u8 pad[7];}Item;typedef struct{u8 pad[16];u8*data;int selected;u8 gap[0x458];Item items[7];}Work;
int ov27_0225C1AC(Work*w,int index);
void ov27_0225C1EC(Work*w);
#pragma force_active on
void ov27_0225C1EC(Work*w){int i;u8*p;GF_ASSERT(w->data);if(!w->items[w->selected].yes){i=0;p=(u8*)w;for(;i<7;i++,p+=8){if(p[0x470]){w->data[0xd3]=i;w->selected=ov27_0225C1AC(w,w->data[0xd3]);return;}}}}
