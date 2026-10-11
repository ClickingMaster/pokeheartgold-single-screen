#pragma force_active on
#include "global.h"
#pragma force_active on
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void*NewString_ReadMsgData(void*,u32);u8 AddTextPrinterParameterizedWithColor(void*,u32,void*,u32,u32,u32,u32,void*);void String_Delete(void*),ScheduleWindowCopyToVram(void*),BattlePartyMenu_PrintPPLabel(u8*,int,u16,u32);
void BattlePartyMenu_PrintPPLabel(u8*d,int index,u16 x,u32 y){void*text=NewString_ReadMsgData(P(d,0x1fa8),14);AddTextPrinterParameterizedWithColor((u8*)P(d,0x2070)+index*16,0,text,x,y,255,0xf0e00,0);String_Delete(text);ScheduleWindowCopyToVram((u8*)P(d,0x2070)+index*16);}
