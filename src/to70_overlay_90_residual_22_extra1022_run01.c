#pragma force_active on
#include "global.h"
typedef struct {u8 pad[28];u8 flags,value;} Work;extern void Comm_SendCommand(int,int,int);

void ov90_0225B53C(Work*w,int index,int keep,int required);

void ov90_0225B53C(Work*w,int index,int keep,int required){int bit=1<<index,i;u32 count;if(!(w->flags&bit)){if(keep==0)w->value=0;w->flags|=bit;}count=0;for(i=count;(int)i<4;i++)if(w->flags&(1<<i))count++;if(count==required){if(w->value)Comm_SendCommand(24,0,0);else Comm_SendCommand(25,0,0);}}
