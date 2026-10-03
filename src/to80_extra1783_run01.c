#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x3c];u16 value;}Work;
void ov48_0225A428(Work*w);
void ov48_0225A428(Work*w){w->value=(u32)w->value>>1;}
