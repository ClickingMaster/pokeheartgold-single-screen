#pragma force_active on
#include "global.h"
#include "pokemon.h"
#include "pokepic.h"
#include "heap.h"
#include "sys_task.h"
typedef struct{void*bs,*owner;Pokepic*pic;u8 data[88];u8 who,battler,state,form;u16 species;u8 gender,cryform;u32 personality;u16 message,voice;}W;
extern void*ov12_0223A8DC(void*);extern void BattleController_InitMoveAnimData(void*,int,void*,int,int,int,int,int);extern void ov12_02261B80(void*,void*,void*,void*);extern void ov12_02261CA8(void*,void*,void*,int);extern void BattleAnimUtil_RestorePic(void*,int);
extern void BattleAnimSystem_Execute(void*);extern int BattleAnimSystem_IsActive(void*);extern void BattleAnimSystem_EndMove(void*);extern SOUND_CHATOT*BattleSystem_GetChatotVoice(void*,int);
extern int IsCryFinished(void);extern void PlaySE_SetPan(int,int);extern void BattleController_EmitCommandComplete(void*,int,int);
extern void SysTask_Destroy(SysTask*);
void ov12_022600F0(SysTask*,W*);
void ov12_022600F0(SysTask*task,W*w){u8 msgA[88],effect[80],msgB[88];void*input=ov12_0223A8DC(w->bs);int n;
 switch(w->state){case 0:if(w->message){BattleController_InitMoveAnimData(w->bs,0,msgA,1,15,w->battler,w->battler,0);ov12_02261B80(w->bs,w->owner,input,msgA);w->state++;}else w->state=4;break;
 case 2:ov12_02261CA8(w->bs,w->data,effect,w->battler);BattleAnimUtil_RestorePic(effect,5);BattleController_InitMoveAnimData(w->bs,0,msgB,1,16,w->battler,w->battler,0);ov12_02261B80(w->bs,w->owner,input,msgB);*(u32*)((u8*)w->owner+0x1a0)=0;w->state++;break;
 case 1:case 3:BattleAnimSystem_Execute(input);if(!BattleAnimSystem_IsActive(input)){BattleAnimSystem_EndMove(input);w->state++;}break;
 case 4:n=w->form==2?117:-117;sub_0207204C(BattleSystem_GetChatotVoice(w->bs,w->battler),5,w->species,w->cryform,n,127,w->voice,5,0);w->state++;
 case 5:if(!IsCryFinished())w->state++;break;
 case 6:if(w->form==2)PlaySE_SetPan(1795,117);else PlaySE_SetPan(1795,-117);if(Pokepic_GetAttr(w->pic,41)>0)w->state=7;else w->state=8;break;
 case 7:n=Pokepic_GetAttr(w->pic,41)-8;if(n<0)n=0;Pokepic_SetAttr(w->pic,41,n);if(n)break;w->state++;
 case 8:n=GetMonPicHeightBySpeciesGenderForm(w->species,w->gender,w->form,w->cryform,w->personality);Pokepic_SetVisible(w->pic,0,0,80,80-n);w->state++;break;
 case 9:n=Pokepic_GetAttr(w->pic,1);Pokepic_SetAttr(w->pic,1,n+8);n=Pokepic_GetAttr(w->pic,18)-8;if(n<0)n=0;Pokepic_SetAttr(w->pic,18,n);if(!n){Pokepic_Delete(w->pic);w->state++;}break;
 case 10:BattleController_EmitCommandComplete(w->bs,w->battler,w->who);Heap_Free(w);SysTask_Destroy(task);break;}}
