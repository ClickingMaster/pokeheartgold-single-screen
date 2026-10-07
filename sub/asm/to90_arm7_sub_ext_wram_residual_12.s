	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_CancelVAlarm
	.extern WMSP_CleanSendQueue
	.extern WMSP_IndicateDisconnectionFromMyself
	.extern WMSP_ResetSizeVars
	.extern WMSP_SetThreadPriorityLow
	.extern WMSP_WL_DevGetStationState
	.extern WMSP_WL_DevIdle
	.extern WMSP_WL_DevTestSignal
	.extern WMSP_WL_MlmeDeAuthenticate
	.extern WMSP_WL_MlmeReset
	.extern WMSP_WL_ParamGetMode
	.extern WMSP_WL_ParamSetPreambleType
	.extern WmspError_06002FC0
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MI_CpuFill8
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text

	arm_func_start WMSP_Reset
WMSP_Reset: ; 0x06002BE0
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x210
	ldr r0, _06002FAC ; =wmspW + 0x1000
	mov r4, #0
	ldr r8, [r0, #0x550]
	bl __VENEER_OS_DisableInterrupts
	ldr r1, [r8, #0xc]
	mov r5, r0
	cmp r1, #1
	bne _06002C3C
	mov r0, r4
	str r0, [r8, #0xc]
	mov r4, #1
	bl WMSP_CancelVAlarm
	bl WMSP_SetThreadPriorityLow
	ldrh r0, [r8]
	cmp r0, #0xa
	moveq r0, #8
	streqh r0, [r8]
	beq _06002C3C
	cmp r0, #9
	moveq r0, #7
	streqh r0, [r8]
_06002C3C:
	ldrh r1, [r8]
	add r0, r1, #0xf9
	add r0, r0, #0xff00
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	cmp r0, #1
	bhi _06002C70
	add r0, r8, #0x100
	cmp r1, #7
	moveq sl, #1
	ldrh sb, [r0, #0x82]
	movne sl, #0
	b _06002C74
_06002C70:
	mov sb, #0
_06002C74:
	add r0, r8, #0x100
	mov r1, #0
	strh r1, [r0, #0x82]
	strh r1, [r8, #0x86]
	str r1, [r8, #0x14]
	str r1, [r8, #0x10]
	str r1, [r8, #0x1c]
	mov r0, r5
	strh r1, [r8, #0xc2]
	bl __VENEER_OS_RestoreInterrupts
	cmp r4, #0
	beq _06002CAC
	ldr r0, _06002FB0 ; =0x0000FFFF
	bl WMSP_CleanSendQueue
_06002CAC:
	cmp sl, #0
	movne r0, #0
	strneh r0, [r8, #0xf6]
	cmp sb, #0
	beq _06002D08
	mov r7, #0
	add r5, r8, #0x128
	add r4, r8, #0x8a
	mov r6, #1
	mov fp, #6
_06002CD4:
	tst sb, r6, lsl r7
	beq _06002CFC
	cmp r7, #0
	addeq r2, r4, #0x100
	subne r0, r7, #1
	mlane r2, r0, fp, r5
	mov r1, r7, lsl #0x10
	mov r0, sl
	mov r1, r1, lsr #0x10
	bl WMSP_IndicateDisconnectionFromMyself
_06002CFC:
	add r7, r7, #1
	cmp r7, #0x10
	blt _06002CD4
_06002D08:
	add r0, r8, #0x128
	mov r1, #0
	mov r2, #0x5a
	bl __VENEER_MI_CpuFill8
	add r0, sp, #0x10
	bl WMSP_WL_DevGetStationState
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06002D38
	mov r0, #0x308
	bl WmspError_06002FC0
	b _06002FA0
_06002D38:
	ldrh r4, [r0, #6]
	add r0, sp, #0x10
	bl WMSP_WL_ParamGetMode
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06002D5C
	mov r0, #0x284
	bl WmspError_06002FC0
	b _06002FA0
_06002D5C:
	ldrh r1, [r0, #6]
	cmp r4, #0x20
	bgt _06002DA0
	bge _06002E74
	cmp r4, #0
	bgt _06002D7C
	beq _06002E98
	b _06002F78
_06002D7C:
	cmp r4, #0x12
	bgt _06002F78
	cmp r4, #0x10
	blt _06002F78
	beq _06002EB8
	cmp r4, #0x11
	cmpne r4, #0x12
	beq _06002F10
	b _06002F78
_06002DA0:
	cmp r4, #0x30
	bgt _06002DB0
	beq _06002DB8
	b _06002F78
_06002DB0:
	cmp r4, #0x40
	bne _06002F78
_06002DB8:
	add r0, r1, #0xfe
	add r0, r0, #0xff00
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	cmp r0, #1
	bhi _06002E3C
	add r0, r8, #0x8a
	add r1, sp, #0xa
	add r0, r0, #0x100
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	mov r6, #0
	add r5, sp, #0xa
	mov r4, #3
	b _06002E30
_06002DF4:
	add r0, sp, #0x10
	mov r1, r5
	mov r2, r4
	bl WMSP_WL_MlmeDeAuthenticate
	ldrh r0, [r0, #4]
	cmp r0, #0
	beq _06002E20
	cmp r0, #7
	cmpne r0, #0xc
	beq _06002E2C
	b _06002E74
_06002E20:
	mov r0, #3
	strh r0, [r8]
	b _06002E74
_06002E2C:
	add r6, r6, #1
_06002E30:
	cmp r6, #2
	blt _06002DF4
	b _06002E74
_06002E3C:
	cmp r1, #1
	bne _06002E74
	add r0, sp, #4
	mov r1, #0xff
	mov r2, #6
	bl __VENEER_MI_CpuFill8
	add r1, sp, #4
	add r0, sp, #0x10
	mov r2, #3
	bl WMSP_WL_MlmeDeAuthenticate
	ldrh r0, [r0, #4]
	cmp r0, #0
	moveq r0, #3
	streqh r0, [r8]
_06002E74:
	add r0, sp, #0x10
	mov r1, #1
	bl WMSP_WL_MlmeReset
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06002E98
	mov r0, #0
	bl WmspError_06002FC0
	b _06002FA0
_06002E98:
	add r0, sp, #0x10
	bl WMSP_WL_DevIdle
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06002EB8
	ldr r0, _06002FB4 ; =0x00000302
	bl WmspError_06002FC0
	b _06002FA0
_06002EB8:
	add r0, r8, #0x100
	ldrh r0, [r0, #0xee]
	cmp r0, #0
	bne _06002EF8
	add r0, sp, #0x10
	mov r1, #1
	bl WMSP_WL_ParamSetPreambleType
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06002EEC
	ldr r0, _06002FB8 ; =0x0000020E
	bl WmspError_06002FC0
	b _06002FA0
_06002EEC:
	add r0, r8, #0x100
	mov r1, #1
	strh r1, [r0, #0xee]
_06002EF8:
	mov r0, #2
	strh r0, [r8]
	mov r0, #0
	str r0, [r8, #0x198]
	bl WMSP_ResetSizeVars
	b _06002F88
_06002F10:
	cmp r1, #0
	bne _06002F4C
	mov r0, #1
	mov r1, #0
	str r0, [sp]
	add r0, sp, #0x10
	mov r2, r1
	mov r3, #0x14
	bl WMSP_WL_DevTestSignal
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06002F4C
	ldr r0, _06002FBC ; =0x00000309
	bl WmspError_06002FC0
	b _06002FA0
_06002F4C:
	add r0, sp, #0x10
	bl WMSP_WL_DevIdle
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06002F6C
	ldr r0, _06002FB4 ; =0x00000302
	bl WmspError_06002FC0
	b _06002FA0
_06002F6C:
	mov r0, #2
	strh r0, [r8]
	b _06002F88
_06002F78:
	mov r0, #0x308
	mov r1, #0
	bl WmspError_06002FC0
	b _06002FA0
_06002F88:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #1
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	bl __VENEER_WMSP_ReturnResult2Wm9
_06002FA0:
	add sp, sp, #0x210
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06002FAC: .word wmspW + 0x1000
_06002FB0: .word 0x0000FFFF
_06002FB4: .word 0x00000302
_06002FB8: .word 0x0000020E
_06002FBC: .word 0x00000309
	arm_func_end WMSP_Reset
