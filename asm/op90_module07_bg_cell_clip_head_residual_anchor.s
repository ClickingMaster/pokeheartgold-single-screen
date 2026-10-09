	.public _020F6298
	.include "asm/macros.inc"
	.include "unk_020192D0.inc"
	.include "global.inc"

    .rodata

	.public _020F6298
_020F6298:
	.word sub_02019BA0, BgCommitTilemapBufferToVram, ScheduleBgTilemapBufferTransfer

.text
