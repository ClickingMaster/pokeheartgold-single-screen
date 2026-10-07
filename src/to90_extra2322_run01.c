#pragma force_active on
#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{void*data;volatile u32 size;} Work;void ov00_021EC2EC(int,void*,u32);Work*ov45_0223ECB0(Work*);
static inline void init(u8*p){*p=0;}
Work*ov45_0223ECB0(Work*w){if(w->data){u32 old=w->size;struct{volatile u8 zero;} init={0};w->size-=old;if(w->data)ov00_021EC2EC(0,w->data,0);}return w;}
