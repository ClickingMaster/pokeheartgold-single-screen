#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *parent;u32 pad4;void *sprites;int state;void *picture,*sprite;u8 motion[0x24];u8 fade[0x28];s16 x,y;int repeat,tick;} Work;
void ov07_0222DE30(Work*);
BOOL ov07_022222B4(void*);
void ov07_0222207C(void*,void*,s16,s16);
void ov07_02222AC4(void*,int,int,int,int,int);
BOOL ov07_02222AF4(void*);
void ManagedSprite_SetOamMode(void*,int);
void ManagedSprite_SetDrawFlag(void*,BOOL);
void Pokepic_StartPaletteFade(void*,int,int,int,int);
BOOL Pokepic_ResumePaletteFade(void*);
void Sprite_DeleteAndFreeResources(void*);
void SpriteSystem_DrawSprites(void*);
void Heap_Free(void*);
void BattleAnimSystem_DestroyTask(void*,void*);
void ov07_0222DEEC(void*,Work*);
void ov07_0222DEEC(void *task,Work *w){
 switch(w->state){
 case 0:if(++w->tick>4){ov07_0222DE30(w);w->state++;w->tick=0;}break;
 case 1:
  if(ov07_022222B4(w->motion))ov07_0222207C(w->motion,w->sprite,w->x,w->y);
  else if(++w->repeat<6){w->state=0;w->x+=*(s16*)w->motion;w->y+=*(s16*)(w->motion+2);}
  else {w->state++;ManagedSprite_SetOamMode(w->sprite,1);ov07_02222AC4(w->fade,16,0,0,16,16);Pokepic_StartPaletteFade(w->picture,0,10,0,0x7fff);}break;
 case 2:
  if(ov07_02222AF4(w->fade))ManagedSprite_SetDrawFlag(w->sprite,0);
  if(!Pokepic_ResumePaletteFade(w->picture)){w->state++;Pokepic_StartPaletteFade(w->picture,10,0,0,0x7fff);}break;
 case 3:if(!Pokepic_ResumePaletteFade(w->picture))w->state++;break;
 case 4:Sprite_DeleteAndFreeResources(w->sprite);BattleAnimSystem_DestroyTask(w->parent,task);Heap_Free(w);return;
 }
 SpriteSystem_DrawSprites(w->sprites);
}
