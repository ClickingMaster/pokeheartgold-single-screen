#pragma force_active on
#include "global.h"
extern void GfGfxLoader_LoadCharData(int,int,void *,int,int,int,int,int);extern void GfGfxLoader_LoadScrnData(int,int,void *,int,int,int,int,int);extern void PaletteData_LoadNarc(void *,int,int,int,int,int,int);
void ov95_021E5D44(void *,void *);
void ov95_021E5D44(void *bg,void *palette){GfGfxLoader_LoadCharData(115,0,bg,3,0,0,1,70);GfGfxLoader_LoadScrnData(115,1,bg,3,0,0,1,70);PaletteData_LoadNarc(palette,115,8,70,0,64,0);}
