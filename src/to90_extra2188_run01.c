#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 x,y;u32 rest[12];} Template;
typedef struct {u8 pad[0x66c];void *manager;void *sprites[24];} Work;
extern Template sLanguageIconSpriteTemplate,ov18_021FAB24;void PokedexApp_CreateSpriteInSlot(Work *,int,Template *);int SpriteManager_FindPlttResourceOffset(void *,int,int);void ManagedSprite_SetPaletteOverride(void *,int);void PokedexApp_CreateLanguageIconSprites(Work *);
void PokedexApp_CreateLanguageIconSprites(Work *w){Template t=sLanguageIconSpriteTemplate;u32 i;int x;for(i=18,x=0x1b0;i<=23;i++){t.x=x-0x134;PokedexApp_CreateSpriteInSlot(w,i,&t);ManagedSprite_SetPaletteOverride(w->sprites[i],SpriteManager_FindPlttResourceOffset(w->manager,50522,2));x+=24;}PokedexApp_CreateSpriteInSlot(w,8,&ov18_021FAB24);}
