#pragma force_active on
#include "global.h"
#pragma force_active on
extern void*NewString_ReadMsgData(void*,int);extern void BagApp_BufferSlotItemNamePlural(void*,int,int),BagApp_BufferSlotItemName(void*,int,int),BufferIntegerAsString(void*,int,int,int,int,int),StringExpandPlaceholders(void*,void*,void*),String_Delete(void*);extern int BagApp_PrintMessage(void*,int);
void BagApp_PrintTrashConfirmMessage(void*);
void BagApp_PrintTrashConfirmMessage(void*s){void*str=NewString_ReadMsgData(*(void**)((u8*)s+0x2f0),55);u8*menu=*(u8**)((u8*)s+0x234);u8*entry=menu+4+menu[0x64]*12;if(*(s16*)((u8*)s+0x680)>1)BagApp_BufferSlotItemNamePlural(s,*(s16*)(entry+6)+*(int*)((u8*)s+0x644)-8,0);else BagApp_BufferSlotItemName(s,*(s16*)(entry+6)+*(int*)((u8*)s+0x644)-8,0);BufferIntegerAsString(*(void**)((u8*)s+0x2f4),1,*(s16*)((u8*)s+0x680),3,0,1);StringExpandPlaceholders(*(void**)((u8*)s+0x2f4),*(void**)((u8*)s+0x5e4),str);String_Delete(str);*((u8*)s+0x616)=(u8)BagApp_PrintMessage(s,0);}
