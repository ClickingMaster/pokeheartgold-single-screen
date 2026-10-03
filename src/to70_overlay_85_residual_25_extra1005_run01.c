#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct{u8 p[0x30];s32 value;} Object;
typedef struct{u32 pad;int state;u32 unused;s32 step;Object *obj;} Work;
extern void ov85_021E8530(u32 *,u32);
void ov85_021E7DC8(Work *);
void ov85_021E7DC8(Work *w){Object *o=w->obj;switch(w->state){case 0:w->step=98304;o->value=0;w->state++;case 1:ov85_021E8530(&o->value,w->step);if(o->value<16384){w->step=(u32)w->step>>1;if(w->step<16384){o->value=0;memset(w,0,20);}}break;}}
