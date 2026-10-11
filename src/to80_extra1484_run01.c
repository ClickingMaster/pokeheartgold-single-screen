#pragma force_active on
#include "global.h"
#include <string.h>
typedef struct{u8 pad0[0x20];void*pic;u8 pad1[0x64];void*actor;u8 pad2[0x118];void*narc;}B;
typedef struct{void*bs;B*b;void*obj;u8 pad0[4];void*token;u8 picData[0x10];void*effect;u8 pad1[0x58];u8 unk80,side,position,state,kind,flag;u16 species;u32 field88;u8 party,pad8d;u16 color;s8 x,y;u8 hasEffect,picFlag,pad94[2],timer,field97;}W;
typedef struct{s16 a,b,c;}R;
typedef struct{R front[6],back[6];}Table;extern const Table ov07_022377F4;
extern u32 BattleSystem_GetBattleType(void*),BattleSystem_GetBattleSpecial(void*);extern int ov12_0223B688(void*),PokepicAnimManager_GetAnimState(void*,int),Pokepic_IsAnimFinished(void*),ov07_0221C3B0(void*);extern void*ov07_0221FDFC(void*,int),*BattleSystem_GetPokepicManager(void*),*ov12_022612A4(void*,void*,void*,int,int,int,int,int,int,int,int,void*,int),*ov07_0221BEDC(int),*ov12_0223B750(void*);
extern void NARC_ReadPokepicAnimScript(void*,void*,int,int),Pokepic_SetAttr(void*,int,int),ov07_0221FE08(void*),BattleController_InitMoveAnimData(void*,int,void*,int,int,int,int,int),ov12_02261B80(void*,B*,void*,void*),ov07_0221C394(void*),ov07_0221C3C0(void*),ov07_0221BFE0(void*),ov12_02261F38(void*,int,int,void*,void*,int,int,int,u32),ov07_02233ECC(void*),sub_02005B58(int),BattleController_EmitCommandComplete(void*,int,int),Heap_Free(void*),SysTask_Destroy(void*);
typedef struct{int position;int unused;void*mon;}Config;
extern const s16 ov07_022377DC[][2];extern const u16 ov12_0226D15A[];
extern void*BattleSystem_GetPartyMon(void*,int,int),*ov07_02232694(int,Config*);extern int ov07_02233F20(void*),ov07_02232A04(void*),ov07_02233EA0(void*),ov07_02233E88(void*),ov07_02232A54(void*),Pokepic_GetAttr(void*,int);
extern void ov07_022329B0(void*),Pokepic_StartPaletteFade(void*,int,int,int,int),ov07_02232A44(void*),PlaySE_SetPan(int,int),Pokepic_AddAttr(void*,int,int),sub_0200914C(void*,int),ov07_02232AB8(void*);
void ov12_0225B960(void*,W*);
#define SETUP() ov12_02261F38(w->bs,w->side,w->position,w->b->pic,w->b->narc,w->species,w->field97,w->kind,w->field88)
void ov12_0225B960(void*task,W*w){u32 effect[22];Config config;u32 anim[10];switch(w->state){
case 0:w->timer=0;w->token=0;if(BattleSystem_GetBattleType(w->bs)&8){if(!(BattleSystem_GetBattleSpecial(w->bs)&32)&&w->position==2)w->token=ov07_0221FDFC(w->bs,5);}else if(!(BattleSystem_GetBattleSpecial(w->bs)&32)){if(ov12_0223B688(w->bs)==1&&w->position==2)w->token=ov07_0221FDFC(w->bs,5);else if(w->position==0)w->token=ov07_0221FDFC(w->bs,5);}w->state++;return;
case 1:{memset(&config,0,sizeof(config));config.position=w->position;config.mon=BattleSystem_GetPartyMon(w->bs,w->side,w->party);w->obj=ov07_02232694(5,&config);ov07_022329B0(w->obj);w->state++;return;}
case 2:{void*manager;if(ov07_02233F20(w->b->actor)||ov07_02232A04(w->obj)!=1||ov07_02233EA0(w->b->actor)!=1)break;if(w->position==4){w->timer++;if(w->timer<12)break;w->timer=0;}manager=BattleSystem_GetPokepicManager(w->bs);NARC_ReadPokepicAnimScript(w->b->narc,anim,w->species,w->position);w->b->pic=ov12_022612A4(w->bs,manager,w->picData,ov07_022377DC[w->position][0],ov07_022377F4.front[w->position].b,ov07_022377F4.front[w->position].c,w->flag,w->x,w->y,w->picFlag,w->side,anim,0);Pokepic_SetAttr(w->b->pic,12,0);Pokepic_SetAttr(w->b->pic,13,0);Pokepic_SetAttr(w->b->pic,44,0);Pokepic_SetAttr(w->b->pic,6,1);Pokepic_StartPaletteFade(w->b->pic,16,16,0,ov12_0226D15A[w->color]);Pokepic_SetAttr(w->b->pic,6,0);ov07_02232A44(w->obj);if(w->kind==2)PlaySE_SetPan(1798,117);else PlaySE_SetPan(1798,-117);if(w->token){ov07_0221FE08(w->token);w->token=0;}w->state++;return;}
case 3:if(ov07_02233E88(w->b->actor)!=1)w->state++;
case 4:if(Pokepic_GetAttr(w->b->pic,12)==256&&!ov07_02232A54(w->obj)){if(w->kind==2)Pokepic_SetAttr(w->b->pic,45,0);SETUP();Pokepic_StartPaletteFade(w->b->pic,16,0,0,ov12_0226D15A[w->color]);w->state=5;return;}if(Pokepic_GetAttr(w->b->pic,12)>=256){Pokepic_SetAttr(w->b->pic,12,256);Pokepic_SetAttr(w->b->pic,13,256);if(w->kind==2)Pokepic_SetAttr(w->b->pic,45,0);SETUP();Pokepic_StartPaletteFade(w->b->pic,16,0,1,ov12_0226D15A[w->color]);w->state=5;return;}else{Pokepic_AddAttr(w->b->pic,12,32);Pokepic_AddAttr(w->b->pic,13,32);sub_0200914C(w->b->pic,w->x);}return;
case 5:if(!ov07_02232A54(w->obj))w->state=6;return;
case 6:if(PokepicAnimManager_GetAnimState(ov12_0223B750(w->bs),w->side)!=1||Pokepic_IsAnimFinished(w->b->pic))break;ov07_02233ECC(w->b->actor);w->b->actor=0;ov07_02232AB8(w->obj);if(w->hasEffect){w->effect=ov07_0221BEDC(5);BattleController_InitMoveAnimData(w->bs,0,effect,1,11,w->side,w->side,0);ov12_02261B80(w->bs,w->b,w->effect,effect);w->state=7;}else w->state=255;return;
case 7:ov07_0221C394(w->effect);if(ov07_0221C3B0(w->effect))break;ov07_0221C3C0(w->effect);ov07_0221BFE0(w->effect);w->state=255;return;
default:sub_02005B58(0);BattleController_EmitCommandComplete(w->bs,w->side,w->unk80);Heap_Free(w);SysTask_Destroy(task);break;}}
