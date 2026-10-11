#pragma force_active on
#include "global.h"
#include "application/pokedex/pokedex_internal.h"
#pragma force_active on
extern void PokedexApp_LoadSizeCheckResources(void*),PokedexApp_SetSizeCheckMonIcons(PokedexAppData*),PokedexApp_DrawSizeCheckMonPic(PokedexAppData*),PokedexApp_ScaleSizeCheckTrainer(PokedexAppData*);
extern ManagedSprite* PokedexApp_NewSprite(void*,const ManagedSpriteTemplate*);
extern const ManagedSpriteTemplate ov18_021FA7B0[];
void PokedexApp_SetupSizeCheckSprites(PokedexAppData*);
void PokedexApp_SetupSizeCheckSprites(PokedexAppData*w){u32 i;PokedexApp_LoadSizeCheckResources(w);for(i=1;i<10;i++)w->sprites[i]=PokedexApp_NewSprite(w,&ov18_021FA7B0[i-1]);PokedexApp_SetSizeCheckMonIcons(w);PokedexApp_DrawSizeCheckMonPic(w);PokedexApp_ScaleSizeCheckTrainer(w);PokedexApp_SetSpriteVisible(w,3,0);PokedexApp_SetSpriteVisible(w,5,0);}
