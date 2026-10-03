#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x1f2d0];volatile int ticks;}Work;extern void ov112_021EA670(Work*,int),ov112_021EA7D0(Work*);extern u32 MTRandom(void);

int ov112_021EC338(Work*w);

int ov112_021EC338(Work*w){w->ticks++;if(w->ticks>180){ov112_021EA670(w,8);return 26;}if((u32)w->ticks>MTRandom()%180)ov112_021EA7D0(w);return 25;}
