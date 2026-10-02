#pragma force_active on
#include "global.h"
#pragma force_active on
void PaletteData_LoadNarc(void *,int,int,int,int,int,int);
void PaletteData_LoadFromNarc(void *,int,int,int,int,int,int,int);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *,int,void *,int,int,int,int,int);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *,int,void *,int,int,int,int,int);
void ov93_02260BF0(void *),ov93_02260BB0(void *);
void LoadUserFrameGfx1(void *,int,int,int,int,int);
void PaletteData_LoadPaletteSlotFromHardware(void *,int,int,int);
void PaletteData_FillPaletteInBuffer(void *,int,int,int,int,int);
void ov93_0225DBC8(u8 *,void *);
#define P(o) (*(void **)(w+(o)))
void ov93_0225DBC8(u8 *w,void *narc){
 PaletteData_LoadNarc(P(140),201,5,117,1,160,0);
 PaletteData_LoadFromNarc(P(140),201,6,117,1,320,96,96);
 GfGfxLoader_LoadCharDataFromOpenNarc(narc,13,P(44),6,0,0,0,117);
 GfGfxLoader_LoadScrnDataFromOpenNarc(narc,14,P(44),6,0,0,0,117);
 GfGfxLoader_LoadCharDataFromOpenNarc(narc,18,P(44),5,0,0,0,117);
 switch(((u8 *)P(0))[48]){
 case 2:GfGfxLoader_LoadScrnDataFromOpenNarc(narc,15,P(44),5,0,0,0,117);break;
 case 3:GfGfxLoader_LoadScrnDataFromOpenNarc(narc,16,P(44),5,0,0,0,117);break;
 default:GfGfxLoader_LoadScrnDataFromOpenNarc(narc,17,P(44),5,0,0,0,117);break;
 }
 GfGfxLoader_LoadCharDataFromOpenNarc(narc,18,P(44),4,0,0,0,117);
 GfGfxLoader_LoadScrnDataFromOpenNarc(narc,19,P(44),4,0,0,0,117);
 ov93_02260BF0(w);ov93_02260BB0(w);
 LoadUserFrameGfx1(P(44),4,848,6,0,117);
 PaletteData_LoadPaletteSlotFromHardware(P(140),1,96,32);
 PaletteData_FillPaletteInBuffer(P(140),1,2,0,0,1);
}
