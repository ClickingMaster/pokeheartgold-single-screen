#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work{u8 pad0[0x210];struct {int index;u8 pad214[0x3a];s16 meter;} data;}Work;
extern u8 ov28_0225EB84[],ov28_0225EA90[];void ov28_0225E31C(Work*,int);
#pragma opt_propagation off
void ov28_0225E31C(Work*w,int count){if(count>=0){int add=count*ov28_0225EB84[w->data.index];w->data.meter=w->data.meter+add;int limit=ov28_0225EA90[w->data.index*2];if(w->data.meter>limit)w->data.meter=limit;}else if(w->data.meter>0)w->data.meter--;}
