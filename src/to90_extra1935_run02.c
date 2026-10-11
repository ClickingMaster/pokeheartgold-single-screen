#pragma force_active on
#include "global.h"
#include "gf_gfx_loader.h"
#pragma force_active on
typedef struct {BgConfig*bg;u8 pad[16];enum HeapID heap;void*chars;NNSG2dCharacterData*unpacked;u8 gap[0x350];void*last;} Work;
extern void PokeathlonPennantCaptureDisplay_LoadBgGfx(void*);
void ov96_02205048(Work*);
void ov96_02205048(Work*w){GfGfxLoader_LoadCharData(NARC_a_2_2_4,1,w->bg,(GFBgLayer)0,0,0,FALSE,w->heap);w->chars=GfGfxLoader_LoadFromNarc(NARC_a_2_2_4,3,FALSE,w->heap,FALSE);NNS_G2dGetUnpackedBGCharacterData(w->chars,&w->unpacked);GfGfxLoader_LoadScrnData(NARC_a_2_2_4,2,w->bg,(GFBgLayer)0,0,0,FALSE,w->heap);GfGfxLoader_GXLoadPal(NARC_a_2_2_4,0,GF_PAL_LOCATION_MAIN_BG,GF_PAL_SLOT_0_OFFSET,64,w->heap);PokeathlonPennantCaptureDisplay_LoadBgGfx(w->last);}
