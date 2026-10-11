#pragma force_active on
#include "global.h"
#include "battle_controller_opponent_r2_exact_private.h"
#include "pokemon.h"
#include "pokepic_anim_internal.h"
#include "battle_anim_ball_params.h"
#pragma force_active on
typedef struct {void *data;u32 narc,pal;int height;} Render;
typedef struct {int current,index;Render *render[4];void *pic[4];u16 species[4];u8 gender[4],form[4],shiny[4];u32 pid[4];u8 flags[4];} W;
typedef union {BattleParamsR2 params;W display;} Source;
typedef struct {BattleSourceR1 source;} Message;
typedef struct {u8 pad0[0x20];Pokepic *pic;u8 pad24[0x17c];int flag;NARC *narc;} Context;
typedef struct Ov07Work Ov07Work;
typedef struct {BattleSystem *system;Context *context;Ov07Work *object8;Ov07CallbackWork *objectC;void *object10;PokepicTemplate template;u32 gap24;BattleSourceR2 source;u8 reserved64[0x1c];u8 command,battler,type,state,facing,height;u16 species;u32 personality;u8 slot,nature;u16 palette;s8 value90,value91;u8 shiny,value93;u16 value94;u8 value96,form;u32 value98;} Work;
typedef Ov07Params Descriptor;
typedef struct Ov07Input {u32 unk0;u32 fallbackSpecies;Pokemon *mon;} Ov07Input;
extern Pokepic *ov12_022612A4(BattleSystem*,PokepicManager*,PokepicTemplate*,int,int,int,int,int,int,int,int,PokepicAnimScript*,PokepicCallback);
extern int BattleAnimSystem_Execute(u8*);
extern BOOL BattleAnimSystem_IsActive(void*);
extern BOOL BattleAnimSystem_EndMove(void*);
extern void ov12_02261F38(void*,int,int,void*,void*,int,int,int,int);
extern void BattleController_InitMoveAnimData(BattleSystem*,BattleContext*,void*,int,int,int,int,int);
extern void ov07_02234A20(W*,int);
extern void BattleController_EmitCommandComplete(BattleSystem*,int,int);
extern const u8 ov12_0226D120[];
extern const u16 ov12_0226D15A[];
extern Ov07Work *BallSealEffect_New(enum HeapID,const Ov07Input *);
extern void BallSealEffect_LoadResources(Ov07Work *);
extern int BallSealEffect_IsLoaded(u8*);
extern void BallSealEffect_Start(void*);
extern void *BallSealEffect_IsActive(void*);
extern void BallSealEffect_Free(void*);
extern void *BattleAnimBattlerSprites_NewAll(BattleSystem*,enum HeapID);
extern void BattleAnimBattlerSprites_Free(void*);
typedef Ov07Input Arguments;
extern void PlaySE_SetPan(int,int);
void ov12_0225C18C(SysTask*,void*);
void ov12_0225C18C(SysTask *task,void *data) {
 BattleEngineWork *engine;Work *work=data;
 Message message1,message2;Source source;Message message3;Descriptor descriptor;PokepicAnimScript script[10];Arguments args;
 u8 state;engine=(BattleEngineWork *)ov12_0223A8DC(work->system);state=work->state;
 switch(state) {
 case 0:
 descriptor.kind=ov12_0226D120[work->type];descriptor.heapId=(enum HeapID)5;descriptor.unk0C=work->battler;descriptor.unk10=work->palette;
 descriptor.spriteSystem=BattleSystem_GetSpriteSystem(work->system);descriptor.unk20=(int)BattleSystem_GetPaletteData(work->system);descriptor.unk14=1;descriptor.unk18=0;
 if(work->value94==1)descriptor.callbackId=1;else descriptor.callbackId=0;
 work->objectC=UnkBallData_New(&descriptor);
 {PokepicManager *manager=BattleSystem_GetPokepicManager(work->system);
 NARC_ReadPokepicAnimScript(work->context->narc,script,work->species,work->type);
 work->context->pic=ov12_022612A4(work->system,manager,&work->template,ov07_022377DC[work->type][0],ov07_022377F4[work->type][1],ov07_022377F4[work->type][2],work->height,work->value90,work->value91,work->value93,work->battler,script,0);}
 Pokepic_SetAttr(work->context->pic,12,0);Pokepic_SetAttr(work->context->pic,13,0);Pokepic_SetAttr(work->context->pic,44,0);Pokepic_SetAttr(work->context->pic,6,1);
 work->object10=0;work->object10=BattleAnimBattlerSprites_NewAll(work->system,(enum HeapID)5);work->state++;break;
 case 1:memset(&args,0,sizeof(args));args.unk0=work->type;args.mon=BattleSystem_GetPartyMon(work->system,work->battler,work->slot);args.fallbackSpecies=work->palette;
 work->object8=BallSealEffect_New((enum HeapID)5,&args);BallSealEffect_LoadResources(work->object8);work->state++;break;
 case 2:
 if((UnkBallData_GetMode(work->objectC)==0 ||work->value94!=0) &&BallSealEffect_IsLoaded((u8*)work->object8)==1 &&UnkBallData_HasOpened(work->objectC)==1) {
 if(work->object10)BattleAnimBattlerSprites_Free(work->object10);
 Pokepic_StartPaletteFade(work->context->pic,16,16,0,ov12_0226D15A[work->palette]);Pokepic_SetAttr(work->context->pic,6,0);BallSealEffect_Start(work->object8);
 if(work->facing==2)PlaySE_SetPan(1798,117);else PlaySE_SetPan(1798,-117);work->state++;
 }break;
 case 3:if(UnkBallData_IsActive(work->objectC)!=1)work->state++;
 case 4:
 if(Pokepic_GetAttr(work->context->pic,12)==256 &&BallSealEffect_IsActive(work->object8)==0) {
 if(work->facing==2)Pokepic_SetAttr(work->context->pic,45,0);
 ov12_02261F38(work->system,work->battler,work->type,work->context->pic,work->context->narc,work->species,work->form,work->facing,work->personality);
 Pokepic_StartPaletteFade(work->context->pic,16,0,0,ov12_0226D15A[work->palette]);work->state=5;
 }else if(Pokepic_GetAttr(work->context->pic,12)>=256) {
 Pokepic_SetAttr(work->context->pic,12,256);Pokepic_SetAttr(work->context->pic,13,256);
 if(work->facing==2)Pokepic_SetAttr(work->context->pic,45,0);
 ov12_02261F38(work->system,work->battler,work->type,work->context->pic,work->context->narc,work->species,work->form,work->facing,work->personality);
 Pokepic_StartPaletteFade(work->context->pic,16,0,1,ov12_0226D15A[work->palette]);work->state=5;
 }else {Pokepic_AddAttr(work->context->pic,12,32);Pokepic_AddAttr(work->context->pic,13,32);sub_0200914C(work->context->pic,work->value90);}
 break;
 case 5:if(BallSealEffect_IsActive(work->object8)==0)work->state=6;break;
 case 6:
 if(PokepicAnimManager_GetAnimState(ov12_0223B750(work->system),work->battler)==1 &&Pokepic_IsAnimFinished(work->context->pic)==0) {
 UnkBallData_Delete(work->objectC);BallSealEffect_Free(work->object8);
 if(work->shiny){BattleController_InitMoveAnimData(work->system,0,&message1,1,11,work->battler,work->battler,0);ov12_02261B80(work->system,(u8*)work->context,engine,&message1.source);work->state=7;}else work->state=8;
 }break;
 case 7:case 9:case 11:BattleAnimSystem_Execute((u8*)engine);if(BattleAnimSystem_IsActive(engine)==0){BattleAnimSystem_EndMove(engine);work->state++;}break;
 case 8:if(work->value98){BattleController_InitMoveAnimData(work->system,0,&message2,1,15,work->battler,work->battler,0);ov12_02261B80(work->system,(u8*)work->context,engine,&message2.source);work->state++;}else work->state=255;break;
 case 10:ov12_02261CA8(work->system,&work->source,&source.params,work->battler);ov07_02234A20(&source.display,5);
 BattleController_InitMoveAnimData(work->system,0,&message3,1,16,work->battler,work->battler,0);ov12_02261B80(work->system,(u8*)work->context,engine,&message3.source);work->context->flag=1;work->state++;break;
 default:BattleController_EmitCommandComplete(work->system,work->battler,work->command);Heap_Free(work);SysTask_Destroy(task);break;
 }
}

