#pragma force_active on
#include "global.h"
#include "heap.h"
#include "pokepic.h"
#include "sprite.h"
#include "palette.h"
#pragma force_active on
typedef struct R2Motion{s16 x,y;s32 duration,value8,valueC,currentX,currentY,deltaX,deltaY,deltaTail;}R2Motion;
typedef struct Motion24{s16 x,y;u8 raw[0x20];}Motion24;
typedef struct W35Fade{void*owner;int state;Pokepic*pic;ManagedSprite*sprite;int special;Motion24 motion;s16 x,y;int delta;}W35Fade;
typedef char AssertSize[(sizeof(W35Fade)==64)?1:-1];
extern int BattleAnimSystem_GetHeapID(void*),BattleAnimSystem_GetDefender(void*),ov07_02222004(void*,int),BattleAnimSystem_GetAttacker(void*),BattleAnimSystem_IsBattlerSemiInvulnerable(u8*,int);
extern int BattleAnimUtil_GetBattlerType(void*,int);extern u8 BattleAnimUtil_GetSpritePaletteSlot(void**);
extern Pokepic*BattleAnimSystem_GetBattlerPokepic(void*,int);extern ManagedSprite*BattleAnimSystem_GetBattlerSprite(void*,int);extern PaletteData*BattleAnimSystem_GetPaletteData(void*);
extern void BattleAnimUtil_InitShake(R2Motion*,int,int,int,int);
extern void BattleAnimSystem_CreateTaskDefaultPriority(void*,void*,void*),ov07_0222CCBC(int,W35Fade*);
void ov07_0222CEC0(void*);
void ov07_0222CEC0(void *ctx){W35Fade *p;u32 mask,shift,first,second;p=Heap_Alloc((enum HeapID)BattleAnimSystem_GetHeapID(ctx),sizeof(W35Fade));memset(p,0,64);p->owner=ctx;p->pic=BattleAnimSystem_GetBattlerPokepic(p->owner,BattleAnimSystem_GetDefender(ctx));p->x=Pokepic_GetAttr(p->pic,0);p->y=Pokepic_GetAttr(p->pic,1);p->delta=Pokepic_GetAttr(p->pic,41);BattleAnimUtil_InitShake((R2Motion*)&p->motion,4,0,1,4);p->motion.x=(s16)((u32)(s32)p->motion.x+(u32)ov07_02222004(p->owner,BattleAnimSystem_GetDefender(p->owner)));Pokepic_StartPaletteFade(p->pic,0,16,0,0);p->sprite=BattleAnimSystem_GetBattlerSprite(p->owner,1);first=BattleAnimUtil_GetBattlerType(p->owner,BattleAnimSystem_GetAttacker(p->owner));second=BattleAnimUtil_GetBattlerType(p->owner,BattleAnimSystem_GetDefender(p->owner));if((first==2&&second==4)||(first==5&&second==3)){shift=BattleAnimUtil_GetSpritePaletteSlot((void**)p->sprite);mask=1;mask<<=shift;PaletteData_BeginPaletteFade(BattleAnimSystem_GetPaletteData(p->owner),4,(u16)mask,0,0,16,0);p->special=1;if(!BattleAnimSystem_IsBattlerSemiInvulnerable((u8*)p->owner,BattleAnimSystem_GetDefender(p->owner)))Pokepic_SetAttr(p->pic,6,1);}BattleAnimSystem_CreateTaskDefaultPriority(p->owner,(void*)ov07_0222CCBC,p);}
