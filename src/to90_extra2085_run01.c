#pragma force_active on
#include "global.h"
#pragma force_active on
extern u32 PaletteData_GetSelectedBuffersBitmask(void*);
extern int ov12_0226BD50(void*),BattleBagMenu_TryUseItem(void*),BattleBagMenu_GetItemOnPage(void*,u8);
extern void BattleBagMenu_StartButtonAnim(void*,int,int),BattleBagMenu_ShowItemListPage(void*),BattleBagMenu_ShowUseItemPage(void*),PlaySE(int);
int BattleBagMenu_RunTutorial(u8*);
#define PTR(p,o) (*(void**)((u8*)(p)+(o)))
int BattleBagMenu_RunTutorial(u8 *s){if(PaletteData_GetSelectedBuffersBitmask(PTR(s,8)))return 12;switch(s[0x1159]){case 0:if(ov12_0226BD50(PTR(s,0x38))==1){PlaySE(1501);s[0x114d]=2;s[0x114b]=12;BattleBagMenu_StartButtonAnim(s,2,0);s[0x115a]=0;s[0x1159]++;return 11;}s[0x115a]++;break;case 1:BattleBagMenu_ShowItemListPage(s);s[0x1159]++;break;case 2:if(ov12_0226BD50(PTR(s,0x38))==1){PlaySE(1501);*((u8*)PTR(s,0)+s[0x114d]+0x27)=0;s[0x114b]=12;BattleBagMenu_StartButtonAnim(s,6,0);s[0x115a]=0;s[0x1159]++;return 11;}s[0x115a]++;break;case 3:BattleBagMenu_ShowUseItemPage(s);s[0x1159]++;break;case 4:if(ov12_0226BD50(PTR(s,0x38))==1){PlaySE(1501);*(u16*)((u8*)PTR(s,0)+0x1c)=BattleBagMenu_GetItemOnPage(s,*((u8*)PTR(s,0)+s[0x114d]+0x27));((u8*)PTR(s,0))[0x1e]=s[0x114d];BattleBagMenu_StartButtonAnim(s,15,0);return BattleBagMenu_TryUseItem(s);}s[0x115a]++;break;}return 12;}
