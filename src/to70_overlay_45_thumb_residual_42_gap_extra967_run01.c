#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[2];u8 tick,step;u32 p2;void *object;} Work;extern u8 ov45_02254C34[];extern void *ov45_022309C4(int,int);extern void sub_02023EE0(void *,void *),sub_02023F40(void *,int);void ov45_02230CD8(Work *);
void ov45_02230CD8(Work *w){if(((int)w->tick)%4==0){sub_02023EE0(w->object,ov45_022309C4(1,ov45_02254C34[(u32)w->tick>>2]));sub_02023F40(w->object,0);}if(w->tick+w->step<16)w->tick+=w->step;else w->tick=0;}
