	.include "asm/macros.inc"
	.include "frontier_map_bg_animation_data.inc"
	.include "global.inc"
	.public FrontierMapBgAnim_CreateTileAnim
	.public FrontierMapBgAnim_TileTask
	.public gFrontierMapBgAnimScreens


    .text
	.public FrontierMapBgAnim_Create
	.public FrontierMapBgAnim_Free
	.public FrontierMapBgAnim_DestroyTileAnim
	.public FrontierMapBgAnim_CreatePaletteAnim
	.public FrontierMapBgAnim_DestroyPaletteAnim
	.public FrontierMapBgAnim_PaletteTask
    .rodata

gFrontierMapBgAnimScreens: ; 0x0223DB2C
	.byte 0x14, 0x00, 0x15, 0x00
