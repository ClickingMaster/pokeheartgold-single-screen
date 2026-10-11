#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
extern const u8 sBattleBagItemListHitboxes[];extern u8 gSystem[];
extern u32 BattleBagMenu_CheckTouchHitbox(void*,const void*);
extern int BattleDpadCursor_IsActive(void*),BattleDpadCursor_GetPosition(void*),BattleDpadCursor_HandleInput(void*),BattleBagMenu_GetItemOnPage(void*,int);
extern void BattleBagMenu_DeactivateCursor(void*),BattleBagMenu_StartButtonAnim(void*,u8,int),PlaySE(int);
int BattleBagMenu_HandleItemListInput(void*);
#pragma force_active on
int BattleBagMenu_HandleItemListInput(void *w){u32 selection=BattleBagMenu_CheckTouchHitbox(w,sBattleBagItemListHitboxes);
 if(selection==~0u){int moved=BattleDpadCursor_IsActive(P(w,0x34));int old=BattleDpadCursor_GetPosition(P(w,0x34));selection=BattleDpadCursor_HandleInput(P(w,0x34));int current=BattleDpadCursor_GetPosition(P(w,0x34));if(selection==~1u)selection=6;else if(moved==1&&old==current){u32 buttons=U(gSystem,0x4c);if((buttons&32)&&(current==0||current==2||current==4))selection=7;if((buttons&16)&&(current==1||current==3||current==5))selection=8;}}
 else BattleBagMenu_DeactivateCursor(w);
 switch(selection){case 0:case 1:case 2:case 3:case 4:case 5:if(BattleBagMenu_GetItemOnPage(w,selection)){PlaySE(1501);B((u8*)P(w,0)+B(w,0x114d),0x27)=selection;B(w,0x114b)=6;BattleBagMenu_StartButtonAnim(w,(u8)(selection+6),0);return 11;}break;
 case 6:PlaySE(1501);B(w,0x114b)=4;BattleBagMenu_StartButtonAnim(w,14,0);return 11;
 case 7:if(B((u8*)w+B(w,0x114d),0x1154)){PlaySE(1501);B(w,0x114b)=7;C(w,0x114e)=-1;BattleBagMenu_StartButtonAnim(w,12,0);return 11;}break;
 case 8:if(B((u8*)w+B(w,0x114d),0x1154)){PlaySE(1501);B(w,0x114b)=7;B(w,0x114e)=1;BattleBagMenu_StartButtonAnim(w,13,0);return 11;}break;
 }return 2;
}
