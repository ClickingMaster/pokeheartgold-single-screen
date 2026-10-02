#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work{u32 state;void*owner;u16 speed,pad;void*callback;}Work;
extern void*Heap_Alloc(int,int);Work*ov92_0225EB40(void*,void*);
Work*ov92_0225EB40(void*owner,void*callback){Work*w=Heap_Alloc(113,16);memset(w,0,16);w->owner=owner;w->speed=1200;w->callback=callback;return w;}
