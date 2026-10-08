#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_73.inc"
	.include "global.inc"

	.public BG_SetMaskColor
	.public BlendPalette
	.public DC_FlushRange
	.public GF_AssertFail
	.public GfGfxLoader_GXLoadPalFromOpenNarc
	.public GfGfxLoader_GetPlttDataFromOpenNarc
	.public GfGfxLoader_LoadCharDataFromOpenNarc
	.public GfGfxLoader_LoadScrnDataFromOpenNarc
	.public Heap_Free
	.public LoadFontPal1
	.public LoadUserFrameGfx1
	.public LoadUserFrameGfx2
	.public MI_CpuFill8
	.public MIi_CpuCopy16
	.public NARC_Delete
	.public NARC_New
	.public Options_GetFrame
	.public SysTask_CreateOnVBlankQueue
.text
