#pragma force_active on
#include "global.h"
typedef struct{void*sprite[3];s32 values[3];VecFx32 positions[3];}Work;
extern void Sprite_SetMatrix(void*,VecFx32*);void ov91_0225D884(Work*,int);
void ov91_0225D884(Work*w,int i){VecFx32 pos;fx32 y=FX_Div((fx32)(((s64)(w->values[i]<<12)*-65536+2048)>>12),65536);pos=w->positions[i];pos.y+=y;Sprite_SetMatrix(w->sprite[i],&pos);}
