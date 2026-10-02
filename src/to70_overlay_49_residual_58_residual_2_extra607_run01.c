#pragma force_active on
#include "global.h"
typedef struct {s32 value;volatile s8 step;u8 state,timer,repeat;} Work;
extern const s16 FX_SinCosTable_[];extern u32 MTRandom(void);s32 ov49_0225F394(Work*);
s32 ov49_0225F394(Work*w){switch(w->state){case 0:w->value=0;break;case 1:{int t=w->timer-1;if(t>0)w->timer=t;else w->state=2;break;}case 2:{int angle;w->step++;angle=w->step*0x7fff/4;w->value=(s32)(((s64)FX_SinCosTable_[((u16)angle>>4)*2]*0x6000+0x800)>>12);if(w->step>=4){if(w->repeat==1){w->step=0;w->state=1;w->timer=MTRandom()&3;}else w->state=0;}break;}}return w->value;}
