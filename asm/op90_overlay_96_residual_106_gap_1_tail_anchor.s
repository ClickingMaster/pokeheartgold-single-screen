	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

	.public AddTextPrinterParameterizedWithColor
	.public CopyRectToBgTilemapRect
	.public CopyWindowToVram
	.public FillWindowPixelBuffer
	.public GF_AssertFail
	.public ManagedSprite_SetPaletteOverride
	.public ManagedSprite_SetPriority
	.public PlayerProfile_GetPlayerName_NewString
	.public PokeathlonCourse_GetPlayerProfileFromData
	.public ScheduleBgTilemapBufferTransfer
	.public String_Delete
	.public _s32_div_f
	.public ov96_022104D8
.text
