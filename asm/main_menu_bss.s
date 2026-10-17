	.include "asm/macros.inc"
	.public sMigrateFromAgbIcons
	.public sPmAgbSave
	.public sPmAgbSaveCounter
	.public sPmAgbRomHeader
	.public sMainMenuUtilCryptoHeapId
	.bss

sMigrateFromAgbIcons:
	.space 0x4

sPmAgbSave:
	.space 0x10

sPmAgbSaveCounter:
	.space 0x8

sPmAgbRomHeader:
	.space 0xFC

sMainMenuUtilCryptoHeapId:
	.space 0x4
