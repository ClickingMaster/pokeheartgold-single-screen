#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 species,flag;} Entry;typedef struct {void *dex;} Save;typedef struct {Save *save;u8 p[0x66c];void *sprites[8];u8 p2[0x9a0];Entry entries[];} Work;
extern void ManagedSprite_SetDrawFlag(void *,int);extern u32 Pokedex_GetSeenFormByIdx(void *,u32,u32);extern void PokedexApp_SetMonIcon(Work *,u32,u32,int);extern void PokedexApp_SetSpriteTranslucent(Work *,int,int);void PokedexApp_SetListIcon(Work *,int,int);
void PokedexApp_SetListIcon(Work *w,int index,int sprite){void **sprites=w->sprites;Entry *volatile entries;u32 form,species,offset;ManagedSprite_SetDrawFlag(sprites[sprite],0);offset=(u32)index*4;{Entry *base=w->entries;species=((Entry*)((u8*)base+offset))->species;entries=base;}if(species!=0){form=Pokedex_GetSeenFormByIdx(w->save->dex,species,0);species=((Entry*)((u8*)entries+offset))->species;if(species==172){if(form==2)form=1;else form=0;}PokedexApp_SetMonIcon(w,species,form,sprite);ManagedSprite_SetDrawFlag(sprites[sprite],1);if(((Entry*)((u8*)w->entries+offset))->flag==1)PokedexApp_SetSpriteTranslucent(w,sprite,1);else PokedexApp_SetSpriteTranslucent(w,sprite,0);}}
