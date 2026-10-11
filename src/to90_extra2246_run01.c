#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define SW(p,n) (*(s32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
void FillWindowPixelBuffer(void*,u32);void*ov18_021E590C(void*,u32,u32);int Pokedex_GetInternationalViewFlag(void*);void PokedexApp_PrintString(void*,void*,int,int,int,int,int);void String_Delete(void*);void ScheduleWindowCopyToVram(void*);void ClearWindowTilemapAndScheduleTransfer(void*);void PokedexApp_DrawCenteredSpeciesName(void*,void*,int);
void PokedexApp_DrawCenteredSpeciesName(void*s,void*input,int i){if(input){u8*win=(u8*)s+12;void*str;FillWindowPixelBuffer(win+i*16,0);str=ov18_021E590C(input,B(s,0x185c),0x25);if(Pokedex_GetInternationalViewFlag(P(P(s,0),0))==1)PokedexApp_PrintString(win+i*16,str,0x38,0,0,0x20100,2);else PokedexApp_PrintString(win+i*16,str,0x2c,0,0,0x20100,2);String_Delete(str);ScheduleWindowCopyToVram(win+i*16);}else ClearWindowTilemapAndScheduleTransfer((u8*)s+12+i*16);}
