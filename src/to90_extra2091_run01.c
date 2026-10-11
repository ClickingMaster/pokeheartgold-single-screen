#pragma force_active on
#include "global.h"
#pragma force_active on
extern void BattleBagMenu_DrawButton(void*,u8,int,int);extern int BattleBagMenu_GetItemOnPage(void*,u32);
void BattleBagMenu_DrawPageButtons(void*,int);
void BattleBagMenu_DrawPageButtons(void*s,int mode){u32 i;switch(mode){case 0:BattleBagMenu_DrawButton(s,0,0,mode);BattleBagMenu_DrawButton(s,1,0,mode);BattleBagMenu_DrawButton(s,2,0,mode);BattleBagMenu_DrawButton(s,3,0,mode);if(*(u16*)((u8*)*(void**)s+0x20)==0)BattleBagMenu_DrawButton(s,4,3,mode);else BattleBagMenu_DrawButton(s,4,0,mode);BattleBagMenu_DrawButton(s,5,0,mode);break;case 1:for(i=0;i<6;i++){if(BattleBagMenu_GetItemOnPage(s,i)==0)BattleBagMenu_DrawButton(s,(u8)(i+6),3,mode);else BattleBagMenu_DrawButton(s,(u8)(i+6),0,mode);}if(*((u8*)s+0x1154+*((u8*)s+0x114d))==0){BattleBagMenu_DrawButton(s,12,3,mode);BattleBagMenu_DrawButton(s,13,3,mode);}else{BattleBagMenu_DrawButton(s,12,0,mode);BattleBagMenu_DrawButton(s,13,0,mode);}BattleBagMenu_DrawButton(s,14,0,mode);break;case 2:BattleBagMenu_DrawButton(s,15,0,mode);BattleBagMenu_DrawButton(s,16,0,mode);break;}}
