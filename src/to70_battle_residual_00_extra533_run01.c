#pragma force_active on
#include "global.h"
typedef struct {int v[10];} Config;
typedef struct {u8 p[0x88];void*effect;u8 q[0x194-0x8c];u8 battler,kind,flag;} Work;
extern u8 ov12_0226D120[];extern int BattleSystem_GetBattleType(void*);extern void* BattleSystem_GetSpriteSystem(void*),*BattleSystem_GetPaletteData(void*),*UnkBallData_New(Config*);extern void UnkBallData_SetDrawPriority(void*,int),UnkBallData_SetPriority(void*,int),UnkBallData_SetDrawFlag(void*,int),UnkBallData_SetAnimateSprite(void*,int);
void ov12_02258DB0(void*,Work*,int);
void ov12_02258DB0(void*b,Work*w,int arg){Config c;if((BattleSystem_GetBattleType(b)&0x220)==0){if((w->kind&1)==0 || (BattleSystem_GetBattleType(b)&1)){c.v[0]=ov12_0226D120[w->kind];c.v[1]=5;c.v[2]=4;c.v[3]=w->battler;c.v[4]=arg;c.v[7]=(int)BattleSystem_GetSpriteSystem(b);c.v[8]=(int)BattleSystem_GetPaletteData(b);c.v[6]=0;c.v[5]=1;w->effect=UnkBallData_New(&c);UnkBallData_SetDrawPriority(w->effect,100);UnkBallData_SetPriority(w->effect,2);UnkBallData_SetDrawFlag(w->effect,0);UnkBallData_SetAnimateSprite(w->effect,0);}}}
