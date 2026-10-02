#pragma force_active on
#include "global.h"
typedef struct{int value,count,angle,amplitude,step;} Work;
int ov07_022224AC(Work *);
int ov07_022224AC(Work *w){if(!w)GF_AssertFail();if(w->count){w->angle=(u16)(w->angle+w->step);w->count--;{s64 cosine=FX_SinCosTable_[(w->angle>>4)*2+1];w->value=(int)((cosine*(s64)w->amplitude+2048)>>12)>>12;}return 1;}return 0;}
