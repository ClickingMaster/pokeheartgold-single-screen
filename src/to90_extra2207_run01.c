#pragma force_active on
#include "global.h"
typedef struct {u32 words[13];} Template;
extern const Template sMonIconSpriteTemplate;extern void *SpriteSystem_NewSprite(void *,void *,Template *);
void PokedexApp_CreateMonIconSprites(void *,u32);
#pragma force_active on
void PokedexApp_CreateMonIconSprites(void *ctx,u32 index){Template current,base;u32 i;u8 *slot;i=0;slot=(u8*)ctx+index*4;base=sMonIconSpriteTemplate;for(;i<60;i++){current=base;current.words[5]=0xc550+i;*(void**)(slot+0x670)=SpriteSystem_NewSprite(*(void**)((u8*)ctx+0x668),*(void**)((u8*)ctx+0x66c),&current);slot+=4;}}
