#pragma force_active on
#include "global.h"
typedef struct{void*sprite;u8 pad[0xf4-4];int state;void*data;u8 gap[8];u16 tick;}Work;typedef struct{u8 a[4];u16 pal;}Data;extern void Sprite_SetPaletteOverride(void*,u16),ov91_02260A88(Work*,Data*,void*),Sprite_SetDrawFlag(void*,int);

void ov91_02260848(void*unused,Work*w,Data*d,void*p);

void ov91_02260848(void*unused,Work*w,Data*d,void*p){w->data=d;w->state=1;w->tick=0;Sprite_SetPaletteOverride(w->sprite,d->pal);ov91_02260A88(w,d,p);Sprite_SetDrawFlag(w->sprite,1);}
