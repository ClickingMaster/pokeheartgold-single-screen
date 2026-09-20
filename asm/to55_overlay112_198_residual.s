#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_112.inc"
	.include "global.inc"

	.text
	.extern ov112_021F178C
	.extern ov112_021F1838
	.extern ov112_021F1A40
	.extern ov112_021F1B94
	.extern ov112_021F1DC0
	.extern ov112_021F2B80
	.extern ov112_021F2DE4

	thumb_func_start ov112_021F2EF4
ov112_021F2EF4: ; 0x021F2EF4
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r6, r0, #0
	bl OverlayManager_GetData
	ldr r1, [r4]
	add r5, r0, #0
	cmp r1, #4
	bhi _021F2F9C
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_021F2F14: ; jump table
	.short _021F2F1E - _021F2F14 - 2 ; case 0
	.short _021F2F2C - _021F2F14 - 2 ; case 1
	.short _021F2F3E - _021F2F14 - 2 ; case 2
	.short _021F2F70 - _021F2F14 - 2 ; case 3
	.short _021F2F8E - _021F2F14 - 2 ; case 4
_021F2F1E:
	add r0, r6, #0
	bl ov112_021F2DE4
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _021F2FA0
_021F2F2C:
	bl ov112_021F1838
	add r0, r5, #0
	bl ov112_021F1A40
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _021F2FA0
_021F2F3E:
	bl ov112_021F1DC0
	cmp r0, #0
	beq _021F2FA0
	add r0, r5, #0
	bl ov112_021F1B94
	add r0, r5, #0
	bl ov112_021F178C
	mov r1, #0x51
	lsl r1, r1, #2
	ldr r2, [r5, r1]
	mov r0, #0xff
	bic r2, r0
	mov r0, #0xff
	orr r0, r2
	str r0, [r5, r1]
	add r0, r5, #0
	bl ov112_021F2B80
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _021F2FA0
_021F2F70:
	mov r0, #6
	mov r1, #1
	str r0, [sp]
	str r1, [sp, #4]
	ldr r0, [r5, #4]
	ldr r3, _021F2FA8 ; =0x00007FFF
	str r0, [sp, #8]
	mov r0, #0
	add r2, r1, #0
	bl BeginNormalPaletteFade
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _021F2FA0
_021F2F8E:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _021F2FA0
	add sp, #0xc
	mov r0, #1
	pop {r3, r4, r5, r6, pc}
_021F2F9C:
	bl GF_AssertFail
_021F2FA0:
	mov r0, #0
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	nop
_021F2FA8: .word 0x00007FFF
	thumb_func_end ov112_021F2EF4
