#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct {u32 words[4];} Template;
typedef struct {u16 narc,charId,palette;u8 pad[10];} SpriteInfo;
extern const Template ov18_021FA328;
void *Heap_AllocAtEnd(int,int),*Sprite_GetImageProxy(void*),*Sprite_GetPaletteProxy(void*);
u32 Pokedex_GetSeenSpindaPersonality(void*,int);
void GetMonSpriteCharAndPlttNarcIdsEx(SpriteInfo*,int,int,u8,int,int,u32),sub_02014510(u16,u16,int,Template*,void*,u32,int,u8,int),GfGfxLoader_GXLoadPal(u16,u16,int,u32,int,int),PaletteData_LoadPaletteSlotFromHardware(void*,int,u16,int),PaletteData_LoadNarc(void*,u16,u16,int,int,int,u16),PaletteData_FillPaletteInBuffer(void*,int,int,int,u16,u16),Heap_Free(void*);
void PokedexApp_LoadMonPicToSprite(void*,int,u8,u8,u8,int,int);
void PokedexApp_LoadMonPicToSprite(void*w,int species,u8 arg,u8 shiny,u8 face,int slot,int mode){SpriteInfo info;Template t=ov18_021FA328;void*data=Heap_AllocAtEnd(37,3200);u32 personality,loc,pal;void**sprites;int index;
 MIi_CpuClearFast(0,(u32*)&info,16);if(species==327 && face==2)personality=Pokedex_GetSeenSpindaPersonality(P(P(w,0),0),0);else personality=0;
 GetMonSpriteCharAndPlttNarcIdsEx(&info,species,shiny,face,0,arg,personality);sub_02014510(info.narc,info.charId,37,&t,data,personality,0,face,species);
 sprites=(void**)((u8*)w+0x670);index=slot;loc=NNS_G2dGetImageLocation(Sprite_GetImageProxy(P(P(sprites,index*4),0)),(NNS_G2D_VRAM_TYPE)2);DC_FlushRange(data,3200);GXS_LoadOBJ(data,loc,3200);
 pal=NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(P(P(sprites,index*4),0)),(NNS_G2D_VRAM_TYPE)2);
 if(mode==0){GfGfxLoader_GXLoadPal(info.narc,info.palette,5,pal,32,37);PaletteData_LoadPaletteSlotFromHardware(P(w,0x850),3,(u16)(pal>>1),32);}else if(mode==1)PaletteData_LoadNarc(P(w,0x850),info.narc,info.palette,37,3,32,(u16)(pal>>1));else PaletteData_FillPaletteInBuffer(P(w,0x850),3,2,0,(u16)(pal>>1),(u16)((pal>>1)+16));Heap_Free(data);
}
