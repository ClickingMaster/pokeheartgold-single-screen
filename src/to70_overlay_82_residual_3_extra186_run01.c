#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work{u8 pad0[0x48];void*config;u8 pad4c[0x48];void*pal;}Work;
extern void ov82_0223EC48(Work*),ov82_0223EC68(void*),ov82_0223ED94(Work*),PaletteData_AllocBuffers(void*,int,int,int),ov82_0223EDF0(Work*,int),ov82_0223EE38(void),GfGfx_EngineATogglePlanes(int,int),ov82_0223EE6C(Work*,int);extern void*PaletteData_Init(int);void ov82_0223EB3C(Work*);
void ov82_0223EB3C(Work*w){ov82_0223EC48(w);ov82_0223EC68(w->config);ov82_0223ED94(w);w->pal=PaletteData_Init(105);PaletteData_AllocBuffers(w->pal,2,512,105);PaletteData_AllocBuffers(w->pal,0,512,105);ov82_0223EDF0(w,3);ov82_0223EE38();GfGfx_EngineATogglePlanes(4,0);ov82_0223EE6C(w,5);}
