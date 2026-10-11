#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int a;void*source;u8 pad[8];void*pic,*sprite;s16 x,y;u8 p1[12];int scale;u8 p2[0x68-0x2c];s16 attr;}Work;
extern Work *BattleAnimUtil_AllocWork(void*,int);
extern void *BattleAnimSystem_GetBattlerSpriteManager(void*),*BattleAnimSystem_GetBattlerPokepic(void*,int),*BattleAnimSystem_GetBattlerSprite(void*,int);
extern int BattleAnimSystem_GetAttacker(void*),ov07_02222004(void*,int),Pokepic_GetAttr(void*,int);
extern void ManagedSprite_SetOamMode(void*,int),Pokepic_SetAttr(void*,int,int),ov07_02222A44(void*,int,int),BattleAnimUtil_SetBlendWithBattleBg(void*,int,int),ov07_02222AC4(void*,int,int,int,int,int),BattleAnimSystem_CreateTaskDefaultPriority(void*,void(*)(void),void*),ov07_0222DA60(void),SpriteSystem_DrawSprites(void*);

void ov07_0222DB14(void*arg);

void ov07_0222DB14(void*arg){Work*w=BattleAnimUtil_AllocWork(arg,108);int factor;w->a=(int)arg;w->source=BattleAnimSystem_GetBattlerSpriteManager(arg);factor=ov07_02222004((void*)w->a,BattleAnimSystem_GetAttacker((void*)w->a));w->pic=BattleAnimSystem_GetBattlerPokepic((void*)w->a,BattleAnimSystem_GetAttacker(arg));w->x=Pokepic_GetAttr(w->pic,0);w->y=Pokepic_GetAttr(w->pic,1);w->attr=Pokepic_GetAttr(w->pic,41);w->y+=8;w->sprite=BattleAnimSystem_GetBattlerSprite((void*)w->a,0);ManagedSprite_SetOamMode(w->sprite,1);Pokepic_SetAttr(w->pic,6,1);ov07_02222A44((u8*)w+28,2,16);BattleAnimUtil_SetBlendWithBattleBg((void*)w->a,16,0);ov07_02222AC4((u8*)w+64,16,0,0,16,32);w->scale*=factor;BattleAnimSystem_CreateTaskDefaultPriority((void*)w->a,ov07_0222DA60,w);SpriteSystem_DrawSprites(w->source);}
