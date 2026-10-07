#pragma force_active on
#include "global.h"
#include "application/pokedex/pokedex_internal.h"
#pragma force_active on
extern void ov18_021F4A6C(void*),ov18_021F4D64(PokedexAppData*),ov18_021F4DDC(PokedexAppData*),ov18_021F4E28(PokedexAppData*);
extern ManagedSprite* ov18_021F11EC(void*,const ManagedSpriteTemplate*);
extern const ManagedSpriteTemplate ov18_021FA7B0[];
void ov18_021F49F8(PokedexAppData*);
void ov18_021F49F8(PokedexAppData*w){u32 i;ov18_021F4A6C(w);for(i=1;i<10;i++)w->sprites[i]=ov18_021F11EC(w,&ov18_021FA7B0[i-1]);ov18_021F4D64(w);ov18_021F4DDC(w);ov18_021F4E28(w);ov18_021F11C0(w,3,0);ov18_021F11C0(w,5,0);}
