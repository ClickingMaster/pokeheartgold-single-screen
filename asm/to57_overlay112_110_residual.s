#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_112.inc"
	.include "global.inc"

	.text
	.extern ov112_021F30BC
	.extern ov112_021F31BC
	.extern ov112_021F3140
	thumb_func_start ov112_021ED7DC
ov112_021ED7DC: ; 0x021ED7DC
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r4, r0, #0
	add r0, sp, #4
	mov r2, #0
	str r2, [r0]
	str r2, [r0, #4]
	ldr r3, _021ED8C8 ; =0x00009D54
	str r2, [r0, #8]
	add r0, r4, r3
	str r0, [sp, #4]
	add r0, r3, #0
	sub r0, #0x10
	ldrh r1, [r4, r0]
	add r0, sp, #0
	mov r5, #0x1f
	strh r1, [r0, #0xc]
	ldrb r1, [r0, #0xe]
	bic r1, r5
	sub r5, r3, #3
	ldrb r5, [r4, r5]
	lsl r5, r5, #0x1b
	lsr r6, r5, #0x1b
	mov r5, #0x1f
	and r5, r6
	orr r1, r5
	strb r1, [r0, #0xe]
	ldrb r1, [r0, #0xe]
	mov r5, #0x60
	bic r1, r5
	sub r5, r3, #3
	ldrb r5, [r4, r5]
	sub r3, r3, #2
	lsl r5, r5, #0x19
	lsr r5, r5, #0x1e
	lsl r5, r5, #0x1e
	lsr r5, r5, #0x19
	orr r1, r5
	strb r1, [r0, #0xe]
	ldrb r3, [r4, r3]
	ldrb r1, [r0, #0xe]
	mov r5, #0x80
	lsl r3, r3, #0x1e
	lsr r3, r3, #0x1f
	lsl r3, r3, #0x1f
	bic r1, r5
	lsr r3, r3, #0x18
	orr r1, r3
	strb r1, [r0, #0xe]
	ldr r0, _021ED8CC ; =0x000010F0
	ldr r1, [r4, r0]
	ldr r0, _021ED8D0 ; =0x0001D758
	ldr r0, [r4, r0]
	cmp r1, r0
	bhs _021ED84E
	str r2, [sp, #8]
	b _021ED852
_021ED84E:
	sub r0, r1, r0
	str r0, [sp, #8]
_021ED852:
	ldr r0, _021ED8D4 ; =0x0000AA34
	add r1, sp, #4
	add r0, r4, r0
	bl ov112_021F30BC
	ldr r0, _021ED8D8 ; =0x00009DFC
	add r0, r4, r0
	bl ov112_021F31BC
	cmp r0, #0
	beq _021ED8C2
	mov r0, #0xad
	lsl r0, r0, #8
	ldrh r2, [r4, r0]
	add r1, sp, #0
	mov r3, #0x1f
	strh r2, [r1]
	ldrb r2, [r1, #2]
	bic r2, r3
	add r3, r0, #0
	add r3, #0xd
	ldrb r3, [r4, r3]
	lsl r3, r3, #0x1b
	lsr r5, r3, #0x1b
	mov r3, #0x1f
	and r3, r5
	orr r2, r3
	strb r2, [r1, #2]
	ldrb r2, [r1, #2]
	mov r3, #0x60
	bic r2, r3
	add r3, r0, #0
	add r3, #0xd
	ldrb r3, [r4, r3]
	add r0, #0xe
	lsl r3, r3, #0x19
	lsr r3, r3, #0x1e
	lsl r3, r3, #0x1e
	lsr r3, r3, #0x19
	orr r2, r3
	strb r2, [r1, #2]
	ldrb r0, [r4, r0]
	ldrb r2, [r1, #2]
	mov r3, #0x80
	lsl r0, r0, #0x1e
	lsr r0, r0, #0x1f
	lsl r0, r0, #0x1f
	bic r2, r3
	lsr r0, r0, #0x18
	orr r0, r2
	strb r0, [r1, #2]
	ldr r0, _021ED8D8 ; =0x00009DFC
	add r1, sp, #0
	add r0, r4, r0
	bl ov112_021F3140
_021ED8C2:
	add sp, #0x10
	pop {r4, r5, r6, pc}
	nop
_021ED8C8: .word 0x00009D54
_021ED8CC: .word 0x000010F0
_021ED8D0: .word 0x0001D758
_021ED8D4: .word 0x0000AA34
_021ED8D8: .word 0x00009DFC
	thumb_func_end ov112_021ED7DC
