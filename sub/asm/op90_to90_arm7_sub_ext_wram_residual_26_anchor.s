	.include "asm/macros.inc"
	.include "global.inc"

	.extern AddTask
	.extern OS_IsThreadTerminated
	.extern OS_JoinThread
	.extern pTaskFunc

.text
