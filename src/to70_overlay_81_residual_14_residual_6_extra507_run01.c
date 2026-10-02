#pragma force_active on
#include "global.h"
#include "palette.h"
typedef struct {u8 pad[0x1a0];PaletteData *pal;u8 gap[0x478-0x1a4];int state;u8 gap2[8];u8 a,b,c,d,e,f,g,h;} Work;

void ov81_02242514(Work *w);
void ov81_0224258C(Work*w,u8 a,u8 b,u8 c,u8 d,u8 h);

void ov81_02242514(Work *w){if(w->pal!=0 && w->state!=255 && PaletteData_GetSelectedBuffersBitmask(w->pal)==0){if(w->state==0)PaletteData_BeginPaletteFade(w->pal,4,8,2,0,16,65535);else PaletteData_BeginPaletteFade(w->pal,4,8,2,16,0,65535);w->state^=1;}}
void ov81_0224258C(Work*w,u8 a,u8 b,u8 c,u8 d,u8 h){w->a=a;w->b=b;w->c=c;w->d=d;w->h=h;w->e=0;w->f=0;w->g=0;}
