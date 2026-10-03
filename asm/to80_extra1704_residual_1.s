	.extern ov99_021E8FEC
	.extern ov99_021EA350
	.extern ov99_021EA3C4
	.include "asm/macros.inc"
	.include "overlay_99.inc"
	.include "global.inc"

.text
	thumb_func_start ov99_021E92EC
ov99_021E92EC: ; 0x021E92EC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	str r0, [sp]
	str r1, [sp, #4]
	add r0, r1, #0
	str r2, [sp, #8]
	add r1, r2, #0
	mov r2, #0x6e
	lsl r2, r2, #2
	bl MI_CpuCopy8
	ldr r1, _021E93D8 ; =ov99_021EA350
	mov r0, #0
	str r0, [sp, #0xc]
	ldrh r2, [r1, #4]
	add r0, sp, #0x20
	strh r2, [r0]
	ldrh r2, [r1, #6]
	ldrh r1, [r1, #8]
	strh r2, [r0, #2]
	strh r1, [r0, #4]
_021E9316:
	ldr r0, [sp, #0xc]
	mov r6, #0
	lsl r0, r0, #0x18
	ldr r4, [sp, #4]
	lsr r5, r0, #0x18
	add r7, r6, #0
_021E9322:
	ldrh r2, [r4]
	add r0, r5, #0
	add r1, r7, #0
	bl ov98_0221EFB4
	cmp r0, #0
	beq _021E9338
	add r6, r6, #1
	add r4, #8
	cmp r6, #5
	blt _021E9322
_021E9338:
	cmp r6, #0
	bne _021E9344
	ldr r0, [sp]
	mov r1, #1
	add r0, #0xbc
	str r1, [r0]
_021E9344:
	add r0, sp, #0x20
	ldrh r0, [r0]
	ldr r4, [sp, #8]
	ldr r5, [sp, #4]
	str r0, [sp, #0x10]
	add r0, sp, #0x20
	ldrh r0, [r0, #2]
	mov r7, #0
	str r0, [sp, #0x14]
	add r0, sp, #0x20
	ldrh r0, [r0, #4]
	str r0, [sp, #0x18]
	ldr r0, [sp, #0xc]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x1c]
_021E9364:
	cmp r7, r6
	bne _021E938E
	ldr r1, [sp, #0x10]
	add r0, sp, #0x20
	strh r1, [r0, #6]
	ldr r1, [sp, #0x14]
	mov r2, #6
	strh r1, [r0, #8]
	ldr r1, [sp, #0x18]
	strh r1, [r0, #0xa]
	add r0, sp, #0x24
	add r0, #2
	add r1, r4, #2
	bl MI_CpuCopy8
	ldr r0, [sp, #0x1c]
	mov r1, #0
	bl ov98_0221EFA4
	strh r0, [r4]
	b _021E93AC
_021E938E:
	cmp r7, r6
	ble _021E93AC
	add r0, r5, #0
	sub r0, #8
	ldrh r0, [r0]
	strh r0, [r4]
	sub r0, r5, #6
	ldrh r0, [r0]
	strh r0, [r4, #2]
	sub r0, r5, #4
	ldrh r0, [r0]
	strh r0, [r4, #4]
	sub r0, r5, #2
	ldrh r0, [r0]
	strh r0, [r4, #6]
_021E93AC:
	add r7, r7, #1
	add r4, #8
	add r5, #8
	cmp r7, #5
	blt _021E9364
	ldr r0, [sp, #4]
	add r0, #0x2c
	str r0, [sp, #4]
	ldr r0, [sp]
	add r0, r0, #4
	str r0, [sp]
	ldr r0, [sp, #8]
	add r0, #0x2c
	str r0, [sp, #8]
	ldr r0, [sp, #0xc]
	add r0, r0, #1
	str r0, [sp, #0xc]
	cmp r0, #0xa
	blt _021E9316
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	nop
_021E93D8: .word ov99_021EA350
	thumb_func_end ov99_021E92EC
