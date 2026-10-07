	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_SetAllParams
	.extern WMSP_WL_DevClass1
	.extern WMSP_WL_DevGetStationState
	.extern WMSP_WL_DevIdle
	.extern WMSP_WL_MlmeMeasureChannel
	.extern WMSP_WL_MlmePowerManagement
	.extern WmspError_06005BB8
	.extern __VENEER_MI_CpuFill8
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text

	arm_func_start WMSP_MeasureChannel
WMSP_MeasureChannel: ; 0x06005A10
	stmdb sp!, {r4, r5, r6, r7, r8, sb, lr}
	sub sp, sp, #0x214
	ldr r1, _06005BAC ; =wmspW + 0x1000
	mov r7, r0
	ldr r6, [r1, #0x550]
	add r4, sp, #0x14
	ldrh r0, [r6]
	cmp r0, #2
	beq _06005A50
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x1e
	strh r1, [r0]
	mov r1, #3
	strh r1, [r0, #2]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06005BA0
_06005A50:
	mov r0, r4
	bl WMSP_WL_DevGetStationState
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06005A70
	mov r0, #0x308
	bl WmspError_06005BB8
	b _06005BA0
_06005A70:
	ldrh r1, [r0, #6]
	mov r0, #2
	strh r0, [r6, #0xe6]
	cmp r1, #0x10
	bne _06005AF0
	mov r1, r4
	mov r0, #0xa
	bl WMSP_SetAllParams
	cmp r0, #0
	beq _06005BA0
	mov r0, r4
	bl WMSP_WL_DevClass1
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06005AB8
	ldr r0, _06005BB0 ; =0x00000303
	bl WmspError_06005BB8
	b _06005BA0
_06005AB8:
	mov r0, #3
	mov r1, #1
	strh r0, [r6]
	mov r0, r4
	mov r3, r1
	mov r2, #0
	bl WMSP_WL_MlmePowerManagement
	ldrh r1, [r0, #4]
	mov r0, #1
	cmp r1, #0
	beq _06005AEC
	bl WmspError_06005BB8
	b _06005BA0
_06005AEC:
	strh r0, [r6, #0xc6]
_06005AF0:
	ldrh r8, [r7, #2]
	ldrh sb, [r7, #4]
	ldrh r5, [r7, #6]
	ldrh r7, [r7, #8]
	add r0, sp, #4
	mov r1, #0
	mov r2, #0x10
	bl __VENEER_MI_CpuFill8
	strb r5, [sp, #4]
	add r0, sp, #4
	str r0, [sp]
	mov r0, r4
	mov r1, r8
	mov r2, sb
	mov r3, r7
	bl WMSP_WL_MlmeMeasureChannel
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06005B48
	mov r0, #0xa
	bl WmspError_06005BB8
	b _06005BA0
_06005B48:
	ldrh r1, [r0, #8]
	mov r0, r4
	and r4, r1, #0xff
	mov r1, r1, lsl #8
	mov r5, r1, lsr #0x10
	bl WMSP_WL_DevIdle
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06005B78
	ldr r0, _06005BB4 ; =0x00000302
	bl WmspError_06005BB8
	b _06005BA0
_06005B78:
	mov r0, #2
	strh r0, [r6]
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x1e
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	strh r4, [r0, #8]
	strh r5, [r0, #0xa]
	bl __VENEER_WMSP_ReturnResult2Wm9
_06005BA0:
	add sp, sp, #0x214
	ldmia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bx lr
	.align 2, 0
_06005BAC: .word wmspW + 0x1000
_06005BB0: .word 0x00000303
_06005BB4: .word 0x00000302
	arm_func_end WMSP_MeasureChannel
