	.include "asm/macros.inc"
	.include "global.inc"

	.extern WMSP_CancelVAlarm
	.extern WMSP_CleanSendQueue
	.extern WMSP_IndicateDisconnectionFromMyself
	.extern WMSP_ResetSizeVars
	.extern WMSP_SetThreadPriorityLow
	.extern WMSP_WL_DevIdle
	.extern WMSP_WL_MlmeDeAuthenticate
	.extern WMSP_WL_MlmeReset
	.extern WmspError_06004CB4
	.extern WmspIndError
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MI_CpuFill8
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspW

.text

	arm_func_start WMSP_DisconnectCore
WMSP_DisconnectCore: ; 0x06004664
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x220
	ldr r3, _06004BE8 ; =wmspW + 0x1000
	ldr r4, [r0, #4]
	movs sl, r1
	ldrne r0, [r0, #8]
	mov r1, r4, lsl #0x10
	moveq r0, #0
	mov r0, r0, lsl #0x10
	mov fp, #0
	ldr r7, [r3, #0x550]
	mov r8, r1, lsr #0x10
	ldrh r1, [r7]
	mov r0, r0, lsr #0x10
	cmp r1, #9
	str r2, [sp]
	mov r5, fp
	str r0, [sp, #8]
	cmpne r1, #7
	bne _060046C4
	ldr r0, [r7, #0xc]
	cmp r0, #1
	moveq r5, #1
	b _060047BC
_060046C4:
	cmp r1, #0xa
	cmpne r1, #8
	bne _06004780
	bl __VENEER_OS_DisableInterrupts
	add r1, r7, #0x100
	ldrh r1, [r1, #0x82]
	mov r4, r0
	cmp r1, #0
	bne _06004728
	bl __VENEER_OS_RestoreInterrupts
	cmp sl, #0
	bne _06004720
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xd
	strh r1, [r0]
	mov r1, #3
	strh r1, [r0, #2]
	mov r1, #0
	strh r1, [r0, #4]
	strh r1, [r0, #6]
	strh r8, [r0, #8]
	strh r1, [r0, #0xa]
	bl __VENEER_WMSP_ReturnResult2Wm9
_06004720:
	mov r0, #0
	b _06004BDC
_06004728:
	ldr r0, [r7, #0xc]
	cmp r0, #1
	bne _06004758
	mov r0, #0
	str r0, [r7, #0xc]
	mov r5, #1
	bl WMSP_CancelVAlarm
	bl WMSP_SetThreadPriorityLow
	ldrh r0, [r7]
	cmp r0, #0xa
	moveq r0, #8
	streqh r0, [r7]
_06004758:
	add r0, r7, #0x100
	mov r1, #0
	strh r1, [r0, #0x82]
	strh r1, [r7, #0x86]
	str r1, [r7, #0x14]
	str r1, [r7, #0x10]
	mov r0, r4
	str r1, [r7, #0x1c]
	bl __VENEER_OS_RestoreInterrupts
	b _060047BC
_06004780:
	cmp sl, #0
	bne _060047B4
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xd
	strh r1, [r0]
	mov r1, #3
	strh r1, [r0, #2]
	mov r1, fp
	strh r1, [r0, #4]
	strh r1, [r0, #6]
	strh r8, [r0, #8]
	strh r1, [r0, #0xa]
	bl __VENEER_WMSP_ReturnResult2Wm9
_060047B4:
	mov r0, #0
	b _06004BDC
_060047BC:
	ldrh r0, [r7]
	cmp r0, #0xa
	cmpne r0, #8
	bne _060049E4
	add r0, r7, #0x8a
	add r1, sp, #0x1a
	add r0, r0, #0x100
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	mov sb, #0
	add r6, sp, #0x1a
	mov r4, #3
	b _06004874
_060047F0:
	add r0, sp, #0x20
	mov r1, r6
	mov r2, r4
	bl WMSP_WL_MlmeDeAuthenticate
	ldrh r1, [r0, #4]
	cmp r1, #7
	bgt _0600482C
	bge _06004834
	cmp r1, #1
	bgt _0600483C
	cmp r1, #0
	blt _0600483C
	cmpne r1, #1
	beq _0600487C
	b _0600483C
_0600482C:
	cmp r1, #0xc
	bne _0600483C
_06004834:
	add sb, sb, #1
	b _06004874
_0600483C:
	cmp sl, #0
	mov r2, r8
	mov r0, #5
	mov r3, #0
	beq _06004858
	bl WmspIndError
	b _0600485C
_06004858:
	bl WmspError_06004CB4
_0600485C:
	cmp r5, #0
	beq _0600486C
	mov r0, #1
	bl WMSP_CleanSendQueue
_0600486C:
	mov r0, #0
	b _06004BDC
_06004874:
	cmp sb, #2
	blt _060047F0
_0600487C:
	mov r0, #0
	strh r0, [r7, #0xc2]
	mov r0, #3
	mov fp, #1
	strh r0, [r7]
	add r0, sp, #0x20
	mov r1, fp
	bl WMSP_WL_MlmeReset
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _060048E0
	cmp sl, #0
	mov r2, r8
	mov r0, #0
	mov r3, fp
	beq _060048C4
	bl WmspIndError
	b _060048C8
_060048C4:
	bl WmspError_06004CB4
_060048C8:
	cmp r5, #0
	beq _060048D8
	mov r0, #1
	bl WMSP_CleanSendQueue
_060048D8:
	mov r0, #0
	b _06004BDC
_060048E0:
	add r0, sp, #0x20
	bl WMSP_WL_DevIdle
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06004930
	ldr r0, _06004BEC ; =0x00000302
	cmp sl, #0
	mov r2, r8
	beq _06004910
	mov r3, fp
	bl WmspIndError
	b _06004918
_06004910:
	mov r3, fp
	bl WmspError_06004CB4
_06004918:
	cmp r5, #0
	beq _06004928
	mov r0, #1
	bl WMSP_CleanSendQueue
_06004928:
	mov r0, #0
	b _06004BDC
_06004930:
	mov r0, #2
	strh r0, [r7]
	mov r1, #0
	str r1, [r7, #0x198]
	add r3, r7, #0x100
	add r0, r7, #0x19c
	mov r2, #0x50
	strh r1, [r3, #0x96]
	bl __VENEER_MI_CpuFill8
	bl WMSP_ResetSizeVars
	cmp sl, #1
	bne _060049C0
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r4, r0
	mov r0, #0xc
	strh r0, [r4]
	mov r0, #0
	strh r0, [r4, #2]
	mov r0, #9
	strh r0, [r4, #8]
	ldr r0, [sp, #8]
	mov r2, #6
	strh r0, [r4, #0xc]
	add r0, r7, #0x100
	ldrh r1, [r0, #0x88]
	add r0, sp, #0x1a
	strh r1, [r4, #0xa]
	add r1, r4, #0x10
	bl __VENEER_MI_CpuCopy8
	ldrh r1, [r7, #0x30]
	mov r0, r4
	strh r1, [r4, #0x16]
	ldrh r1, [r7, #0x32]
	strh r1, [r4, #0x18]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _060049D0
_060049C0:
	mov r0, #0
	add r2, sp, #0x1a
	mov r1, r0
	bl WMSP_IndicateDisconnectionFromMyself
_060049D0:
	cmp r5, #0
	beq _06004BCC
	mov r0, #1
	bl WMSP_CleanSendQueue
	b _06004BCC
_060049E4:
	mov r6, #1
	b _06004BC4
_060049EC:
	add r0, r7, #0x100
	ldrh r0, [r0, #0x82]
	mov r1, #1
	mov sb, r1, lsl r6
	and r0, r0, r8
	tst sb, r0
	beq _06004BC0
	sub r1, r6, #1
	mov r2, #6
	mul r0, r1, r2
	str r0, [sp, #0x10]
	add r0, r7, #0x128
	str r0, [sp, #0xc]
	mov r0, r6, lsl #0x10
	mov r0, r0, lsr #0x10
	str r0, [sp, #4]
	ldr r1, [sp, #0xc]
	ldr r0, [sp, #0x10]
	add r0, r1, r0
	add r1, sp, #0x14
	bl __VENEER_MI_CpuCopy8
	mov r4, #0
	b _06004AB0
_06004A48:
	add r0, sp, #0x20
	add r1, sp, #0x14
	mov r2, #3
	bl WMSP_WL_MlmeDeAuthenticate
	ldrh r1, [r0, #4]
	cmp r1, #0
	beq _06004AB8
	cmp r1, #7
	cmpne r1, #0xc
	bne _06004A78
	add r4, r4, #1
	b _06004AB0
_06004A78:
	cmp sl, #0
	mov r2, r8
	mov r0, #5
	mov r3, fp
	beq _06004A94
	bl WmspIndError
	b _06004A98
_06004A94:
	bl WmspError_06004CB4
_06004A98:
	cmp r5, #0
	beq _06004AA8
	mov r0, #1
	bl WMSP_CleanSendQueue
_06004AA8:
	mov r0, #0
	b _06004BDC
_06004AB0:
	cmp r4, #2
	blt _06004A48
_06004AB8:
	bl __VENEER_OS_DisableInterrupts
	mov r4, r0
	add r1, r7, #0x100
	ldrh r3, [r1, #0x82]
	tst r3, sb
	beq _06004BBC
	ldr r2, [sp, #0xc]
	ldr r0, [sp, #0x10]
	mov ip, #1
	add r0, r2, r0
	ldr r2, [sp, #4]
	orr r2, fp, ip, lsl r2
	mov r2, r2, lsl #0x10
	mov fp, r2, lsr #0x10
	mvn r2, sb
	and r3, r3, r2
	strh r3, [r1, #0x82]
	ldrh r3, [r7, #0x86]
	mov r1, #0
	and r2, r3, r2
	strh r2, [r7, #0x86]
	ldr r2, [sp, #4]
	add r2, r7, r2, lsl #3
	str r1, [r2, #0x738]
	str r1, [r2, #0x73c]
	mov r2, #6
	bl __VENEER_MI_CpuFill8
	mov r0, r4
	bl __VENEER_OS_RestoreInterrupts
	cmp sl, #1
	bne _06004B90
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r4, r0
	mov r0, #8
	strh r0, [r4]
	mov r0, #0
	strh r0, [r4, #2]
	mov r0, #9
	strh r0, [r4, #8]
	ldr r0, [sp, #8]
	add r1, r4, #0xa
	strh r0, [r4, #0x12]
	ldr r0, [sp, #4]
	mov r2, #6
	strh r0, [r4, #0x10]
	add r0, sp, #0x14
	bl __VENEER_MI_CpuCopy8
	ldrh r1, [r7, #0x30]
	mov r0, r4
	strh r1, [r4, #0x2c]
	ldrh r1, [r7, #0x32]
	strh r1, [r4, #0x2e]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06004BA4
_06004B90:
	mov r0, r6, lsl #0x10
	mov r1, r0, lsr #0x10
	add r2, sp, #0x14
	mov r0, #1
	bl WMSP_IndicateDisconnectionFromMyself
_06004BA4:
	cmp r5, #0
	beq _06004BC0
	mov r0, sb, lsl #0x10
	mov r0, r0, lsr #0x10
	bl WMSP_CleanSendQueue
	b _06004BC0
_06004BBC:
	bl __VENEER_OS_RestoreInterrupts
_06004BC0:
	add r6, r6, #1
_06004BC4:
	cmp r6, #0x10
	blt _060049EC
_06004BCC:
	ldr r0, [sp]
	cmp r0, #0
	strneh fp, [r0]
	mov r0, #1
_06004BDC:
	add sp, sp, #0x220
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06004BE8: .word wmspW + 0x1000
_06004BEC: .word 0x00000302
	arm_func_end WMSP_DisconnectCore
