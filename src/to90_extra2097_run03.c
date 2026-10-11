#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define P(p,n) (*(void**)((u8*)(p)+(n)))
void FillWindowPixelBuffer(void*,u8);
u32 GetMoveAttr(u16,int);
extern u32 sBattlePartyMoveNameMsgIds[];
void BattlePartyMenu_PrintMonName(void*,int,int,int,int,int);
void BattlePartyMenu_PrintPPLabel(void*,int,int,int);
void BattlePartyMenu_PrintAccuracyLabel(void*,int);void BattlePartyMenu_PrintPowerLabel(void*,int);void BattlePartyMenu_PrintCategoryLabel(void*,int);void BattlePartyMenu_PrintForgetButtonLabel(void*,int);
void BattlePartyMenu_PrintMoveName(void*,int,int,u32,int,int,u32);
void BattlePartyMenu_PrintMoveAccuracy(void*,int,int);void BattlePartyMenu_PrintMovePower(void*,int,int);void BattlePartyMenu_PrintMoveDescription(void*,int,int);void BattlePartyMenu_PrintMoveCategory(void*,int,int);void BattlePartyMenu_PrintMovePP(void*,int,int,int);
void BattlePartyMenu_PrintForgetMovePage(void*);
void BattlePartyMenu_PrintForgetMovePage(void *p){u8 *row,*entry;u32 index;int attr;
FillWindowPixelBuffer((u8*)P(p,0x2070)+0,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+32,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+48,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+16,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+64,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+96,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+80,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+112,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+144,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+160,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+128,0);
FillWindowPixelBuffer((u8*)P(p,0x2070)+176,0);
BattlePartyMenu_PrintMonName(p,0,0,B(P(p,0),0x11),0,0);BattlePartyMenu_PrintPPLabel(p,2,0,0);BattlePartyMenu_PrintAccuracyLabel(p,4);BattlePartyMenu_PrintPowerLabel(p,5);BattlePartyMenu_PrintCategoryLabel(p,9);index=B(P(p,0),0x34);if(index<4){row=(u8*)p+0x34+B(P(p,0),0x11)*80;entry=row+index*8;BattlePartyMenu_PrintMoveName(p,H(entry,0),1,sBattlePartyMoveNameMsgIds[index],0,0,0xf0e00);BattlePartyMenu_PrintMoveAccuracy(p,6,B(entry,6));BattlePartyMenu_PrintMovePower(p,7,B(entry,7));BattlePartyMenu_PrintMoveDescription(p,8,H(entry,0));BattlePartyMenu_PrintMoveCategory(p,10,B(entry,5));BattlePartyMenu_PrintMovePP(p,3,B(entry,2),B(entry,3));}else{attr=GetMoveAttr(H(P(p,0),0x24),5);BattlePartyMenu_PrintMoveName(p,H(P(p,0),0x24),1,0x49,0,0,0xf0e00);BattlePartyMenu_PrintMoveDescription(p,8,H(P(p,0),0x24));BattlePartyMenu_PrintMoveAccuracy(p,6,GetMoveAttr(H(P(p,0),0x24),4));BattlePartyMenu_PrintMovePower(p,7,GetMoveAttr(H(P(p,0),0x24),2));BattlePartyMenu_PrintMoveCategory(p,10,GetMoveAttr(H(P(p,0),0x24),1));BattlePartyMenu_PrintMovePP(p,3,attr,attr);}BattlePartyMenu_PrintForgetButtonLabel(p,11);}
