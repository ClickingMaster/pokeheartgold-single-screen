#pragma force_active on
#include "global.h"
typedef struct {u32 words[13];} Template;
extern const Template ov18_021FA3E8;extern void *SpriteSystem_NewSprite(void *,void *,Template *);
void ov18_021F1424(void *,u32);
#pragma force_active on
void ov18_021F1424(void *ctx,u32 index){Template current,base;u32 i;u8 *slot;i=0;slot=(u8*)ctx+index*4;base=ov18_021FA3E8;for(;i<60;i++){current=base;current.words[5]=0xc550+i;*(void**)(slot+0x670)=SpriteSystem_NewSprite(*(void**)((u8*)ctx+0x668),*(void**)((u8*)ctx+0x66c),&current);slot+=4;}}
