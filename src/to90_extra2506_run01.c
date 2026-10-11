#pragma force_active on
#include "global.h"
#include "system.h"
#include "sound.h"
#pragma force_active on
extern void PlaySE(int);
typedef struct {u8 pad[6];s16 scroll;u8 pad2;u8 count;u8 tail[2];} Pocket;
typedef struct {u8 pad[4];Pocket pockets[8];u8 selected;} Bag;
typedef struct {u8 pad[0x234];Bag *bag;u8 pad2[0x434];volatile int current;u8 pad3[2];u8 position;} Work;
extern int BagApp_GetMoveCursorFromDpad(int);
extern int BagApp_GetMoveTargetResult(Work *,int),BagApp_GetTouchedButton(Work *,int),BagApp_GetPageItemCount(Work *);
extern void BagApp_SetMoveCursorPosition(Work *,u32);
extern int BagApp_StartButtonFlash(void *,u8,u8,u8,void *);
extern int BagApp_StartButtonAnim(void *,u8,u32,void *);
int BagApp_State_MoveItemInput(Work *);
int BagApp_State_MoveItemInput(Work *w){
u32 result=-1,choice;u16 next;Pocket *p=&w->bag->pockets[w->bag->selected];
next=BagApp_GetMoveCursorFromDpad(w->current);
if(w->current!=next){if((u16)(next+0xfffa)<=1)result=BagApp_GetMoveTargetResult(w,next);else{w->current=next;BagApp_SetMoveCursorPosition(w,w->current);PlaySE(1500);}}
choice=BagApp_GetTouchedButton(w,2);
if(choice!=-1){if(choice==8){result=BagApp_GetMoveTargetResult(w,choice);BagApp_SetMoveCursorPosition(w,choice);}else if(choice-6<=1){result=BagApp_GetMoveTargetResult(w,choice);}else{w->current=choice;if(BagApp_GetPageItemCount(w)>choice)result=BagApp_GetMoveTargetResult(w,choice);else PlaySE(1523);BagApp_SetMoveCursorPosition(w,w->current);}}
else if(gSystem.newKeys&1)result=BagApp_GetMoveTargetResult(w,w->current);else if(gSystem.newKeys&2)result=-2;
switch(result){case -1:break;case -2:PlaySE(2368);p->scroll=(w->position/6)*6;return BagApp_StartButtonFlash(w,19,9,8,(void *)32);
case 14:if(p->count>6){PlaySE(1500);return BagApp_StartButtonFlash(w,17,9,8,(void *)31);}break;
case 15:if(p->count>6){PlaySE(1500);return BagApp_StartButtonFlash(w,18,9,8,(void *)30);}break;
default:PlaySE(1500);if(w->position==p->scroll+w->current)return BagApp_StartButtonAnim(w,20,41,(void *)33);return BagApp_StartButtonAnim(w,20,42,(void *)33);}
return 3;}
