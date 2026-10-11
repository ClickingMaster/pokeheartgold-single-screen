#pragma force_active on
#include "global.h"
#pragma force_active on
#pragma opt_common_subs off
#define W(p,n) (*(u32 *)((u8 *)(p)+(n)))
typedef struct{u16 id,other;}Item;
extern void FillWindowPixelBuffer(void *,u8),CopyWindowPixelsToVram_TextMode(void *);
extern void PokedexApp_DrawSearchOrderLabel(void *,int),PokedexApp_DrawSearchNameLabel(void *,int),PokedexApp_DrawSearchTypeLabel(void *,int),PokedexApp_DrawSearchHeightLabel(void *,int),PokedexApp_DrawSearchWeightLabel(void *,int),PokedexApp_DrawSearchAreaLabel(void *,int),PokedexApp_DrawSearchFormLabel(void *,int),PokedexApp_DrawSearchOrderValue(void *,int),PokedexApp_DrawSearchNameValue(void *,int),PokedexApp_DrawSearchTypeValue(void *,int,int,int),PokedexApp_DrawHeightValue(void *,u16,int),PokedexApp_DrawWeightValue(void *,u16,int),PokedexApp_DrawSearchAreaValue(void *,int);
void PokedexApp_DrawSearchCriteriaSummary(u8 *);
void PokedexApp_DrawSearchCriteriaSummary(u8 *p){u32 i;FillWindowPixelBuffer(p+28,0);FillWindowPixelBuffer(p+44,0);FillWindowPixelBuffer(p+60,0);FillWindowPixelBuffer(p+76,0);FillWindowPixelBuffer(p+92,0);FillWindowPixelBuffer(p+108,0);FillWindowPixelBuffer(p+124,0);PokedexApp_DrawSearchOrderLabel(p,1);PokedexApp_DrawSearchNameLabel(p,2);PokedexApp_DrawSearchTypeLabel(p,3);PokedexApp_DrawSearchHeightLabel(p,4);PokedexApp_DrawSearchWeightLabel(p,5);PokedexApp_DrawSearchAreaLabel(p,6);PokedexApp_DrawSearchFormLabel(p,7);PokedexApp_DrawSearchOrderValue(p,8);PokedexApp_DrawSearchNameValue(p,9);PokedexApp_DrawSearchTypeValue(p,W(p,0x1870),10,29);PokedexApp_DrawSearchTypeValue(p,W(p,0x1874),11,29);PokedexApp_DrawHeightValue(p,((Item *)W(p,0x1850))[W(p,0x1878)].id,12);PokedexApp_DrawHeightValue(p,((Item *)W(p,0x1850))[W(p,0x187c)].id,13);PokedexApp_DrawWeightValue(p,((Item *)W(p,0x1850))[W(p,0x1880)].other,14);PokedexApp_DrawWeightValue(p,((Item *)W(p,0x1850))[W(p,0x1884)].other,15);PokedexApp_DrawSearchAreaValue(p,16);i=1;p+=28;for(;i<=16;i++,p+=16)CopyWindowPixelsToVram_TextMode(p);}
