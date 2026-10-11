#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {s16 x,y,z;u16 animation;int priority,pal,vram,ids[6],bgPriority,transfer;} Template;
extern void *SpriteSystem_NewSprite(void *,void *,const Template *);
extern void *SpriteSystem_NewSpriteWithYOffset(void *,void *,const Template *,int);
typedef struct{u8 pad[0x668];void *system,*manager,*sprites[44];u8 rest[0x188c-0x720];int mode;} Work;
extern const Template ov18_021FB004[],ov18_021FA520,ov18_021FB54C,ov18_021FB580,ov18_021FA4EC;
extern void PokedexApp_SetSpriteVisible(Work *,int,int),PokedexApp_SetSpriteAnim(Work *,int,int),PokedexApp_CreateMonIconSprites(Work *,int);
void PokedexApp_CreateSearchSprites(Work *);
void PokedexApp_CreateSearchSprites(Work *w){u16 i;Template temp;
for(i=0;i<=25;i++){w->sprites[i]=SpriteSystem_NewSprite(w->system,w->manager,&ov18_021FB004[i]);PokedexApp_SetSpriteVisible(w,i,0);}
w->sprites[43]=SpriteSystem_NewSpriteWithYOffset(w->system,w->manager,&ov18_021FA520,0x200000);PokedexApp_SetSpriteVisible(w,43,0);
w->sprites[26]=SpriteSystem_NewSpriteWithYOffset(w->system,w->manager,&ov18_021FB54C,0x200000);
w->sprites[27]=SpriteSystem_NewSpriteWithYOffset(w->system,w->manager,&ov18_021FB580,0x200000);PokedexApp_SetSpriteVisible(w,27,0);
temp=ov18_021FA4EC;
for(i=28;i<=42;i++){if(i==28){temp.x=224;temp.y=72;w->sprites[i]=SpriteSystem_NewSprite(w->system,w->manager,&temp);if(w->mode==14)PokedexApp_SetSpriteVisible(w,i,0);else PokedexApp_SetSpriteAnim(w,i,w->mode);}else{u16 index=i-29;temp.x=32+48*(index%5);temp.y=56+40*(index/5);w->sprites[i]=SpriteSystem_NewSprite(w->system,w->manager,&temp);PokedexApp_SetSpriteAnim(w,i,index);PokedexApp_SetSpriteVisible(w,i,0);}}
PokedexApp_CreateMonIconSprites(w,59);}
