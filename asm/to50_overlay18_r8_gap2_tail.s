	.include "asm/macros.inc"
	.include "overlay_18.inc"
	.include "global.inc"
	.public ov18_021F1598

	.text
	thumb_func_start ov18_021F16C4
ov18_021F16C4: ; 0x021F16C4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	str r1, [sp]
	add r1, r3, #1
	add r5, r0, #0
	lsl r0, r1, #2
	mov r6, #0
	add r0, r1, r0
	str r2, [sp, #4]
	add r7, r6, #0
	str r0, [sp, #0xc]
	add r4, sp, #0x14
_021F16DC:
	ldr r1, _021F1758 ; =0x0000185E
	ldr r0, [sp]
	ldrb r2, [r5, r1]
	mov r1, #0x1e
	add r0, r0, r7
	mul r1, r2
	add r0, r0, r1
	str r0, [sp, #0x10]
	lsl r0, r0, #2
	add r1, r5, r0
	mov r0, #0x67
	lsl r0, r0, #4
	str r1, [sp, #8]
	ldr r0, [r1, r0]
	add r1, sp, #0x14
	add r1, #2
	add r2, sp, #0x14
	bl ManagedSprite_GetPositionXY
	mov r0, #0
	ldrsh r0, [r4, r0]
	cmp r0, #0xe0
	bne _021F172C
	mov r0, #0xf
	mvn r0, r0
	strh r0, [r4]
	ldr r1, _021F175C ; =0x00001859
	mov r2, #0xf
	ldrb r1, [r5, r1]
	add r0, r5, #0
	mul r2, r1
	ldr r1, [sp, #0xc]
	sub r1, r2, r1
	ldr r2, [sp, #0x10]
	add r1, r6, r1
	bl ov18_021F1598
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
_021F172C:
	mov r0, #0
	ldrsh r1, [r4, r0]
	ldr r0, [sp, #4]
	mov r2, #0
	add r0, r1, r0
	strh r0, [r4]
	mov r0, #0x67
	ldr r1, [sp, #8]
	lsl r0, r0, #4
	ldr r0, [r1, r0]
	mov r1, #2
	ldrsh r1, [r4, r1]
	ldrsh r2, [r4, r2]
	bl ManagedSprite_SetPositionXY
	add r0, r7, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	cmp r7, #0x1e
	blo _021F16DC
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021F1758: .word 0x0000185E
_021F175C: .word 0x00001859
	thumb_func_end ov18_021F16C4


	thumb_func_start ov18_021F1760
ov18_021F1760: ; 0x021F1760
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	str r1, [sp]
	add r1, r3, #5
	add r5, r0, #0
	lsl r0, r1, #2
	mov r6, #0
	add r0, r1, r0
	str r2, [sp, #4]
	add r7, r6, #0
	str r0, [sp, #0xc]
	add r4, sp, #0x14
_021F1778:
	ldr r1, _021F17F4 ; =0x0000185E
	ldr r0, [sp]
	ldrb r2, [r5, r1]
	mov r1, #0x1e
	add r0, r0, r7
	mul r1, r2
	add r0, r0, r1
	str r0, [sp, #0x10]
	lsl r0, r0, #2
	add r1, r5, r0
	mov r0, #0x67
	lsl r0, r0, #4
	str r1, [sp, #8]
	ldr r0, [r1, r0]
	add r1, sp, #0x14
	add r1, #2
	add r2, sp, #0x14
	bl ManagedSprite_GetPositionXY
	mov r0, #0
	ldrsh r1, [r4, r0]
	sub r0, #0x10
	cmp r1, r0
	bne _021F17C8
	mov r0, #0xe0
	strh r0, [r4]
	ldr r1, _021F17F8 ; =0x00001859
	mov r2, #0xf
	ldrb r1, [r5, r1]
	add r0, r5, #0
	mul r2, r1
	ldr r1, [sp, #0xc]
	add r1, r2, r1
	ldr r2, [sp, #0x10]
	add r1, r6, r1
	bl ov18_021F1598
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
_021F17C8:
	mov r0, #0
	ldrsh r1, [r4, r0]
	ldr r0, [sp, #4]
	mov r2, #0
	add r0, r1, r0
	strh r0, [r4]
	mov r0, #0x67
	ldr r1, [sp, #8]
	lsl r0, r0, #4
	ldr r0, [r1, r0]
	mov r1, #2
	ldrsh r1, [r4, r1]
	ldrsh r2, [r4, r2]
	bl ManagedSprite_SetPositionXY
	add r0, r7, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	cmp r7, #0x1e
	blo _021F1778
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021F17F4: .word 0x0000185E
_021F17F8: .word 0x00001859
	thumb_func_end ov18_021F1760
