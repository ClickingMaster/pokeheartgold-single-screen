	.include "asm/macros.inc"
	.include "global.inc"

	.extern PM_SetLEDPattern
	.extern WMSP_CopyParentParam
	.extern WMSP_SetAllParams
	.extern WMSP_SetChildMaxSize
	.extern WMSP_SetParentMaxSize
	.extern WMSP_WL_DevClass1
	.extern WMSP_WL_MlmePowerManagement
	.extern WMSP_WL_MlmeStart
	.extern WMSP_WL_ParamSetMaxConnectableChild
	.extern WmspError_060033F4
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text

	arm_func_start WMSP_StartParent
WMSP_StartParent: ; 0x06003168
	stmdb sp!, {r4, r5, r6, lr}
	sub sp, sp, #0x2b8
	ldr r1, _060033EC ; =wmspW + 0x1000
	add r5, sp, #0xb8
	ldr r4, [r1, #0x550]
	ldrh r1, [r4]
	cmp r1, #2
	bne _06003194
	ldr r1, [r4, #0xc8]
	tst r1, #1
	beq _060031B8
_06003194:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #8
	strh r1, [r0]
	mov r1, #3
	strh r1, [r0, #2]
	mov r1, #0
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _060033E0
_060031B8:
	add r1, r4, #0x100
	ldrh r2, [r1, #0x1a]
	mov r3, #1
	ldrh ip, [r1, #0xf6]
	mov r2, r3, lsl r2
	ldr r6, [r0, #4]
	tst ip, r2, asr #1
	bne _060031FC
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #8
	strh r1, [r0]
	mov r1, #6
	strh r1, [r0, #2]
	mov r1, #0
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _060033E0
_060031FC:
	strh r3, [r4, #0xe6]
	mov r0, #0
	strh r0, [r1, #0x88]
	bl __VENEER_OS_DisableInterrupts
	add r1, r4, #0x100
	mov r2, #0
	strh r2, [r1, #0x82]
	strh r2, [r4, #0x86]
	bl __VENEER_OS_RestoreInterrupts
	mov r1, r5
	add r2, r4, #0x100
	mov r3, #1
	mov r0, #8
	strh r3, [r2, #0xee]
	bl WMSP_SetAllParams
	cmp r0, #0
	beq _060033E0
	mov r0, r5
	bl WMSP_WL_DevClass1
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06003260
	ldr r0, _060033F0 ; =0x00000303
	bl WmspError_060033F4
	b _060033E0
_06003260:
	cmp r6, #0
	movne r0, #1
	moveq r0, #0
	mov r0, r0, lsl #0x10
	mov r6, r0, lsr #0x10
	mov r0, r5
	mov r1, r6
	mov r2, #0
	mov r3, #1
	bl WMSP_WL_MlmePowerManagement
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _060032A0
	mov r0, #1
	bl WmspError_060033F4
	b _060033E0
_060032A0:
	strh r6, [r4, #0xc6]
	add r1, sp, #0x38
	mov r0, #0
	mov r2, #0x80
	add r6, r4, #0xe8
	bl __VENEER_MIi_CpuClear16
	add r0, sp, #0x38
	mov r1, r6
	bl WMSP_CopyParentParam
	add r1, sp, #0x18
	mov r0, #0
	mov r2, #0x20
	bl __VENEER_MIi_CpuClear16
	ldr r1, [r6, #8]
	mov r0, r5
	strh r1, [sp, #0x18]
	ldr r2, [r6, #8]
	mov r3, #0
	mov r2, r2, lsr #0x10
	strh r2, [sp, #0x1a]
	ldrh r5, [r6, #0xc]
	mov r2, #2
	strh r5, [sp, #0x1c]
	strh r3, [sp, #0x1e]
	str r2, [sp]
	ldrh r3, [r6, #0x32]
	mov r2, #3
	str r3, [sp, #4]
	str r2, [sp, #8]
	str r2, [sp, #0xc]
	ldrh r2, [r6, #4]
	add r3, sp, #0x38
	add r2, r2, #0x10
	mov r2, r2, lsl #0x10
	mov r2, r2, lsr #0x10
	str r2, [sp, #0x10]
	str r3, [sp, #0x14]
	ldrh r3, [r6, #0x18]
	mov r1, #0x20
	add r2, sp, #0x18
	bl WMSP_WL_MlmeStart
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _0600335C
	mov r0, #9
	bl WmspError_060033F4
	b _060033E0
_0600335C:
	ldrh r0, [r6, #0x14]
	cmp r0, #0
	ldrh r0, [r6, #0x34]
	movne r1, #0x2a
	moveq r1, #0
	add r0, r0, r1
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl WMSP_SetParentMaxSize
	ldrh r0, [r6, #0x14]
	cmp r0, #0
	movne r1, #6
	ldrh r0, [r6, #0x36]
	moveq r1, #0
	add r0, r0, r1
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl WMSP_SetChildMaxSize
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #7
	strh r1, [r4]
	mov r1, #8
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	strh r1, [r0, #8]
	ldrh r1, [r4, #0x30]
	strh r1, [r0, #0x2c]
	ldrh r1, [r4, #0x32]
	strh r1, [r0, #0x2e]
	bl __VENEER_WMSP_ReturnResult2Wm9
	mov r0, #1
	strh r0, [r4, #0xc2]
_060033E0:
	add sp, sp, #0x2b8
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	.align 2, 0
_060033EC: .word wmspW + 0x1000
_060033F0: .word 0x00000303
	arm_func_end WMSP_StartParent
