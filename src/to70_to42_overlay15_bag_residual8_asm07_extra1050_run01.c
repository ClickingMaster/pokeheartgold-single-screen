#pragma force_active on
#include "global.h"
extern int BagApp_HandleItemMenuInput(void *),BagApp_StartButtonFlash(void *,u8,int,int,int);
int BagApp_State_ItemMenuInput(void *);
int BagApp_State_ItemMenuInput(void *w){int result=BagApp_HandleItemMenuInput(w);switch(result){case -2:return BagApp_StartButtonFlash(w,19,9,8,28);default:*(int*)((u8*)w+0x948)=result;return BagApp_StartButtonFlash(w,(u8)(result+28),9,8,29);case -1:return 4;}}
