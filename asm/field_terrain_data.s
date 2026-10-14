	.include "asm/macros.inc"
	.include "field_terrain_data.inc"
	.include "global.inc"

	.rodata

	.public _020FC5FC
_020FC5FC:
	.short 0x00D0
	.short 0x00D2
	.short 0x00D3
	.short 0x0000
	.public gTerrainQueries_Bdhc
gTerrainQueries_Bdhc:
	.word Terrain_GetHeight_Bdhc
	.word Terrain_GetAttributes_Loaded
	.public _020FC60C
_020FC60C:
	.short 0x00D0
	.short 0x00D1
	.short 0x00D2
	.short 0x00D3
	.public gTerrainQueries_Simple
gTerrainQueries_Simple:
	.word Terrain_GetHeight_Simple
	.word Terrain_GetAttributes_Simple
