	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_WL_MaData
	.extern WMSP_WL_MlmePowerManagement
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text

	arm_func_start WMSP_SetPowerSaveMode
WMSP_SetPowerSaveMode: ; 0x0600894C
	stmdb sp!, {r3, r4, r5, r6, r7, lr}
	sub sp, sp, #0x230
	ldr r1, _06008AC8 ; =wmspW + 0x1000
	mov r7, r0
	ldr r6, [r1, #0x550]
	add r5, sp, #0x30
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r4, r0
	mov r1, #0x28
	strh r1, [r4]
	ldrh r1, [r6]
	cmp r1, #0xb
	beq _06008990
	mov r1, #3
	strh r1, [r4, #2]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06008ABC
_06008990:
	ldr r0, [r7, #4]
	mov r2, #0
	cmp r0, #1
	moveq r0, #1
	movne r0, #0
	mov r1, r0, lsl #0x10
	mov r0, r5
	mov r1, r1, lsr #0x10
	mov r3, #1
	bl WMSP_WL_MlmePowerManagement
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _060089E4
	mov r1, #1
	strh r1, [r4, #2]
	strh r1, [r4, #4]
	ldrh r1, [r0, #4]
	mov r0, r4
	strh r1, [r4, #6]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06008ABC
_060089E4:
	ldr r0, _06008AC8 ; =wmspW + 0x1000
	mov r2, #6
	ldr r5, [r0, #0x550]
	add r0, r5, #0x8a
	add r0, r0, #0x100
	add r1, r5, #0xa2
	bl __VENEER_MI_CpuCopy8
	add r1, sp, #0x30
	str r1, [r5, #0xa8]
	mov r0, #0
	strh r0, [r5, #0xac]
	mov r2, #1
	str r2, [r5, #0x18]
	add r1, sp, #0
	mov r2, #0x30
	bl __VENEER_MIi_CpuClear16
	mov r0, #0
	strh r0, [sp]
	strh r0, [sp, #6]
	add r0, r5, #0x100
	ldrh r0, [r0, #0xec]
	add r1, sp, #0x18
	cmp r0, #2
	moveq r3, #0x14
	add r0, r5, #0x8a
	movne r3, #0xa
	add r0, r0, #0x100
	mov r2, #6
	strb r3, [sp, #0xe]
	bl __VENEER_MI_CpuCopy8
	add r1, sp, #0x1e
	add r0, r5, #0xe0
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	add r0, sp, #0x30
	str r0, [sp, #0x2c]
	add r1, sp, #0
	bl WMSP_WL_MaData
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06008AAC
	mov r1, #1
	strh r1, [r4, #2]
	mov r1, #0x100
	strh r1, [r4, #4]
	ldrh r1, [r0, #4]
	mov r0, r4
	strh r1, [r4, #6]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06008ABC
_06008AAC:
	mov r1, #0
	mov r0, r4
	strh r1, [r4, #2]
	bl __VENEER_WMSP_ReturnResult2Wm9
_06008ABC:
	add sp, sp, #0x230
	ldmia sp!, {r3, r4, r5, r6, r7, lr}
	bx lr
	.align 2, 0
_06008AC8: .word wmspW + 0x1000
	arm_func_end WMSP_SetPowerSaveMode
