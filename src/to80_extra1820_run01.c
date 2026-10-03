#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov91_0225D768(void*,int,int,int);
void ov91_0225D694(u8*w,int index,int a,int b);
void ov91_0225D694(u8*w,int index,int a,int b){ov91_0225D768(w+(index<<6),index,a,b);}
