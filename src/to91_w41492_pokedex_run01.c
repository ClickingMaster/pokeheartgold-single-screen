#pragma force_active on
#include "global.h"
#include "application/pokedex/pokedex_internal.h"
#include "sprite_system.h"
#pragma force_active on
extern const ManagedSpriteTemplate ov18_021FA484,ov18_021FAB58,ov18_021FAB8C;
void ov18_021F17FC(void*);void ov18_021F1CB4(void*);void ov18_021F1E70(void*);void ov18_021F2724(void*);void ov18_021F2348(void*);
void ov18_021F1A30(void*,int);void ov18_021F1D98(void*,int);void ov18_021F1FDC(void*,int);void ov18_021F2424(void*,int,const ManagedSpriteTemplate*);
void ov18_021F3190(PokedexAppData *app){ManagedSpriteTemplate template;u32 i;int coordinate;
 ov18_021F17FC(app);ov18_021F1CB4(app);ov18_021F1E70(app);ov18_021F2724(app);ov18_021F2348(app);
 app->sprites[44]=SpriteSystem_NewSpriteWithYOffset(app->spriteSystem,app->unk_066C,&ov18_021FAB58,0x200000);
 app->sprites[45]=SpriteSystem_NewSpriteWithYOffset(app->spriteSystem,app->unk_066C,&ov18_021FAB8C,0x200000);
 ov18_021F1A30(app,46);ov18_021F1D98(app,48);ov18_021F1FDC(app,49);
 template=ov18_021FA484;
 for(i=53,coordinate=1272;i<=58;i++){template.x=coordinate-1148;ov18_021F2424(app,i,&template);ManagedSprite_SetPaletteOverride(app->sprites[i],SpriteManager_FindPlttResourceOffset(app->unk_066C,50522,2));coordinate+=24;}
 for(i=44;i<=58;i++){ManagedSprite_SetDrawFlag(app->sprites[i],FALSE);ManagedSprite_SetPriority(app->sprites[i],2);}
}
