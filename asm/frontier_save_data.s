#include "config.h"
	.include "asm/macros.inc"
	.include "frontier_save_data.inc"
	.include "global.inc"

	.bss

	.public gFrontierSave
gFrontierSave:
	.space 0x1628

	.text
