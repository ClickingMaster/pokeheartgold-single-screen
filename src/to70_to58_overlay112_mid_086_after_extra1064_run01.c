#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x1f2d0];volatile int ticks;}Work;extern void PokewalkerApp_ShowSprite(Work*,int),PokewalkerApp_SpawnFallingParticle(Work*);extern u32 MTRandom(void);

int ov112_021EC338(Work*w);

int ov112_021EC338(Work*w){w->ticks++;if(w->ticks>180){PokewalkerApp_ShowSprite(w,8);return 26;}if((u32)w->ticks>MTRandom()%180)PokewalkerApp_SpawnFallingParticle(w);return 25;}
