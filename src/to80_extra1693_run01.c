#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define S(p,o) (*(s16*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
extern struct{u8 pad[0x48];u32 keys;} gSystem;
extern int BagApp_GetTouchedButton(void*,int),BagApp_GetUnusedDigitCount(u16),sub_020881C0(s16*,u16),BagApp_StartButtonFlash(void*,int,int,int,int);extern s16 BagApp_StepQuantity(s16,u16,int);extern void ManagedSprite_SetAnimationFrame(void*,int),ManagedSprite_SetAnim(void*,int),BagApp_PrintQuantityDigits(void*,int),PlaySE(u16);
int BagApp_State_TrashQuantityInput(u8*);
int BagApp_State_TrashQuantityInput(u8*w){int result=0;int action=BagApp_GetTouchedButton(w,3);if(action!=-1){switch(BagApp_GetUnusedDigitCount(H(w,0x682))){case 1:if(action==0||action==3)action=-1;break;case 2:if(action==0||action==1||action==3||action==4)action=-1;break;}switch(action){
case 0:S(w,0x680)=BagApp_StepQuantity(S(w,0x680),H(w,0x682),100);ManagedSprite_SetAnimationFrame(P(w,720),0);ManagedSprite_SetAnim(P(w,720),26);result=1;break;
case 1:S(w,0x680)=BagApp_StepQuantity(S(w,0x680),H(w,0x682),10);ManagedSprite_SetAnimationFrame(P(w,724),0);ManagedSprite_SetAnim(P(w,724),26);result=1;break;
case 2:S(w,0x680)=BagApp_StepQuantity(S(w,0x680),H(w,0x682),1);ManagedSprite_SetAnimationFrame(P(w,728),0);ManagedSprite_SetAnim(P(w,728),26);result=1;break;
case 3:S(w,0x680)=BagApp_StepQuantity(S(w,0x680),H(w,0x682),-100);ManagedSprite_SetAnimationFrame(P(w,732),0);ManagedSprite_SetAnim(P(w,732),28);result=2;break;
case 4:S(w,0x680)=BagApp_StepQuantity(S(w,0x680),H(w,0x682),-10);ManagedSprite_SetAnimationFrame(P(w,736),0);ManagedSprite_SetAnim(P(w,736),28);result=2;break;
case 5:S(w,0x680)=BagApp_StepQuantity(S(w,0x680),H(w,0x682),-1);ManagedSprite_SetAnimationFrame(P(w,740),0);ManagedSprite_SetAnim(P(w,740),28);result=2;break;
case 6:result=3;break;case 7:result=4;break;}}else{result=sub_020881C0((s16*)(w+0x680),H(w,0x682));if(!result){if(gSystem.keys&1)result=3;else if(gSystem.keys&2)result=4;}}switch(result){case 0:break;case 1:case 2:BagApp_PrintQuantityDigits(w,3);PlaySE(0x637);return 5;case 3:PlaySE(1500);return BagApp_StartButtonFlash(w,38,9,8,6);case 4:PlaySE(2368);return BagApp_StartButtonFlash(w,19,9,8,7);}return 5;}
