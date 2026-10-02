	.extern ov87_021E7DF8
	.extern ov87_021E7FEC
	.extern ov87_021E84C0
	.include "asm/macros.inc"
	.include "overlay_87.inc"
	.include "global.inc"

.text
	thumb_func_start ov87_021E7F6C
ov87_021E7F6C: ; 0x021E7F6C
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	add r6, r1, #0
	mov r0, #0x7a
	mov r1, #0x10
	add r7, r2, #0
	str r3, [sp, #8]
	bl Heap_Alloc
	add r4, r0, #0
	add r2, r4, #0
	mov r1, #0x10
	mov r0, #0
_021E7F88:
	strb r0, [r2]
	add r2, r2, #1
	sub r1, r1, #1
	bne _021E7F88
	ldr r0, [sp, #0x2c]
	ldr r3, [sp, #0x28]
	str r0, [sp]
	add r0, sp, #0x10
	ldrb r0, [r0, #0x14]
	add r1, r6, #0
	add r2, r7, #0
	str r0, [sp, #4]
	add r0, r5, #0
	bl ov87_021E7DF8
	add r2, sp, #0x10
	str r0, [r4, #0xc]
	ldrb r0, [r2, #0x14]
	strb r0, [r4, #2]
	ldrh r2, [r2, #0x10]
	ldr r1, [sp, #8]
	add r0, r4, #0
	bl ov87_021E7FEC
	add r0, r4, #0
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov87_021E7F6C
