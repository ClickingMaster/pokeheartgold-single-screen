#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[12];void *sprite;}Work;
extern const VecFx32 *Sprite_GetMatrixPtr(void*);
extern void Sprite_SetMatrix(void*,const VecFx32*);

void ov81_02242EC4(VecFx32*out,Work*w,int x,int y);

void ov81_02242EC4(VecFx32*out,Work*w,int x,int y){VecFx32 p=*Sprite_GetMatrixPtr(w->sprite);p.x+=x<<12;p.y+=y<<12;Sprite_SetMatrix(w->sprite,&p);*out=*Sprite_GetMatrixPtr(w->sprite);}
