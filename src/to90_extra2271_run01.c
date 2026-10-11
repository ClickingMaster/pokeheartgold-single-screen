#pragma force_active on
#include "global.h"
#include "heap.h"
typedef struct {void *battle;u8 pad[8];enum HeapID heap;} Head;
typedef struct {Head *head;void *bg,*palette;} Context;
extern void *NARC_New(s32,enum HeapID),*NARC_AllocAndReadWholeMember(void *,s32,enum HeapID);
extern void NARC_Delete(void *),GfGfxLoader_LoadCharDataFromOpenNarc(void *,s32,void *,s32,s32,s32,s32,enum HeapID),GfGfxLoader_LoadScrnDataFromOpenNarc(void *,s32,void *,s32,s32,s32,s32,enum HeapID),PaletteData_LoadNarc(void *,s32,s32,enum HeapID,s32,s32,s32),GfGfxLoader_LoadCharData(s32,s32,void *,s32,s32,s32,s32,enum HeapID),BattleBagMenu_StoreButtonTilemaps(Context *,void *);
extern BOOL NNS_G2dGetUnpackedScreenData(void *,NNSG2dScreenData **);
extern s32 BattleSystem_GetFrame(void *),GetUserFrameGfx2TilesMember(s32),GetUserFrameGfx2PaletteMember(s32);
void BattleBagMenu_LoadBgGraphics(Context *p);
#pragma force_active on
void BattleBagMenu_LoadBgGraphics(Context *p){void *narc=NARC_New(77,p->head->heap);void *raw;NNSG2dScreenData *screen;s32 frame;GfGfxLoader_LoadCharDataFromOpenNarc(narc,2,p->bg,6,0,0,0,p->head->heap);GfGfxLoader_LoadScrnDataFromOpenNarc(narc,0,p->bg,6,0,0,0,p->head->heap);raw=NARC_AllocAndReadWholeMember(narc,1,p->head->heap);NNS_G2dGetUnpackedScreenData(raw,&screen);BattleBagMenu_StoreButtonTilemaps(p,(u8*)screen+12);Heap_Free(raw);NARC_Delete(narc);PaletteData_LoadNarc(p->palette,77,3,p->head->heap,1,384,0);PaletteData_LoadNarc(p->palette,16,8,p->head->heap,1,32,240);frame=BattleSystem_GetFrame(p->head->battle);GfGfxLoader_LoadCharData(38,GetUserFrameGfx2TilesMember(frame),p->bg,4,994,0,0,p->head->heap);PaletteData_LoadNarc(p->palette,38,GetUserFrameGfx2PaletteMember(frame),p->head->heap,1,32,224);}
