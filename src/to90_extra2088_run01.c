#pragma force_active on
#include "global.h"
#include <string.h>
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
void *NARC_New(int,int),*NARC_AllocAndReadWholeMember(void*,int,int),*PaletteData_GetUnfadedBuf(void*,int),*Heap_Alloc(int,int);
void NARC_Delete(void*),Heap_Free(void*),ov08_022217F0(void*,void*),ov08_02221B1C(void*,void*);

void GfGfxLoader_LoadCharDataFromOpenNarc(void*,int,void*,int,int,int,int,int),PaletteData_LoadNarc(void*,int,int,int,int,int,int),GfGfxLoader_LoadCharData(int,int,void*,int,int,int,int,int),PaletteData_LoadPalette(void*,void*,int,int,int);
int BattleSystem_GetFrame(void*),sub_0200E63C(int),sub_0200E640(int);
void ov08_0221CF38(void*);
void ov08_0221CF38(void*w){void*n,*buf;NNSG2dScreenData *screen;u8*pal,*temp;int frame;
 n=NARC_New(71,W(P(w,0),12));GfGfxLoader_LoadCharDataFromOpenNarc(n,22,P(w,0x1e4),7,0,0,0,W(P(w,0),12));
 buf=NARC_AllocAndReadWholeMember(n,20,W(P(w,0),12));NNS_G2dGetUnpackedScreenData(buf,&screen);ov08_022217F0(w,(u8*)screen+12);Heap_Free(buf);
 buf=NARC_AllocAndReadWholeMember(n,21,W(P(w,0),12));NNS_G2dGetUnpackedScreenData(buf,&screen);ov08_02221B1C(w,(u8*)screen+12);Heap_Free(buf);
 PaletteData_LoadNarc(P(w,0x1e8),71,23,W(P(w,0),12),1,512,0);NARC_Delete(n);
 pal=PaletteData_GetUnfadedBuf(P(w,0x1e8),1);memcpy((u8*)w+0x1f60,pal+0x180,64);
 PaletteData_LoadNarc(P(w,0x1e8),16,7,W(P(w,0),12),1,32,208);PaletteData_LoadNarc(P(w,0x1e8),16,8,W(P(w,0),12),1,32,240);
 frame=BattleSystem_GetFrame(P(P(w,0),8));GfGfxLoader_LoadCharData(38,sub_0200E63C(frame),P(w,0x1e4),4,1,0,0,W(P(w,0),12));PaletteData_LoadNarc(P(w,0x1e8),38,sub_0200E640(frame),W(P(w,0),12),1,32,224);
 pal=PaletteData_GetUnfadedBuf(P(w,0x1e8),1);temp=Heap_Alloc(W(P(w,0),12),32);memcpy(temp,pal+0x1a0,32);memcpy(temp+14,pal+0x134,4);memcpy(temp+6,pal+0x138,4);PaletteData_LoadPalette(P(w,0x1e8),temp,1,208,32);Heap_Free(temp);
}
