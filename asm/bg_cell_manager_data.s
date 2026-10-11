	.public sBgCellTransferFuncs
	.include "asm/macros.inc"
	.include "unk_020192D0.inc"
	.include "global.inc"

    .rodata

	.public sBgCellTransferFuncs
sBgCellTransferFuncs:
	.word BgCellManager_NoTransfer, BgCommitTilemapBufferToVram, ScheduleBgTilemapBufferTransfer

.text
