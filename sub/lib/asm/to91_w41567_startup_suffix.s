	.include "asm/macros.inc"
	.include "global.inc"

	.extern SDK_AUTOLOAD_LIST
	.extern SDK_AUTOLOAD_LIST_END
	.extern SDK_AUTOLOAD_START
	.extern SDK_STATIC_BSS_START
	.extern SDK_STATIC_BSS_END
	.text
	arm_func_start detect_main_memory_size
detect_main_memory_size: ; 0x0238015C
	mov r0, #1
	mov r1, #0
	ldr r2, _02380194 ; =0x027FFFFA
	sub r3, r2, #0x400000
_0238016C:
	strh r1, [r2]
	ldrh ip, [r3]
	cmp r1, ip
	movne r0, #2
	bne _0238018C
	add r1, r1, #1
	cmp r1, #2
	bne _0238016C
_0238018C:
	strh r0, [r2]
	bx lr
	.align 2, 0
_02380194: .word 0x027FFFFA
	arm_func_end detect_main_memory_size

	.public _start_ModuleParams
_start_ModuleParams: ; 0x02380198
	.word SDK_AUTOLOAD_LIST
	.word SDK_AUTOLOAD_LIST_END
	.word SDK_AUTOLOAD_START
	.word SDK_STATIC_BSS_START
	.word SDK_STATIC_BSS_END
; SDK_STATIC_TEXT_END = 0x023801B0
