	.include "asm/macros.inc"
	.public gMigrateFromAgbIcons
	.public gPmAgbSave
	.public gPmAgbSaveCounter
	.public gPmAgbRomHeader
	.public gMainMenuUtilCryptoHeapId
	.bss

gMigrateFromAgbIcons:
	.space 0x4

gPmAgbSave:
	.space 0x10

gPmAgbSaveCounter:
	.space 0x8

gPmAgbRomHeader:
	.space 0xFC

gMainMenuUtilCryptoHeapId:
	.space 0x4
