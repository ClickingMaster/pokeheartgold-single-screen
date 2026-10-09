	.include "asm/macros.inc"
	.include "global.inc"
	.extern SDK_AUTOLOAD_LIST
	.extern SDK_AUTOLOAD_LIST_END
	.extern SDK_AUTOLOAD_START
	.extern SDK_STATIC_BSS_START
	.extern SDK_STATIC_BSS_END
	.public _start_ModuleParams

.text
_start_ModuleParams: ; 0x02380198
	.word SDK_AUTOLOAD_LIST
	.word SDK_AUTOLOAD_LIST_END
	.word SDK_AUTOLOAD_START
	.word SDK_STATIC_BSS_START
	.word SDK_STATIC_BSS_END
