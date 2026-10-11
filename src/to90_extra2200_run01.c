#pragma force_active on
#include "global.h"
#pragma force_active on
extern void FillWindowPixelBuffer(void*,int),CopyWindowPixelsToVram_TextMode(void*);
extern u8 GetWindowX(void*),GetWindowY(void*);
extern void *PokedexApp_GetAreaListEntryMsg(void*,int);
extern void PokedexApp_PrintFormattedMessage(void*,void*,int,void*,int,int,int,int,int);
extern void BgCellManager_DrawCellAt(void*,int,s8,s8);
void PokedexApp_DrawAreaList(void*w);
typedef struct {u8 pad[0x18ca];s8 cursor;u8 gap[0x35];int count;} State;
void PokedexApp_DrawAreaList(void*w){u32 i=0;u8*windows=(u8*)w;windows+=12;for(;i<6;i++){u32 offset=(i+8)*16;int index,x,y;FillWindowPixelBuffer(windows+offset,0);index=((State*)w)->cursor+i-2;if(index>=0&&index<((State*)w)->count)PokedexApp_PrintFormattedMessage(w,*(void**)((u8*)w+0x65c),i+8,PokedexApp_GetAreaListEntryMsg(w,index),72,0,4,0xf0c00,2);CopyWindowPixelsToVram_TextMode(windows+offset);x=GetWindowX(windows+offset);y=GetWindowY(windows+offset);BgCellManager_DrawCellAt(*(void**)((u8*)w+8),i+17,x,y);}}
