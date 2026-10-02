	.extern ov99_021E7450
	.extern ov99_021E7794
	.extern ov99_021E77D8
	.include "asm/macros.inc"
	.include "overlay_99.inc"
	.include "global.inc"
	.extern ov99_021E7508

.text
	thumb_func_start ov99_021E7598
ov99_021E7598: ; 0x021E7598
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	mov r0, #0xfd
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	lsl r0, r0, #4
	lsr r0, r0, #0x1f
	beq _021E75AE
	mov r0, #1
	b _021E75B2
_021E75AE:
	mov r0, #0
	mvn r0, r0
_021E75B2:
	lsl r0, r0, #6
	str r0, [sp]
	mov r0, #0xfe
	lsl r0, r0, #2
	ldr r1, [r5, r0]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bne _021E75E6
	add r0, r0, #4
	ldr r0, [r5, r0]
	bl GridInputHandler_GetNextInput
	add r4, r0, #0
	add r0, r5, #0
	bl ov99_021E7508
	cmp r4, #0x1e
	bne _021E75DE
	add r0, r5, #0
	bl ov99_021E77D8
	b _021E7636
_021E75DE:
	add r0, r5, #0
	bl ov99_021E7794
	b _021E7636
_021E75E6:
	cmp r1, #4
	bne _021E7636
	add r0, r5, #0
	bl ov99_021E7450
	mov r6, #0
	add r4, r5, #0
_021E75F4:
	mov r0, #0xfe
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r2, sp, #4
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	add r7, r1, #0
	ldr r0, [sp]
	add r1, sp, #4
	mul r7, r0
	ldr r0, _021E76D4 ; =0x00000458
	add r1, #2
	ldr r0, [r4, r0]
	bl ManagedSprite_GetPositionXY
	ldr r0, _021E76D4 ; =0x00000458
	add r2, sp, #4
	mov r1, #2
	ldrsh r2, [r2, r1]
	lsl r1, r7, #1
	add r3, sp, #4
	sub r1, r2, r1
	mov r2, #0
	lsl r1, r1, #0x10
	ldrsh r2, [r3, r2]
	ldr r0, [r4, r0]
	asr r1, r1, #0x10
	bl ManagedSprite_SetPositionXY
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #0x1e
	blt _021E75F4
_021E7636:
	mov r0, #0xfe
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #8
	bhs _021E7680
	ldr r0, [sp]
	mov r1, #2
	neg r4, r0
	ldr r0, [r5]
	mov r2, #1
	add r3, r4, #0
	bl ScheduleSetBgPosText
	ldr r0, [r5]
	mov r1, #3
	mov r2, #1
	add r3, r4, #0
	bl ScheduleSetBgPosText
	ldr r0, [sp]
	mov r6, #0
	lsl r0, r0, #0x10
	add r4, r5, #0
	asr r7, r0, #0x10
_021E766A:
	ldr r0, _021E76D4 ; =0x00000458
	add r1, r7, #0
	ldr r0, [r4, r0]
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #0x1e
	blt _021E766A
	b _021E76B4
_021E7680:
	mov r2, #0
	ldr r0, [r5]
	mov r1, #2
	add r3, r2, #0
	bl ScheduleSetBgPosText
	mov r2, #0
	ldr r0, [r5]
	mov r1, #3
	add r3, r2, #0
	bl ScheduleSetBgPosText
	mov r1, #0xfe
	lsl r1, r1, #2
	ldr r2, [r5, r1]
	mov r0, #0xff
	bic r2, r0
	str r2, [r5, r1]
	add r1, #0x10
	ldr r0, [r5, r1]
	mov r1, #1
	bl ManagedSprite_SetDrawFlag
	add sp, #8
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_021E76B4:
	mov r2, #0xfe
	lsl r2, r2, #2
	ldr r3, [r5, r2]
	mov r1, #0xff
	add r0, r3, #0
	bic r0, r1
	lsl r1, r3, #0x18
	lsr r1, r1, #0x18
	add r1, r1, #1
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	orr r0, r1
	str r0, [r5, r2]
	mov r0, #0
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021E76D4: .word 0x00000458
	thumb_func_end ov99_021E7598
