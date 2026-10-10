#ifndef POKEHEARTGOLD_RENDER_WINDOW_INTERNAL_H
#define POKEHEARTGOLD_RENDER_WINDOW_INTERNAL_H

// Full declarations of the window frame module: src/render_window.c (frames), src/render_window_frame_icon.c and
// src/render_window_icons.c (down arrow, signposts, waiting icon, Pokemon picture window). include/render_window.h
// is the public subset other modules use; it declares WaitingIcon_DeleteWithoutRestore() without its argument,
// WaitingIcon_New() with a u16 tile and the DrawPokemonPic* results as PokepicManager pointers (what their callers
// were matched against), so the module's own files include this header instead.

#include "field/field_sprite_manager.h"

#include "bg_window.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "unk_02013FDC.h"

typedef struct WaitingIcon WaitingIcon;
typedef struct PokemonPicWindow PokemonPicWindow;

extern const UnkStruct_02014E30 sPokemonPicFrame1Rect;
extern const UnkStruct_02014E30 sPokemonPicFrame0Rect;
extern const SpriteResourceCountsListUnion sPokemonPicWindowResourceCounts;
extern const ManagedSpriteTemplate sPokemonPicWindowSpriteTemplate;

void LoadUserFrameGfx1Tiles(BgConfig *bgConfig, u32 layer, u32 baseTile, BOOL frame, u32 heapId);
u32 GetUserFrameGfx1PaletteMember(void);
void LoadUserFrameGfx1(BgConfig *bgConfig, enum GFBgLayer layer, u32 baseTile, u32 paletteNum, u8 frame, enum HeapID heapId);
void DrawFrame1Tilemap(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 paletteNum, u16 baseTile);
void DrawFrameAndWindow1(Window *window, BOOL dontCopyToVram, u16 baseTile, u8 paletteNum);
void EraseFrameAndWindow1(Window *window, BOOL dontCopyToVram);
u32 GetUserFrameGfx2TilesMember(u32 frame);
u32 GetUserFrameGfx2PaletteMember(u32 frame);
void LoadUserFrameGfx2(BgConfig *bgConfig, u32 layer, u32 baseTile, u32 paletteNum, u8 frame, enum HeapID heapId);
void DrawFrame2Tilemap(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 paletteNum, u16 baseTile);
void DrawFrame2AroundWindow(Window *window, u32 baseTile, u32 paletteNum);
void DrawFrameAndWindow2(Window *window, BOOL dontCopyToVram, u32 baseTile, u32 paletteNum);
void ClearFrameAndWindow2(Window *window, BOOL dontCopyToVram);
void BlitPixelRect4Bit(void *src, u16 srcX, u16 srcY, u16 srcWidth, u16 srcHeight, void *dest, u16 destWidth, u16 destHeight, u16 destX, u16 destY, u16 width, u16 height);
void LoadWindowFrameIconTiles(Window *window, int member, int baseTile, int count, u8 srcX, u8 srcY);
void LoadDownArrowTiles(Window *window, u32 baseTile);
void LoadUserFrameGfx2WithFill(BgConfig *bgConfig, u8 bgId, u16 baseTile, u8 color, u8 frame, enum HeapID heapId);
void LoadMapSignpostFrameAndGraphic(BgConfig *bgConfig, u8 bgId, u16 baseTile, u8 plttNum, u8 type, u16 map, enum HeapID heapId);
void LoadMapSignpostGraphic(BgConfig *bgConfig, u32 bgId, u16 baseTile, BOOL type, u16 map, u32 heapId);
void DrawSignpostFrameTilemap(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 paletteNum, u16 baseTile);
void DrawSignpostGraphicTilemap(Window *window, u16 baseTile, u8 paletteNum);
void DrawFrameAndWindow3(Window *window, BOOL dontCopyToVram, u16 baseTile, u8 paletteNum, u8 type);
WaitingIcon *WaitingIcon_New(Window *window, int baseTile);
void WaitingIcon_Draw(WaitingIcon *icon, int mode);
void WaitingIcon_Task(SysTask *task, WaitingIcon *icon);
void WaitingIcon_FreeTask(SysTask *task, void *icon);
void WaitingIcon_Delete(WaitingIcon *icon);
void WaitingIcon_DeleteWithoutRestore(WaitingIcon *icon);
u8 *DrawPokemonPicFromSpecies(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u8 paletteNum, u16 baseTile, u16 species, u8 gender, u32 heapId);
u8 *DrawPokemonPicFromMon(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u8 paletteNum, u16 baseTile, Pokemon *mon, u32 heapId);
void PokemonPicWindow_Task(SysTask *task, PokemonPicWindow *picWindow);
PokemonPicWindow *PokemonPicWindow_New(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u32 heapId);
void PokemonPicWindow_InitSpriteManager(PokemonPicWindow *picWindow, enum HeapID heapId);
void PokemonPicWindow_LoadSpriteResources(PokemonPicWindow *picWindow);
void PokemonPicWindow_CreateSprite(PokemonPicWindow *picWindow, int x, int y);
void PokemonPicWindow_LoadSpeciesGfx(PokemonPicWindow *picWindow, u16 species, u8 gender);
void PokemonPicWindow_LoadMonGfx(PokemonPicWindow *picWindow, Pokemon *mon);
void PokemonPicWindow_LoadGfx(PokemonPicWindow *picWindow, PokepicTemplate *template);
void PokemonPicWindow_DrawFrame(PokemonPicWindow *picWindow, u8 paletteNum, u16 baseTile);
void PokemonPicWindow_ClearFrame(PokemonPicWindow *picWindow);

#endif // POKEHEARTGOLD_RENDER_WINDOW_INTERNAL_H
