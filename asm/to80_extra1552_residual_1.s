	.extern ov80_02236C2C
	.extern ov80_02236C9C
	.extern ov80_02236DD4
	.extern ov80_02236DF8
	.extern ov80_022370F4
	.extern ov80_02237264
    .include "asm/macros.inc"
	.include "overlay_80_02236B78.inc"
	.include "global.inc"

.text
	thumb_func_start ov80_02236E90
ov80_02236E90: ; 0x02236E90
	push {r4, r5, r6, r7, lr}
	sub sp, #0x84
	str r0, [sp, #0x14]
	ldr r0, [sp, #0xa8]
	add r7, r3, #0
	str r0, [sp, #0xa8]
	add r0, r1, #0
	add r1, r2, #0
	bl ov80_02236C2C
	str r0, [sp, #0x18]
	mov r0, #0
	mov ip, r0
	ldr r0, [sp, #0xa8]
	cmp r0, #0
	ble _02236EE4
	add r4, sp, #0x34
	add r5, sp, #0x1c
_02236EB4:
	add r6, r7, #0
	add r3, sp, #0x4c
	mov r2, #7
_02236EBA:
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02236EBA
	add r0, sp, #0x1c
	ldrh r0, [r0, #0x30]
	add r7, #0x38
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	strh r0, [r4]
	add r0, sp, #0x1c
	ldrh r0, [r0, #0x32]
	add r4, r4, #2
	strh r0, [r5]
	mov r0, ip
	add r1, r0, #1
	ldr r0, [sp, #0xa8]
	add r5, r5, #2
	mov ip, r1
	cmp r1, r0
	blt _02236EB4
_02236EE4:
	ldr r0, [sp, #0x98]
	ldr r2, [sp, #0xa8]
	str r0, [sp]
	mov r0, #0xb
	str r0, [sp, #4]
	ldr r0, [sp, #0x18]
	add r1, sp, #0x1c
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [sp, #0xa0]
	str r0, [sp, #0x10]
	ldr r3, [sp, #0x14]
	add r0, sp, #0x34
	bl ov80_02236C9C
	ldr r0, [sp, #0xa4]
	ldr r1, [sp, #0x98]
	str r0, [sp]
	ldr r0, [sp, #0x14]
	ldr r2, [sp, #0xa0]
	str r0, [sp, #4]
	mov r0, #0xb
	str r0, [sp, #8]
	mov r0, #0xcd
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x9c]
	mov r3, #0
	bl ov80_0222A52C
	add sp, #0x84
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov80_02236E90
