	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_AddRssiToRandomPool
	.extern WMSP_GetLinkLevel
	.extern WMSP_GetRssi8_06003988
	.extern WMSP_SetAllParams
	.extern WMSP_WL_DevClass1
	.extern WMSP_WL_DevGetStationState
	.extern WMSP_WL_MlmePowerManagement
	.extern WMSP_WL_MlmeScan
	.extern WMSP_WL_ParamSetPreambleType
	.extern WMSP_WL_ParamSetSsidMask
	.extern WmspError_06003ED4
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MI_CpuFill8
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern _u32_div_f
	.extern wmspW

.text

	arm_func_start WMSP_StartScanEx
WMSP_StartScanEx: ; 0x060039B8
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0xc8
	sub sp, sp, #0x400
	ldr r1, _06003EB8 ; =wmspW + 0x1000
	mov sb, r0
	ldr r8, [r1, #0x550]
	ldrh r0, [r8]
	cmp r0, #2
	cmpne r0, #3
	cmpne r0, #5
	beq _06003A08
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x26
	strh r1, [r0]
	mov r1, #3
	strh r1, [r0, #2]
	mov r1, #4
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06003EA8
_06003A08:
	ldr r0, [sb, #4]
	add r1, sp, #0x18
	str r0, [r8, #0x184]
	ldrh sl, [sb, #2]
	add r0, r8, #0x100
	strh sl, [r0, #0x90]
	ldrh r3, [sb, #0xa]
	add r0, sb, #0xc
	str r3, [sp, #0x10]
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	ldrh r5, [sb, #0x12]
	ldrh fp, [sb, #0x36]
	cmp r5, #2
	beq _06003A50
	cmp r5, #3
	beq _06003A5C
	b _06003A68
_06003A50:
	mov r6, #1
	mov r5, #0
	b _06003A6C
_06003A5C:
	mov r6, #1
	mov r5, r6
	b _06003A6C
_06003A68:
	mov r6, #0
_06003A6C:
	add r1, sp, #0x4e
	add r0, sb, #0x16
	mov r2, #0x20
	ldrh r4, [sb, #0x14]
	bl __VENEER_MI_CpuCopy8
	ldrh r1, [sp, #0x18]
	ldr r0, _06003EBC ; =0x0000FFFF
	ldrh r7, [sb, #8]
	cmp r1, r0
	beq _06003AA0
	tst r1, #1
	bicne r0, r1, #1
	strneh r0, [sp, #0x18]
_06003AA0:
	add r0, r8, #0x100
	ldrh r1, [r0, #0xf4]
	mov r0, sl, lsl #0x11
	ands sl, r1, r0, lsr #16
	beq _06003AE4
	ldr r0, [r8, #0xc8]
	tst r0, #1
	cmpne r5, #1
	bne _06003AE4
	ldr r0, [sb, #4]
	cmp r0, #0
	beq _06003AE4
	tst r0, #3
	bne _06003AE4
	ldrh r0, [sb, #8]
	cmp r0, #0x40
	bhs _06003B08
_06003AE4:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x26
	strh r1, [r0]
	mov r1, #6
	strh r1, [r0, #2]
	mov r1, #4
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06003EA8
_06003B08:
	mov r0, #2
	strh r0, [r8, #0xe6]
	add r0, sp, #0x70
	bl WMSP_WL_DevGetStationState
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06003B34
	mov r0, #0x308
	mov r2, #1
	bl WmspError_06003ED4
	b _06003EA8
_06003B34:
	ldrh r0, [r0, #6]
	cmp r0, #0x10
	bne _06003BB4
	add r1, sp, #0x70
	mov r0, #0x26
	bl WMSP_SetAllParams
	cmp r0, #0
	beq _06003EA8
	add r0, sp, #0x70
	bl WMSP_WL_DevClass1
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06003B78
	ldr r0, _06003EC0 ; =0x00000303
	mov r2, #1
	bl WmspError_06003ED4
	b _06003EA8
_06003B78:
	mov r0, #3
	mov r1, #1
	strh r0, [r8]
	add r0, sp, #0x70
	mov r3, r1
	mov r2, #0
	bl WMSP_WL_MlmePowerManagement
	ldrh r1, [r0, #4]
	mov r0, #1
	cmp r1, #0
	beq _06003BB0
	mov r2, r0
	bl WmspError_06003ED4
	b _06003EA8
_06003BB0:
	strh r0, [r8, #0xc6]
_06003BB4:
	add r0, r8, #0x100
	ldrh r0, [r0, #0xee]
	cmp r5, #0
	bne _06003C04
	cmp r0, #1
	bne _06003C40
	add r0, sp, #0x70
	mov r1, #0
	bl WMSP_WL_ParamSetPreambleType
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06003BF4
	ldr r0, _06003EC4 ; =0x0000020E
	mov r2, #1
	bl WmspError_06003ED4
	b _06003EA8
_06003BF4:
	add r0, r8, #0x100
	mov r1, #0
	strh r1, [r0, #0xee]
	b _06003C40
_06003C04:
	cmp r0, #0
	bne _06003C40
	add r0, sp, #0x70
	mov r1, #1
	bl WMSP_WL_ParamSetPreambleType
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06003C34
	ldr r0, _06003EC4 ; =0x0000020E
	mov r2, #1
	bl WmspError_06003ED4
	b _06003EA8
_06003C34:
	add r0, r8, #0x100
	mov r1, #1
	strh r1, [r0, #0xee]
_06003C40:
	cmp r6, #1
	bne _06003C98
	add r0, sp, #0x2e
	mov r1, #0xff
	mov r2, #0x20
	bl __VENEER_MI_CpuFill8
	cmp fp, #0x20
	bhi _06003C70
	add r0, sp, #0x2e
	mov r2, fp
	mov r1, #0
	bl __VENEER_MI_CpuFill8
_06003C70:
	add r1, sp, #0x2e
	add r0, sp, #0x70
	bl WMSP_WL_ParamSetSsidMask
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06003C98
	ldr r0, _06003EC8 ; =0x0000020D
	mov r2, #1
	bl WmspError_06003ED4
	b _06003EA8
_06003C98:
	mov r6, #0
	mov r3, #5
	add r0, sp, #0x1e
	mov r1, r6
	mov r2, #0x10
	strh r3, [r8]
	bl __VENEER_MI_CpuFill8
	mov r3, #1
	add r1, sp, #0x1e
	mov r2, r3
_06003CC0:
	tst sl, r2, lsl r3
	addne r0, r6, #1
	movne r0, r0, lsl #0x10
	strneb r3, [r1, r6]
	movne r6, r0, lsr #0x10
	add r0, r3, #1
	mov r0, r0, lsl #0x10
	mov r3, r0, lsr #0x10
	cmp r3, #0xf
	blo _06003CC0
	sub r0, r7, #0x40
	mov r1, #0x42
	bl __VENEER__u32_div_f
	add r2, sp, #0x4e
	stmia sp, {r2, r5}
	add r1, sp, #0x1e
	str r1, [sp, #8]
	ldr r1, [sp, #0x10]
	sub r5, r7, r0, lsl #1
	str r1, [sp, #0xc]
	add r2, sp, #0x18
	add r0, sp, #0x70
	mov r3, r4
	add r1, r5, #0x5e
	bl WMSP_WL_MlmeScan
	mov r5, r0
	ldrh r1, [r5, #4]
	cmp r1, #0
	beq _06003D44
	mov r0, #2
	mov r2, #1
	bl WmspError_06003ED4
	b _06003EA8
_06003D44:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	ldrh r1, [r5, #8]
	mov r6, r0
	cmp r1, #0
	bne _06003D80
	mov r0, #0x26
	strh r0, [r6]
	mov r1, #0
	strh r1, [r6, #2]
	mov r0, #4
	strh r0, [r6, #8]
	strh r1, [r6, #0xe]
	mov r0, sl, asr #1
	strh r0, [r6, #0xa]
	b _06003EA0
_06003D80:
	ldr sb, [r8, #0x184]
	mov r2, r7
	mov r1, sb
	mov r0, #0
	add r8, r5, #0xa
	bl __VENEER_MIi_CpuClear16
	mov r7, #0
	b _06003E6C
_06003DA0:
	ldrh r1, [r8]
	mov r0, r8
	mov r1, r1, lsl #0x11
	mov fp, r1, lsr #0x10
	mov r1, sb
	mov r2, fp
	bl __VENEER_MI_CpuCopy8
	cmp r4, #0
	beq _06003E28
	ldrh r2, [sb, #0xa]
	cmp r2, #0
	moveq r0, #0
	beq _06003E0C
	cmp r2, #0x20
	movhi r0, #0
	bhi _06003E0C
	mov r1, #0
	b _06003E00
_06003DE8:
	add r0, sb, r1
	ldrb r0, [r0, #0xc]
	cmp r0, #0
	movne r0, #1
	bne _06003E0C
	add r1, r1, #1
_06003E00:
	cmp r1, r2
	blt _06003DE8
	mov r0, #0
_06003E0C:
	cmp r0, #0
	bne _06003E28
	add r0, sp, #0x4e
	add r1, sb, #0xc
	mov r2, #0x20
	strh r4, [sb, #0xa]
	bl __VENEER_MI_CpuCopy8
_06003E28:
	add r0, r6, r7, lsl #2
	str sb, [r0, #0x10]
	ldrh r0, [r8, #2]
	and r0, r0, #0xff
	bl WMSP_GetRssi8_06003988
	str r0, [sp, #0x14]
	bl WMSP_GetLinkLevel
	add r1, r6, r7, lsl #1
	strh r0, [r1, #0x50]
	ldr r0, [sp, #0x14]
	bl WMSP_AddRssiToRandomPool
	add sb, sb, fp
	tst sb, #2
	addne r0, sb, #2
	add r8, r8, fp
	bicne sb, r0, #3
	add r7, r7, #1
_06003E6C:
	ldrh r0, [r5, #8]
	cmp r7, r0
	blt _06003DA0
	mov r0, #0x26
	strh r0, [r6]
	mov r0, #0
	strh r0, [r6, #2]
	mov r0, #5
	strh r0, [r6, #8]
	ldrh r1, [r5, #8]
	mov r0, sl, asr #1
	strh r1, [r6, #0xe]
	strh r0, [r6, #0xa]
_06003EA0:
	mov r0, r6
	bl __VENEER_WMSP_ReturnResult2Wm9
_06003EA8:
	add sp, sp, #0xc8
	add sp, sp, #0x400
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06003EB8: .word wmspW + 0x1000
_06003EBC: .word 0x0000FFFF
_06003EC0: .word 0x00000303
_06003EC4: .word 0x0000020E
_06003EC8: .word 0x0000020D
	arm_func_end WMSP_StartScanEx

	arm_func_start __VENEER__u32_div_f
__VENEER__u32_div_f: ; 0x06003ECC
	ldr pc, _06003ED0 ; =_u32_div_f
	.align 2, 0
_06003ED0: .word _u32_div_f
	arm_func_end __VENEER__u32_div_f
