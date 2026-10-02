#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct {u8 pad[0x90];void *system,*manager;Sprite *sprites[5];} Work;
extern u8 ov109_021E7A58[];extern Sprite *SpriteSystem_CreateSpriteFromResourceHeader(void *,void *,void *);
void ov109_021E6EE4(Work *);
void ov109_021E6EE4(Work *w){u8 *template=ov109_021E7A58;int i;for(i=0;i<5;i++){w->sprites[i]=SpriteSystem_CreateSpriteFromResourceHeader(w->system,w->manager,template);Sprite_SetDrawFlag(w->sprites[i],1);Sprite_SetAnimSpeed(w->sprites[i],0x1000);template+=40;}Sprite_SetAnimActiveFlag(w->sprites[0],0);Sprite_SetDrawFlag(w->sprites[1],0);Sprite_SetPriority(w->sprites[4],2);Sprite_SetAnimActiveFlag(w->sprites[4],1);}
