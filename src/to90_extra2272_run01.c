#pragma force_active on
#include "global.h"
extern void BattleBagMenu_PrintItemName(void *,int,int,int,int,int),BattleBagMenu_PrintItemQuantity(void *,int,int,int,int,int,int);
void BattleBagMenu_PrintItemListEntry(void *,int);
#pragma force_active on
void BattleBagMenu_PrintItemListEntry(void *argument,int index){
 u8 *work=argument;int base=(*(u8 **)work+work[0x114d])[0x2c]*6;int shift;if(work[0x31]==0)shift=0;else shift=12;
 BattleBagMenu_PrintItemName(work,index+base,index,shift+index*2,4,0x30201);BattleBagMenu_PrintItemQuantity(work,index+base,index,shift+1+index*2,0,4,0x10200);
}
