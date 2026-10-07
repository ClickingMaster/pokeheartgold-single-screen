#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void*NARC_New(int,int);void NARC_Delete(void*);void SpriteSystem_LoadPlttResObjFromOpenNarc(void*,void*,void*,int,int,int,int,int);void SpriteSystem_LoadCharResObjFromOpenNarc(void*,void*,void*,int,int,int,int);void SpriteSystem_LoadCellResObjFromOpenNarc(void*,void*,void*,int,int,int);void SpriteSystem_LoadAnimResObjFromOpenNarc(void*,void*,void*,int,int,int);void ov95_021E7388(void*,int,int,int,int);void PaletteData_LoadNarc(void*,int,int,int,int,int,int);void GfGfxLoader_LoadCharDataFromOpenNarc(void*,int,void*,int,int,int,int,int);void GfGfxLoader_LoadScrnDataFromOpenNarc(void*,int,void*,int,int,int,int,int);void ToggleBgLayer(int,int);void GfGfx_EngineATogglePlanes(int,int);
void ov95_021E70BC(u8*w,void*palette,int charId,int pltId,int cellId,int animId);
void ov95_021E70BC(u8*w,void*palette,int charId,int pltId,int cellId,int animId){void*system=P(w,4);void*manager=P(w,8);void*narc;GF_ASSERT(w);GF_ASSERT(system);GF_ASSERT(manager);
 narc=NARC_New(239,U(w,0));SpriteSystem_LoadPlttResObjFromOpenNarc(system,manager,narc,11,0,1,2,pltId);SpriteSystem_LoadCharResObjFromOpenNarc(system,manager,narc,12,0,2,charId);SpriteSystem_LoadCellResObjFromOpenNarc(system,manager,narc,13,0,cellId);SpriteSystem_LoadAnimResObjFromOpenNarc(system,manager,narc,14,0,animId);ov95_021E7388(w,charId,pltId,cellId,animId);
 PaletteData_LoadNarc(palette,239,0,70,1,160,0);PaletteData_LoadNarc(palette,16,9,70,1,32,240);
 GfGfxLoader_LoadCharDataFromOpenNarc(narc,1,P(w,12),4,0,0,0,70);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,9,P(w,12),4,0,0,0,70);GfGfxLoader_LoadCharDataFromOpenNarc(narc,1,P(w,12),5,0,0,0,70);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,10,P(w,12),5,0,0,0,70);ToggleBgLayer(5,0);ToggleBgLayer(6,0);GfGfx_EngineATogglePlanes(16,1);NARC_Delete(narc);
}
