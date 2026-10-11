#ifndef POKEHEARTGOLD_OVERLAY14_BOX_WINDOW_FRAME_R58_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY14_BOX_WINDOW_FRAME_R58_PRIVATE_H
#include "global.h"

#include "bg_window.h"
#include "bg_cell_manager.h"
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
extern void ov14_021F5BD8(void *, u32, u32, u32, u32);
extern void PCBox_DrawButtonWindow(void *, u32, u32, u32);
extern void BgCellManager_PutWindowTiles(void *, u32, void *);
void PCBox_DrawChangeBoxTitle(void *);
void PCBox_DrawBoxMenu(void *);
void PCBox_DrawChangeWallpaperTitle(void *);
void PCBox_DrawDepositTitle(void *);
void PCBox_SetPanelWindowFrame(BgCellManager *, int, Window *);
void PCBox_AttachWindowToPanel(BgCellManager *, int, Window *);
void PCBox_AttachPanelWindows(void *);
#endif
