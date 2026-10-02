#pragma force_active on
#include "global.h"
extern void PaletteData_LoadNarc(void *,int,int,int,int,int,int),PaletteData_FillPaletteInBuffer(void *,int,int,int,int,int);
extern void GfGfxLoader_LoadCharDataFromOpenNarc(void *,int,void *,int,int,int,int,int),GfGfxLoader_LoadScrnDataFromOpenNarc(void *,int,void *,int,int,int,int,int);
typedef struct {u8 pad[0x2c];void *bg;u8 pad2[0x5c];void *palette;} Work;
void ov93_0225DB2C(Work *,void *);
void ov93_0225DB2C(Work *w,void *narc){PaletteData_LoadNarc(w->palette,201,4,117,0,0,0);GfGfxLoader_LoadCharDataFromOpenNarc(narc,2,w->bg,3,0,0,0,117);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,3,w->bg,3,0,0,0,117);GfGfxLoader_LoadCharDataFromOpenNarc(narc,0,w->bg,2,0,0,0,117);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,1,w->bg,2,0,0,0,117);PaletteData_FillPaletteInBuffer(w->palette,0,2,0,0,1);}
