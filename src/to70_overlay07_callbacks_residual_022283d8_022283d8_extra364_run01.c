#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct State {u8 step,timer;u16 pad;void *pal,*owner,*unused,*pic;} State;
u32 ov07_0221E6C8(void *);void PaletteData_BeginPaletteFade(void *,int,u16,int,int,int,int),Pokepic_StartPaletteFade(void *,int,int,int,int),ov07_0221C448(void *,void *),Heap_Free(void *),ov07_022283D8(void *,State *);int PaletteData_GetSelectedBuffersBitmask(void *),Pokepic_ResumePaletteFade(void *);
void ov07_022283D8(void *task,State *s){switch(s->step){case 0:PaletteData_BeginPaletteFade(s->pal,1,(u16)ov07_0221E6C8(s->owner),-2,0,16,0x7fff);Pokepic_StartPaletteFade(s->pic,0,16,0,0);s->step++;break;case 1:if(PaletteData_GetSelectedBuffersBitmask(s->pal)==0&&Pokepic_ResumePaletteFade(s->pic)==0)s->step++;break;case 2:s->timer++;if(s->timer>5)s->step++;break;case 3:PaletteData_BeginPaletteFade(s->pal,1,(u16)ov07_0221E6C8(s->owner),-2,16,0,0x7fff);Pokepic_StartPaletteFade(s->pic,16,0,0,0);s->step++;break;case 4:if(PaletteData_GetSelectedBuffersBitmask(s->pal)==0&&Pokepic_ResumePaletteFade(s->pic)==0)s->step++;break;default:ov07_0221C448(s->owner,task);Heap_Free(s);break;}}
