#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Tag{u8 value;}Tag;
void ov45_022387E4(u32*,u32,Tag,u32);
void ov00_021EC2EC(u32,u32,u32);
u32*ov45_02237664(u32*);
static inline Tag tag(void){Tag t;t.value=0;return t;}
u32*ov45_02237664(u32*p){if(p[0]){u32 current=p[1];Tag t={0};ov45_022387E4(p,current,t,0);if(p[0])ov00_021EC2EC(0,p[0],0);}return p;}
