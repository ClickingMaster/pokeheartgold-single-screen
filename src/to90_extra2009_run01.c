#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,n) (*(u32 *)((u8 *)(p)+(n)))
#define H(p,n) (*(u16 *)((u8 *)(p)+(n)))
extern u8 sAreaWindowTemplates[];
extern void PokedexApp_AddWindows(void *,void *,int),FillWindowPixelBuffer(void *,u8),ScheduleWindowCopyToVram(void *),CopyWindowPixelsToVram_TextMode(void *),String_Delete(void *),PokedexApp_DrawTimeOfDayLabel(void *),PokedexApp_DrawAreaList(void *);
extern void PokedexApp_PrintMessage(void *,void *,int,int,int,int,u32,int),PokedexApp_PrintString(void *,void *,int,int,int,u32,int);
extern void *ov18_021E590C(u16,int,int);
void PokedexApp_SetupAreaWindows(u8 *);
void PokedexApp_SetupAreaWindows(u8 *p){void *s;PokedexApp_AddWindows(p,sAreaWindowTemplates,14);FillWindowPixelBuffer(p+12,0);FillWindowPixelBuffer(p+44,0);FillWindowPixelBuffer(p+76,0);PokedexApp_PrintMessage(p+12,(void *)W(p,0x65c),0x8e,0,0,4,0x20100,0);s=ov18_021E590C(H(p,0x18a2),2,37);PokedexApp_PrintString(p+44,s,0x24,0,0,0x20100,2);String_Delete(s);PokedexApp_PrintMessage(p+76,(void *)W(p,0x65c),0x84,24,0,0,0x50900,2);ScheduleWindowCopyToVram(p+12);ScheduleWindowCopyToVram(p+44);ScheduleWindowCopyToVram(p+76);if(W(p,0x1860)==1){FillWindowPixelBuffer(p+92,0);FillWindowPixelBuffer(p+124,0);PokedexApp_PrintMessage(p+92,(void *)W(p,0x65c),0x41,28,0,4,0xf0c00,2);PokedexApp_PrintMessage(p+124,(void *)W(p,0x65c),0x42,28,0,4,0xf0c00,2);ScheduleWindowCopyToVram(p+92);ScheduleWindowCopyToVram(p+124);}else{FillWindowPixelBuffer(p+108,0);PokedexApp_PrintMessage(p+108,(void *)W(p,0x65c),0x41,28,0,0,0x50900,2);ScheduleWindowCopyToVram(p+108);}FillWindowPixelBuffer(p+60,0);PokedexApp_PrintMessage(p+60,(void *)W(p,0x65c),0x80,56,0,0,0x20100,2);CopyWindowPixelsToVram_TextMode(p+60);PokedexApp_DrawTimeOfDayLabel(p);PokedexApp_DrawAreaList(p);}
