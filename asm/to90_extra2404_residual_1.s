	.extern ov99_021E8518
	.extern ov99_021E86D4
	.extern ov99_021E9464
	.extern ov99_021E94CC
	.extern ov99_021EA394
	.include "asm/macros.inc"
	.include "overlay_99.inc"
	.include "global.inc"

.text
	thumb_func_start ov99_021E8D88
ov99_021E8D88: ; 0x021E8D88
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x60
	add r5, r0, #0
	mov r0, #0
	add r2, sp, #0x2c
	add r1, r0, #0
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	str r0, [sp, #0x20]
	str r0, [r2]
	str r0, [sp, #0x18]
	mov r0, #0x48
	str r0, [sp, #0x14]
	mov r0, #6
	str r0, [sp, #0x10]
_021E8DAE:
	mov r0, #0
	str r0, [sp, #0x1c]
	mov r0, #0x20
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x10]
	mov r7, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp, #8]
	ldr r0, [sp, #0x14]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp, #0x24]
_021E8DCC:
	ldr r1, [sp, #0x1c]
	ldr r0, [sp, #0x18]
	mov r2, #0x18
	add r0, r1, r0
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, _021E8EB8 ; =ov99_021EA394
	add r1, sp, #0x40
	bl MI_CpuCopy8
	mov r0, #1
	str r0, [sp, #0x3c]
	ldr r1, [sp, #0xc]
	add r0, sp, #0x2c
	strh r1, [r0]
	ldr r1, [sp, #0x24]
	strh r1, [r0, #2]
	mov r0, #0
	str r0, [sp, #0x34]
	mov r1, #0x10
	add r0, sp, #0x2c
	strh r1, [r0, #6]
	add r0, r4, #0
	add r0, #0xc
	lsl r0, r0, #2
	add r6, r5, r0
	ldr r0, [r5, #0x14]
	add r1, sp, #0x2c
	bl ov98_0221E6E0
	str r0, [r6, #0x18]
	lsl r0, r4, #2
	str r0, [sp, #0x28]
	add r0, r5, r0
	add r0, #0xbc
	ldr r0, [r0]
	cmp r0, #0
	bne _021E8E1C
	mov r1, #1
	b _021E8E1E
_021E8E1C:
	mov r1, #0
_021E8E1E:
	ldr r0, [r6, #0x18]
	bl ManagedSprite_SetDrawFlag
	mov r0, #1
	str r0, [sp, #0x34]
	mov r1, #4
	add r0, sp, #0x2c
	strh r1, [r0, #6]
	add r0, r4, #2
	lsl r0, r0, #2
	add r6, r5, r0
	ldr r0, [r5, #0x14]
	add r1, sp, #0x2c
	bl ov98_0221E6E0
	lsl r1, r4, #0x18
	str r0, [r6, #0x18]
	add r0, r5, #0
	asr r1, r1, #0x18
	mov r2, #1
	bl ov99_021E8518
	add r1, r0, #0
	ldr r0, [r6, #0x18]
	bl ManagedSprite_SetDrawFlag
	ldr r0, [r6, #0x18]
	mov r1, #1
	bl ManagedSprite_SetAnimateFlag
	ldr r0, [sp, #0x28]
	add r0, r5, r0
	add r0, #0xbc
	ldr r0, [r0]
	cmp r0, #0
	bne _021E8E6A
	mov r0, #1
	b _021E8E6C
_021E8E6A:
	mov r0, #0
_021E8E6C:
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	lsl r2, r7, #0x10
	lsr r2, r2, #0x10
	lsl r2, r2, #0x10
	ldr r3, [sp, #8]
	add r0, r5, #0
	add r1, r4, #0
	asr r2, r2, #0x10
	bl ov99_021E86D4
	ldr r0, [sp, #0xc]
	add r7, r7, #6
	add r0, #0x30
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x1c]
	add r0, r0, #1
	str r0, [sp, #0x1c]
	cmp r0, #5
	blo _021E8DCC
	ldr r0, [sp, #0x18]
	add r0, r0, #5
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x14]
	add r0, #0x40
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x10]
	add r0, #8
	str r0, [sp, #0x10]
	ldr r0, [sp, #0x20]
	add r0, r0, #1
	str r0, [sp, #0x20]
	cmp r0, #2
	bhs _021E8EB4
	b _021E8DAE
_021E8EB4:
	add sp, #0x60
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021E8EB8: .word ov99_021EA394
	thumb_func_end ov99_021E8D88
