#pragma force_active on
#include "global.h"
#include "application/pokedex/pokedex_internal.h"
#include "sprite_system.h"
#pragma force_active on
extern const ManagedSpriteTemplate sLanguageIconSpriteTemplate,ov18_021FAB58,ov18_021FAB8C;
void PokedexApp_LoadMonPicResources(void*);void PokedexApp_LoadFootprintResources(void*);void PokedexApp_LoadTypeIconResources(void*);void ov18_021F2724(void*);void PokedexApp_LoadLanguageIconResourcesSub(void*);
void PokedexApp_CreateMonPicSprites(void*,int);void PokedexApp_CreateFootprintSprite(void*,int);void PokedexApp_CreateTypeIconSprites(void*,int);void PokedexApp_CreateSpriteInSlot(void*,int,const ManagedSpriteTemplate*);
void PokedexApp_CreateSearchEntrySprites(PokedexAppData *app){ManagedSpriteTemplate template;u32 i;int coordinate;
 PokedexApp_LoadMonPicResources(app);PokedexApp_LoadFootprintResources(app);PokedexApp_LoadTypeIconResources(app);ov18_021F2724(app);PokedexApp_LoadLanguageIconResourcesSub(app);
 app->sprites[44]=SpriteSystem_NewSpriteWithYOffset(app->spriteSystem,app->unk_066C,&ov18_021FAB58,0x200000);
 app->sprites[45]=SpriteSystem_NewSpriteWithYOffset(app->spriteSystem,app->unk_066C,&ov18_021FAB8C,0x200000);
 PokedexApp_CreateMonPicSprites(app,46);PokedexApp_CreateFootprintSprite(app,48);PokedexApp_CreateTypeIconSprites(app,49);
 template=sLanguageIconSpriteTemplate;
 for(i=53,coordinate=1272;i<=58;i++){template.x=coordinate-1148;PokedexApp_CreateSpriteInSlot(app,i,&template);ManagedSprite_SetPaletteOverride(app->sprites[i],SpriteManager_FindPlttResourceOffset(app->unk_066C,50522,2));coordinate+=24;}
 for(i=44;i<=58;i++){ManagedSprite_SetDrawFlag(app->sprites[i],FALSE);ManagedSprite_SetPriority(app->sprites[i],2);}
}
