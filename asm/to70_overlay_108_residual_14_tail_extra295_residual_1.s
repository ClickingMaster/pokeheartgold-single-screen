	.extern _021EA720
	.extern ov108_021E77D4
	.extern ov108_021E78F4
	.extern ov108_021E7EB0
	.include "asm/macros.inc"
	.include "overlay_108.inc"
	.include "global.inc"

.text
	thumb_func_start ov108_021E82E0
ov108_021E82E0: ; 0x021E82E0
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	str r1, [sp]
	ldr r1, _021E837C ; =0x000184E2
	add r6, r0, #0
	ldrb r0, [r6, r1]
	add r3, sp, #8
	lsl r0, r0, #0x1d
	lsr r2, r0, #0x1f
	mov r0, #6
	mul r0, r2
	sub r2, r1, #3
	ldrb r2, [r6, r2]
	add r2, r2, r0
	strb r2, [r3, #2]
	sub r2, r1, #2
	ldrb r2, [r6, r2]
	add r0, r2, r0
	strb r0, [r3, #3]
	sub r0, r1, #3
	ldrb r0, [r6, r0]
	mov r1, #3
	bl _s32_div_f
	add r1, sp, #8
	strb r0, [r1]
	ldr r0, _021E8380 ; =0x000184E0
	mov r1, #3
	ldrb r0, [r6, r0]
	bl _s32_div_f
	add r1, sp, #8
	strb r0, [r1, #1]
	ldr r0, [sp]
	add r5, sp, #8
	ldr r1, _021E8384 ; =_021EA720
	lsl r0, r0, #1
	add r0, r1, r0
	mov r7, #0
	add r4, sp, #8
	add r5, #2
	str r0, [sp, #4]
_021E8334:
	ldrb r3, [r4]
	ldrb r0, [r5]
	ldr r2, [sp, #4]
	ldrsb r3, [r2, r3]
	lsl r0, r0, #2
	add r1, r6, r0
	mov r0, #0xdb
	mov r2, #0xc
	lsl r0, r0, #2
	mul r2, r3
	ldr r0, [r1, r0]
	lsl r2, r2, #0x10
	mov r1, #0
	asr r2, r2, #0x10
	bl ManagedSprite_OffsetPositionXY
	add r7, r7, #1
	add r4, r4, #1
	add r5, r5, #1
	cmp r7, #2
	blt _021E8334
	ldr r0, _021E8388 ; =0x000184E1
	ldrb r1, [r6, r0]
	add r1, r1, #1
	strb r1, [r6, r0]
	ldrb r1, [r6, r0]
	cmp r1, #8
	blo _021E8376
	mov r1, #0
	strb r1, [r6, r0]
	add sp, #0xc
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_021E8376:
	mov r0, #0
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_021E837C: .word 0x000184E2
_021E8380: .word 0x000184E0
_021E8384: .word _021EA720
_021E8388: .word 0x000184E1
	thumb_func_end ov108_021E82E0
