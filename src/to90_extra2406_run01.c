#pragma force_active on
#include "global.h"
#pragma force_active on
#include "system.h"
typedef struct W31686 {u8 a[0x66c];u32 choice;u8 b[0x180];u32 available[5];} W31686;
extern u8 sItemMenuNavTable[],ov15_02200529[],ov15_0220052A[],ov15_0220052B[],sItemMenuCursorButtons[];
extern void BagApp_SetCursorPosition(W31686 *,u32),PlaySE(int),BagApp_SetMenuInputState(W31686 *,int);
extern int BagApp_GetTouchedButton(W31686 *,int);
int BagApp_HandleItemMenuInput(W31686 *);
int BagApp_HandleItemMenuInput(W31686 *p){u32 old=p->choice,next=old;int selected;if(gSystem.newKeys&64)next=sItemMenuNavTable[old*4];else if(gSystem.newKeys&128)next=ov15_02200529[old*4];else if(gSystem.newKeys&32)next=ov15_0220052A[old*4];else if(gSystem.newKeys&16)next=ov15_0220052B[old*4];if(next!=old){p->choice=next;BagApp_SetCursorPosition(p,sItemMenuCursorButtons[next]);PlaySE(1500);return -1;}selected=BagApp_GetTouchedButton(p,1);if(selected!=-1){BagApp_SetMenuInputState(p,1);p->choice=selected;BagApp_SetCursorPosition(p,sItemMenuCursorButtons[selected]);if(selected==4){PlaySE(2368);return -2;}if(p->available[selected]){PlaySE(1500);return selected;}}else if(gSystem.newKeys&1){if(p->available[next]){PlaySE(1500);BagApp_SetMenuInputState(p,0);return next;}if(next==4){PlaySE(1500);return -2;}}else if(gSystem.newKeys&2){PlaySE(2368);return -2;}return -1;}