void ov12_0225C6C8(SysTask*,void*);
void ov12_0225C6C8(SysTask *task,void *data) {
 BattleEngineWork *engine;Work *work=data;
 Message message1,message2,message3;Source source;Message message4;PokepicAnimScript script[10];
 u8 state;engine=(BattleEngineWork*)ov12_0223A8DC(work->system);
 state=work->state;switch(state) {
 case 0:{PokepicManager *manager=BattleSystem_GetPokepicManager(work->system);
 NARC_ReadPokepicAnimScript(work->context->narc,script,work->species,work->type);
 work->context->pic=ov12_022612A4(work->system,manager,&work->template,(ov07_022377F4+6)[work->type][0],(ov07_022377F4+6)[work->type][1],ov07_022377F4[work->type][2],work->height,work->value90,work->value91,work->value93,work->battler,script,0);
 Pokepic_SetAttr(work->context->pic,12,256);Pokepic_SetAttr(work->context->pic,13,256);
 Pokepic_SetAttr(work->context->pic,44,0);Pokepic_SetAttr(work->context->pic,6,0);work->state++;}break;
 case 1:BattleController_InitMoveAnimData(work->system,0,&message1,1,41,work->battler,work->battler,0);ov12_02261B80(work->system,(u8*)work->context,engine,&message1.source);work->state++;break;
 case 3:
 if(work->facing==2)Pokepic_SetAttr(work->context->pic,45,0);
 ov12_02261F38(work->system,work->battler,work->type,work->context->pic,work->context->narc,work->species,work->form,work->facing,work->personality);work->state=4;break;
 case 4:
 if(PokepicAnimManager_GetAnimState(ov12_0223B750(work->system),work->battler)==1 && Pokepic_IsAnimFinished(work->context->pic)==0) {
 if(work->shiny) {BattleController_InitMoveAnimData(work->system,0,&message2,1,11,work->battler,work->battler,0);ov12_02261B80(work->system,(u8*)work->context,engine,&message2.source);work->state=5;}
 else work->state=6;
 }break;
 case 2:case 5:case 7:case 9:
 BattleAnimSystem_Execute((u8 *)engine);if(BattleAnimSystem_IsActive(engine)==0){BattleAnimSystem_EndMove(engine);work->state++;}break;
 case 6:
 if(work->value98) {BattleController_InitMoveAnimData(work->system,0,&message3,1,15,work->battler,work->battler,0);ov12_02261B80(work->system,(u8*)work->context,engine,&message3.source);work->state++;}
 else work->state=255;break;
 case 8:
 ov12_02261CA8(work->system,&work->source,&source.params,work->battler);ov07_02234A20(&source.display,5);
 BattleController_InitMoveAnimData(work->system,0,&message4,1,16,work->battler,work->battler,0);ov12_02261B80(work->system,(u8*)work->context,engine,&message4.source);
 work->context->flag=1;work->state++;break;
 default:BattleController_EmitCommandComplete(work->system,work->battler,work->command);Heap_Free(work);SysTask_Destroy(task);break;
 }
}
