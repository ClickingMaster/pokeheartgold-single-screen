#include "global.h"
#include "pokemon.h"
#include "unk_02013FDC.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "trade_anim.h"
#pragma force_active on
extern const UnkStruct_02014E30 ov71_0224BBDC;
void ov71_02247124(void *,u32,u32,u32,u32);
void ov71_02247124(void *context,u32 which,u32 bg,u32 palette,u32 large) {
    u32 size=large?6400:3200;
    void *buffer=Heap_AllocAtEnd((enum HeapID)56,size);
    struct {UnkStruct_02014E30 settings;PokepicTemplate info;} local;
    u16 species;
    BOOL lock;
    BoxPokemon *mon;
    u32 personality;
    if(buffer) {
        local.settings=ov71_0224BBDC;
        if(which==0)mon=*(BoxPokemon **)*(void **)context;
        else mon=*(BoxPokemon **)((u8*)*(void **)context+4);
        lock=AcquireBoxMonLock(mon);
        GetBoxmonSpriteCharAndPlttNarcIds(&local.info,mon,2,0);
        personality=GetBoxMonData(mon,0,0);
        species=GetBoxMonData(mon,5,0);
        if(large)local.settings.w*=2;
        if(GetBoxMonData(mon,76,0)==1&&species==327)species=494;
        sub_02014510((NarcId)local.info.narcID,local.info.charDataID,(enum HeapID)56,&local.settings,buffer,personality,large,2,species);
        DC_FlushRange(buffer,size);
        BG_LoadCharTilesData((BgConfig *)*(void **)((u8*)context+8),(u8)bg,buffer,size,0);
        ReleaseBoxMonLock(mon,lock);
        Heap_Free(buffer);
    }
    GfGfxLoader_GXLoadPal((NarcId)local.info.narcID,local.info.palDataID,(enum GFPalLoadLocation)(bg>=4?4:0),(enum GFPalSlotOffset)(palette*32),32,(enum HeapID)56);
}
