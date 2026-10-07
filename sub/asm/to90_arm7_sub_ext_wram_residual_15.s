	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_FillRssiIntoList
	.extern WMSP_GetLinkLevel
	.extern WMSP_SetAllParams
	.extern WMSP_SetChildMaxSize
	.extern WMSP_SetParentMaxSize
	.extern WMSP_WL_DevClass1
	.extern WMSP_WL_MlmeAssociate
	.extern WMSP_WL_MlmeAuthenticate
	.extern WMSP_WL_MlmeJoin
	.extern WMSP_WL_MlmePowerManagement
	.extern WMSP_WL_ParamSetBeaconLostThreshold
	.extern WMSP_WL_ParamSetNullKeyResponseMode
	.extern WmspError_060045D0
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_GetTick
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern _s32_div_f
	.extern wmspW

.text

	arm_func_start WMSP_StartConnectEx
WMSP_StartConnectEx: ; 0x06004024
	stmdb sp!, {r4, r5, r6, r7, r8, lr}
	sub sp, sp, #0x250
	ldr r1, _060045B4 ; =wmspW + 0x1000
	add r4, sp, #0x50
	ldr r7, [r1, #0x550]
	ldr r5, [r1, #0x54c]
	ldrh r1, [r7]
	mov r6, r0
	cmp r1, #2
	bne _06004058
	ldr r0, [r7, #0xc8]
	tst r0, #1
	beq _0600407C
_06004058:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xc
	strh r1, [r0]
	mov r1, #3
	strh r1, [r0, #2]
	mov r1, #6
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _060045A8
_0600407C:
	ldr r0, [r6, #4]
	add r1, r5, #0x10
	mov r2, #0xc0
	bl __VENEER_MI_CpuCopy8
	ldrh r0, [r5, #0x4c]
	cmp r0, #0x10
	blo _060040C8
	ldrb r0, [r5, #0x5b]
	tst r0, #1
	bne _060040C8
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xc
	strh r1, [r0]
	mov r1, #0xb
	strh r1, [r0, #2]
	mov r1, #6
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _060045A8
_060040C8:
	ldrh r1, [r5, #0x46]
	add r0, r7, #0x100
	mov r2, #1
	ldrh r0, [r0, #0xf4]
	mov r1, r2, lsl r1
	tst r1, r0
	beq _060040F0
	rsb r0, r2, #0x2000
	tst r0, r1, asr #1
	bne _06004110
_060040F0:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xc
	strh r1, [r0]
	mov r1, #6
	strh r1, [r0, #2]
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _060045A8
_06004110:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xc
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #6
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	add r0, r7, #0x100
	ldrh r1, [r0, #0xec]
	cmp r1, #1
	ldrh r1, [r5, #0x3e]
	bne _06004158
	tst r1, #1
	movne r1, #1
	strneh r1, [r0, #0xec]
	moveq r1, #2
	b _06004168
_06004158:
	tst r1, #2
	movne r1, #2
	strneh r1, [r0, #0xec]
	moveq r1, #1
_06004168:
	streqh r1, [r0, #0xec]
	ldrh r0, [r5, #0x3c]
	tst r0, #0x20
	movne r1, #1
	add r0, r7, #0x100
	moveq r1, #0
	strh r1, [r0, #0xee]
	ldrh r0, [r5, #0x4c]
	mov r1, r4
	cmp r0, #0
	moveq r0, #3
	movne r0, #2
	strh r0, [r7, #0xe6]
	mov r0, #0xc
	bl WMSP_SetAllParams
	cmp r0, #0
	beq _060045A8
	mov r0, r4
	mov r1, #0
	bl WMSP_WL_ParamSetNullKeyResponseMode
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _060041D4
	ldr r0, _060045B8 ; =0x00000216
	mov r2, #0
	bl WmspError_060045D0
	b _060045A8
_060041D4:
	ldrh r0, [r5, #0x4c]
	cmp r0, #0x10
	bhs _06004230
	ldrh r1, [r5, #0x42]
	cmp r1, #0
	moveq r0, #1
	beq _060041FC
	ldr r0, _060045BC ; =0x00002710
	bl __VENEER__s32_div_f
	add r0, r0, #1
_060041FC:
	mov r0, r0, lsl #0x10
	mov r1, r0, lsr #0x10
	cmp r1, #0xff
	movhi r1, #0xff
	mov r0, r4
	bl WMSP_WL_ParamSetBeaconLostThreshold
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06004230
	ldr r0, _060045C0 ; =0x0000020B
	mov r2, #0
	bl WmspError_060045D0
	b _060045A8
_06004230:
	mov r0, r4
	bl WMSP_WL_DevClass1
	ldrh r1, [r0, #4]
	mov r2, #0
	cmp r1, #0
	beq _06004254
	ldr r0, _060045C4 ; =0x00000303
	bl WmspError_060045D0
	b _060045A8
_06004254:
	mov r0, #3
	strh r0, [r7]
	ldr r0, [r6, #0x20]
	mov r3, #1
	cmp r0, #0
	movne r0, #1
	moveq r0, #0
	mov r0, r0, lsl #0x10
	mov r8, r0, lsr #0x10
	mov r0, r4
	mov r1, r8
	bl WMSP_WL_MlmePowerManagement
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _060042A0
	mov r0, #1
	mov r2, #0
	bl WmspError_060045D0
	b _060045A8
_060042A0:
	strh r8, [r7, #0xc6]
	add r1, sp, #0xc
	add r0, r5, #0x10
	mov r2, #0x40
	bl __VENEER_MI_CpuCopy8
	ldrh r0, [r7, #0xe6]
	cmp r0, #2
	bne _060042FC
	mov r0, #0x20
	strh r0, [sp, #0x16]
	ldr r0, [r5, #0x54]
	add r1, sp, #0x20
	strh r0, [sp, #0x18]
	ldr r2, [r5, #0x54]
	add r0, r6, #8
	mov r2, r2, lsr #0x10
	strh r2, [sp, #0x1a]
	ldrh ip, [r5, #0x58]
	mov r3, #0
	mov r2, #0x18
	strh ip, [sp, #0x1c]
	strh r3, [sp, #0x1e]
	bl __VENEER_MI_CpuCopy8
_060042FC:
	add r2, sp, #0xc
	mov r0, r4
	mov r1, #0x7d0
	bl WMSP_WL_MlmeJoin
	ldrh r1, [r0, #4]
	cmp r1, #0
	ldreqh r2, [r0, #6]
	cmpeq r2, #0
	beq _06004330
	ldrh r2, [r0, #6]
	mov r0, #3
	bl WmspError_060045D0
	b _060045A8
_06004330:
	add r1, r7, #0x8a
	add r0, r0, #8
	add r1, r1, #0x100
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	add r0, r7, #0x8a
	add r1, sp, #6
	add r0, r0, #0x100
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	ldrh r2, [r6, #0x26]
	add r1, sp, #6
	mov r0, r4
	mov r3, #0x7d0
	bl WMSP_WL_MlmeAuthenticate
	ldrh r1, [r0, #4]
	cmp r1, #0xc
	ldreqh r2, [r0, #6]
	cmpeq r2, #0x13
	bne _060043A0
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xc
	strh r1, [r0]
	strh r1, [r0, #2]
	mov r1, #6
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _060045A8
_060043A0:
	cmp r1, #0
	ldreqh r2, [r0, #6]
	cmpeq r2, #0
	beq _060043C0
	ldrh r2, [r0, #6]
	mov r0, #4
	bl WmspError_060045D0
	b _060045A8
_060043C0:
	add r0, r7, #0x8a
	add r1, sp, #0
	add r0, r0, #0x100
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	add r1, sp, #0
	mov r0, r4
	mov r2, #1
	mov r3, #0x7d0
	bl WMSP_WL_MlmeAssociate
	mov r4, r0
	bl __VENEER_OS_DisableInterrupts
	ldrh r2, [r4, #4]
	mov r6, r0
	cmp r2, #0xc
	ldreqh r1, [r4, #6]
	cmpeq r1, #0x13
	bne _0600442C
	bl __VENEER_OS_RestoreInterrupts
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xc
	strh r1, [r0]
	strh r1, [r0, #2]
	mov r1, #6
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _060045A8
_0600442C:
	cmp r2, #0
	ldreqh r0, [r4, #6]
	cmpeq r0, #0
	beq _06004458
	mov r0, r6
	bl __VENEER_OS_RestoreInterrupts
	ldrh r1, [r4, #4]
	ldrh r2, [r4, #6]
	mov r0, #6
	bl WmspError_060045D0
	b _060045A8
_06004458:
	ldrh r1, [r4, #8]
	add r0, r7, #0x100
	strh r1, [r0, #0x88]
	ldrh r0, [r5, #0x58]
	add r1, r7, #0x1f8
	strh r0, [r7, #0xba]
	mov r0, #1
	mov r2, #0x10
	bl __VENEER_MIi_CpuClear16
	ldrh r0, [r5, #0x12]
	and r0, r0, #0xff
	tst r0, #2
	mov r0, r0, asr #2
	addeq r0, r0, #0x19
	and r4, r0, #0xff
	mov r0, r4
	bl WMSP_GetLinkLevel
	strh r0, [r7, #0xbc]
	mov r0, r4
	bl WMSP_FillRssiIntoList
	bl __VENEER_OS_DisableInterrupts
	add r1, r7, #0x100
	mov r2, #1
	strh r2, [r1, #0x82]
	strh r2, [r7, #0x86]
	ldr r1, [r7, #0x7bc]
	ldr r2, [r7, #0x7b8]
	cmp r1, #0
	mov r4, r0
	cmpeq r2, #0
	beq _060044E8
	bl __VENEER_OS_GetTick
	orr r0, r0, #1
	str r0, [r7, #0x738]
	orr r0, r1, #0
	str r0, [r7, #0x73c]
_060044E8:
	mov r0, #8
	strh r0, [r7]
	ldrb r0, [r5, #0x5b]
	tst r0, #4
	movne r1, #0x2a
	ldrh r0, [r5, #0x5c]
	moveq r1, #0
	add r0, r0, r1
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl WMSP_SetParentMaxSize
	ldrb r0, [r5, #0x5b]
	tst r0, #4
	movne r1, #6
	ldrh r0, [r5, #0x5e]
	moveq r1, #0
	add r0, r0, r1
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl WMSP_SetChildMaxSize
	mov r0, r4
	bl __VENEER_OS_RestoreInterrupts
	mov r0, #1
	strh r0, [r7, #0xc2]
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r4, r0
	mov r0, #0xc
	strh r0, [r4]
	mov r0, #0
	strh r0, [r4, #2]
	mov r0, #7
	strh r0, [r4, #8]
	add r0, r7, #0x100
	ldrh r2, [r0, #0x88]
	add r1, r7, #0x8a
	strh r2, [r4, #0xa]
	add r0, r1, #0x100
	add r1, r4, #0x10
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	ldrh r1, [r7, #0x30]
	mov r0, r4
	strh r1, [r4, #0x16]
	ldrh r1, [r7, #0x32]
	strh r1, [r4, #0x18]
	bl __VENEER_WMSP_ReturnResult2Wm9
	mov r0, r6
	bl __VENEER_OS_RestoreInterrupts
_060045A8:
	add sp, sp, #0x250
	ldmia sp!, {r4, r5, r6, r7, r8, lr}
	bx lr
	.align 2, 0
_060045B4: .word wmspW + 0x1000
_060045B8: .word 0x00000216
_060045BC: .word 0x00002710
_060045C0: .word 0x0000020B
_060045C4: .word 0x00000303
	arm_func_end WMSP_StartConnectEx

	arm_func_start __VENEER__s32_div_f
__VENEER__s32_div_f: ; 0x060045C8
	ldr pc, _060045CC ; =_s32_div_f
	.align 2, 0
_060045CC: .word _s32_div_f
	arm_func_end __VENEER__s32_div_f
