	.public sBgCellTransferFuncs
	.include "asm/macros.inc"
	.include "global_terminal_bg_cell_data.inc"
	.include "global.inc"

    .rodata

	.public sBgCellTransferFuncs
sBgCellTransferFuncs:
	.word BgCellManager_NoTransfer, BgCommitTilemapBufferToVram, ScheduleBgTilemapBufferTransfer

.text
