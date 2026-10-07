#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 species,flag;} Entry;typedef struct {void *dex;} Save;typedef struct {Save *save;u8 p[0x66c];void *sprites[8];u8 p2[0x9a0];Entry entries[];} Work;
extern void ManagedSprite_SetDrawFlag(void *,int);extern u32 Pokedex_GetSeenFormByIdx(void *,u32,u32);extern void ov18_021F14FC(Work *,u32,u32,int);extern void ov18_021F1160(Work *,int,int);void ov18_021F1598(Work *,int,int);
void ov18_021F1598(Work *w,int index,int sprite){void **sprites=w->sprites;Entry *volatile entries;u32 form,species,offset;ManagedSprite_SetDrawFlag(sprites[sprite],0);offset=(u32)index*4;{Entry *base=w->entries;species=((Entry*)((u8*)base+offset))->species;entries=base;}if(species!=0){form=Pokedex_GetSeenFormByIdx(w->save->dex,species,0);species=((Entry*)((u8*)entries+offset))->species;if(species==172){if(form==2)form=1;else form=0;}ov18_021F14FC(w,species,form,sprite);ManagedSprite_SetDrawFlag(sprites[sprite],1);if(((Entry*)((u8*)w->entries+offset))->flag==1)ov18_021F1160(w,sprite,1);else ov18_021F1160(w,sprite,0);}}
