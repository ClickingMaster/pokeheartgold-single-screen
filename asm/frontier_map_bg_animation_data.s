	.include "asm/macros.inc"
	.include "overlay_80_02239D74.inc"
	.include "global.inc"
	.public FrontierMapBgAnim_CreateTileAnim
	.public FrontierMapBgAnim_TileTask
	.public ov80_0223DB2C


    .text
	.public FrontierMapBgAnim_Create
	.public FrontierMapBgAnim_Free
	.public FrontierMapBgAnim_DestroyTileAnim
	.public FrontierMapBgAnim_CreatePaletteAnim
	.public FrontierMapBgAnim_DestroyPaletteAnim
	.public FrontierMapBgAnim_PaletteTask
    .rodata

ov80_0223DB2C: ; 0x0223DB2C
	.byte 0x14, 0x00, 0x15, 0x00
