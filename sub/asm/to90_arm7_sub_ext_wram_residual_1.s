	.include "asm/macros.inc"
	.include "global.inc"

	.extern MI_CpuFill8
	.extern MIi_CpuCopy16
	.extern OS_CancelAlarm
	.extern OS_ExitThread
	.extern OS_GetTick
	.extern OS_SetAlarm
	.extern WMSP_AddRssiToList
	.extern WMSP_CancelVAlarm
	.extern WMSP_CheckMacAddress
	.extern WMSP_CleanSendQueue
	.extern WMSP_FlushSendQueue
	.extern WMSP_GetAverageLinkLevel
	.extern WMSP_GetInternalRequestBuf
	.extern WMSP_GetRssi8_06001B5C
	.extern WMSP_ParsePortPacket
	.extern WMSP_RequestResumeMP
	.extern WMSP_ResetSizeVars
	.extern WMSP_SetChildSize
	.extern WMSP_SetThreadPriorityLow
	.extern WmspFreeBufOfWL
	.extern WmspKickMPChild
	.extern WmspKickMPParent
	.extern WmspMPChildIntervalAlarmCallback
	.extern WmspMPParentIntervalAlarmCallback
	.extern WmspMaMultiPollAckAlarmCallback
	.extern __VENEER_MI_CpuCopy8
	.extern __VENEER_MIi_CpuClear16
	.extern __VENEER_OS_DisableInterrupts
	.extern __VENEER_OS_ReceiveMessage
	.extern __VENEER_OS_RestoreInterrupts
	.extern __VENEER_OS_SendMessage
	.extern __VENEER_WMSP_GetBuffer4Callback2Wm9
	.extern __VENEER_WMSP_ReturnResult2Wm9
	.extern wmspMPAckAlarm
	.extern wmspMPIntervalAlarm
	.extern wmspW

.text

	arm_func_start WMSP_IndicateThread
WMSP_IndicateThread: ; 0x06000958
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x28
_06000960:
	ldr r0, _060017D4 ; =wmspW
	add r1, sp, #0x24
	add r0, r0, #0x28
	mov r2, #1
	bl __VENEER_OS_ReceiveMessage
	ldr sb, [sp, #0x24]
	cmp sb, #0
	bne _06000988
	bl __VENEER_OS_ExitThread
	b _06001AD4
_06000988:
	ldrh r1, [sb, #0xc]
	and r0, r1, #0xff00
	mov r0, r0, lsl #0x10
	movs r0, r0, lsr #0x10
	and r1, r1, #0xff
	cmpne r0, #0x100
	bne _06001ABC
	tst r1, #0x80
	beq _06001ABC
	ldr r0, _060017D8 ; =wmspW + 0x1000
	ldr r1, [r0, #0x54c]
	cmp r1, #0
	ldrne r8, [r0, #0x550]
	ldrneh r3, [r8]
	cmpne r3, #1
	beq _06000960
	ldrh r2, [sb, #0xc]
	ldr r1, _060017DC ; =0x00000182
	cmp r2, r1
	bgt _06000A24
	bge _060010DC
	cmp r2, #0x8d
	bgt _06000A18
	subs r0, r2, #0x84
	addpl pc, pc, r0, lsl #2
	b _06001AB0
_060009F0: ; jump table
	b _06000A50 ; case 0
	b _06000A74 ; case 1
	b _06000D00 ; case 2
	b _06000E64 ; case 3
	b _06000E88 ; case 4
	b _06001AB0 ; case 5
	b _06001AB0 ; case 6
	b _06000EAC ; case 7
	b _06000EDC ; case 8
	b _06000F0C ; case 9
_06000A18:
	cmp r2, #0x180
	beq _06001024
	b _06001AB0
_06000A24:
	add r0, r1, #3
	cmp r2, r0
	bgt _06000A40
	bge _06001754
	cmp r2, #0x184
	beq _060013D4
	b _06001AB0
_06000A40:
	add r0, r1, #4
	cmp r2, r0
	beq _060019B8
	b _06001AB0
_06000A50:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x80
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #0x13
	strh r1, [r0, #4]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_06000A74:
	cmp r3, #7
	cmpne r3, #9
	bne _06000BFC
	add r1, sp, #0x1c
	add r0, sb, #0x10
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	mov r6, #0
	mov r5, r6
	add r4, r8, #0x100
	mov sl, #1
	b _06000B74
_06000AA4:
	bl __VENEER_OS_DisableInterrupts
	ldrh r2, [r4, #0x82]
	add r1, r5, #1
	mov r7, r0
	tst r2, sl, lsl r1
	beq _06000B68
	mov r0, #6
	mul r2, r5, r0
	add r0, r8, r2
	ldrb fp, [sp, #0x1c]
	ldrb r3, [r0, #0x128]
	cmp fp, r3
	ldreqb fp, [sp, #0x1d]
	ldreqb r3, [r0, #0x129]
	cmpeq fp, r3
	ldreqb fp, [sp, #0x1e]
	ldreqb r3, [r0, #0x12a]
	cmpeq fp, r3
	ldreqb fp, [sp, #0x1f]
	ldreqb r3, [r0, #0x12b]
	cmpeq fp, r3
	ldreqb fp, [sp, #0x20]
	ldreqb r3, [r0, #0x12c]
	cmpeq fp, r3
	ldreqb r3, [sp, #0x21]
	ldreqb r0, [r0, #0x12d]
	cmpeq r3, r0
	bne _06000B68
	add r3, r8, #0x100
	mov r0, r1, lsl #0x10
	mov r6, r0, lsr #0x10
	ldrh r0, [r3, #0x82]
	mvn r1, sl, lsl r6
	and r0, r0, r1
	strh r0, [r3, #0x82]
	ldrh r0, [r8, #0x86]
	add r3, r8, r6, lsl #3
	and r1, r0, r1
	add r0, r8, #0x128
	strh r1, [r8, #0x86]
	mov r1, #0
	str r1, [r3, #0x738]
	add r0, r0, r2
	mov r2, #6
	str r1, [r3, #0x73c]
	bl __VENEER_MI_CpuFill8
	mov r0, r7
	bl __VENEER_OS_RestoreInterrupts
	b _06000B7C
_06000B68:
	mov r0, r7
	bl __VENEER_OS_RestoreInterrupts
	add r5, r5, #1
_06000B74:
	cmp r5, #0xf
	blt _06000AA4
_06000B7C:
	cmp r6, #0
	beq _06001AB0
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r4, r0
	mov r0, #8
	strh r0, [r4]
	mov r0, #0
	strh r0, [r4, #2]
	mov r0, #9
	strh r0, [r4, #8]
	ldrh r1, [sb, #0x16]
	add r0, sb, #0x10
	strh r1, [r4, #0x12]
	strh r6, [r4, #0x10]
	add r1, r4, #0xa
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	ldrh r1, [r8, #0x30]
	mov r0, r4
	strh r1, [r4, #0x2c]
	ldrh r1, [r8, #0x32]
	strh r1, [r4, #0x2e]
	bl __VENEER_WMSP_ReturnResult2Wm9
	ldr r0, [r8, #0xc]
	cmp r0, #1
	bne _06001AB0
	mov r0, #1
	mov r0, r0, lsl r6
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	bl WMSP_CleanSendQueue
	b _06001AB0
_06000BFC:
	mov r5, #0
	bl __VENEER_OS_DisableInterrupts
	add r1, r8, #0x100
	ldrh r1, [r1, #0x82]
	mov r4, r0
	cmp r1, #0
	bne _06000C20
	bl __VENEER_OS_RestoreInterrupts
	b _06001AB0
_06000C20:
	ldr r0, [r8, #0xc]
	cmp r0, #1
	bne _06000C40
	mov r0, r5
	str r0, [r8, #0xc]
	mov r5, #1
	bl WMSP_CancelVAlarm
	bl WMSP_SetThreadPriorityLow
_06000C40:
	add r0, r8, #0x100
	mov r1, #0
	strh r1, [r0, #0x82]
	strh r1, [r8, #0x86]
	str r1, [r8, #0x14]
	str r1, [r8, #0x10]
	str r1, [r8, #0x1c]
	str r1, [r8, #0x198]
	strh r1, [r0, #0x96]
	add r0, r8, #0x19c
	mov r2, #0x50
	bl __VENEER_MI_CpuFill8
	bl WMSP_ResetSizeVars
	mov r0, #0
	strh r0, [r8, #0xc2]
	mov r1, #3
	mov r0, r4
	strh r1, [r8]
	bl __VENEER_OS_RestoreInterrupts
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r4, r0
	mov r0, #0xc
	strh r0, [r4]
	mov r0, #0
	strh r0, [r4, #2]
	mov r0, #9
	strh r0, [r4, #8]
	ldrh r1, [sb, #0x16]
	add r0, r8, #0x100
	strh r1, [r4, #0xc]
	ldrh r1, [r0, #0x88]
	add r0, r8, #0x8a
	strh r1, [r4, #0xa]
	add r0, r0, #0x100
	add r1, r4, #0x10
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	ldrh r1, [r8, #0x30]
	mov r0, r4
	strh r1, [r4, #0x16]
	ldrh r1, [r8, #0x32]
	strh r1, [r4, #0x18]
	bl __VENEER_WMSP_ReturnResult2Wm9
	cmp r5, #0
	beq _06001AB0
	mov r0, #1
	bl WMSP_CleanSendQueue
	b _06001AB0
_06000D00:
	ldrh r5, [sb, #0x16]
	cmp r5, #0
	beq _06001AB0
	cmp r5, #0x10
	bhs _06001AB0
	ldrh r0, [r8, #0xf6]
	cmp r0, #0
	bne _06000D8C
	bl WMSP_GetInternalRequestBuf
	movs r4, r0
	moveq r0, #0
	beq _06000D58
	mov r3, #0x22
	add r0, sb, #0x10
	add r1, r4, #4
	mov r2, #6
	str r3, [r4]
	bl __VENEER_MI_CpuCopy8
	ldr r0, _060017E0 ; =wmspW + 0x88
	mov r1, r4
	mov r2, #0
	bl __VENEER_OS_SendMessage
_06000D58:
	cmp r0, #0
	bne _06001AB0
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x80
	strh r1, [r0]
	mov r1, #8
	strh r1, [r0, #2]
	mov r1, #0x16
	strh r1, [r0, #4]
	mov r1, #0x22
	strh r1, [r0, #6]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_06000D8C:
	bl __VENEER_OS_DisableInterrupts
	add r1, r8, #0x100
	mov r4, #1
	ldrh r3, [r1, #0x82]
	mvn r2, r4, lsl r5
	orr r3, r3, r4, lsl r5
	strh r3, [r1, #0x82]
	ldrh r1, [r8, #0x86]
	mov r4, r0
	and r0, r1, r2
	strh r0, [r8, #0x86]
	bl __VENEER_OS_GetTick
	orr r2, r0, #1
	add r0, r8, r5, lsl #3
	add r3, r8, #0x128
	str r2, [r0, #0x738]
	orr r1, r1, #0
	str r1, [r0, #0x73c]
	sub r0, r5, #1
	mov r2, #6
	mla r1, r0, r2, r3
	add r0, sb, #0x10
	bl __VENEER_MI_CpuCopy8
	mov r0, r4
	bl __VENEER_OS_RestoreInterrupts
	mov r0, #1
	add r1, r8, #0x1f8
	add r1, r1, r5, lsl #4
	mov r2, #0x10
	bl __VENEER_MIi_CpuClear16
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r4, r0
	mov r0, #8
	strh r0, [r4]
	mov r0, #0
	strh r0, [r4, #2]
	mov r0, #7
	strh r0, [r4, #8]
	add r0, sb, #0x10
	add r1, r4, #0xa
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	strh r5, [r4, #0x10]
	add r0, sb, #0x22
	add r1, r4, #0x14
	mov r2, #0x18
	bl __VENEER_MIi_CpuCopy16
	ldrh r1, [r8, #0x30]
	mov r0, r4
	strh r1, [r4, #0x2c]
	ldrh r1, [r8, #0x32]
	strh r1, [r4, #0x2e]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_06000E64:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x80
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #0x12
	strh r1, [r0, #4]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_06000E88:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x80
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #0x11
	strh r1, [r0, #4]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_06000EAC:
	ldrh r0, [r8, #0xc2]
	cmp r0, #0
	beq _06001AB0
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xc
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #8
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_06000EDC:
	ldrh r0, [r8, #0xc2]
	cmp r0, #0
	beq _06001AB0
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #8
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #2
	strh r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_06000F0C:
	ldrb r0, [sb, #0x1f]
	bl WMSP_GetRssi8_06001B5C
	ldr r2, _060017E4 ; =0x027FFF98
	ldrh r1, [r2]
	eor r0, r0, r1, lsl #1
	eor r0, r0, r0, lsr #16
	strh r0, [r2]
	ldrh r0, [r8]
	cmp r0, #8
	cmpne r0, #0xa
	bne _06001AB0
	ldrh r1, [r8, #0xba]
	ldrh r0, [sb, #0x44]
	cmp r1, r0
	beq _06000FB0
	bl WMSP_GetInternalRequestBuf
	movs r1, r0
	moveq r0, #0
	beq _06000F7C
	mov r0, #0x25
	str r0, [r1]
	mov r0, #1
	ldr r3, _060017E8 ; =0x00008002
	str r0, [r1, #4]
	ldr r0, _060017E0 ; =wmspW + 0x88
	mov r2, #0
	str r3, [r1, #8]
	bl __VENEER_OS_SendMessage
_06000F7C:
	cmp r0, #0
	bne _06001AB0
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x80
	strh r1, [r0]
	mov r1, #8
	strh r1, [r0, #2]
	mov r1, #0x16
	strh r1, [r0, #4]
	mov r1, #0x25
	strh r1, [r0, #6]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_06000FB0:
	ldr r0, _060017D8 ; =wmspW + 0x1000
	ldr r0, [r0, #0x550]
	ldrh r0, [r0, #0xc2]
	cmp r0, #0
	beq _06001AB0
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r4, r0
	mov r0, #0x80
	strh r0, [r4]
	mov r0, #0
	strh r0, [r4, #2]
	mov r0, #0x10
	strh r0, [r4, #4]
	ldrh r0, [sb, #0x44]
	strh r0, [r4, #6]
	ldrh r0, [r8]
	strh r0, [r4, #8]
	ldrh r0, [sb, #0x16]
	strh r0, [r4, #0xa]
	cmp r0, #0x80
	bhi _06001018
	add r2, r0, #1
	add r0, sb, #0x3c
	add r1, r4, #0xc
	bic r2, r2, #1
	bl __VENEER_MIi_CpuCopy16
_06001018:
	mov r0, r4
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_06001024:
	ldr r0, [r8, #0x10]
	cmp r0, #0
	beq _06001AB0
	ldrb r0, [sb, #0x1f]
	bl WMSP_GetRssi8_06001B5C
	bl WMSP_AddRssiToList
	bl WMSP_GetAverageLinkLevel
	strh r0, [r8, #0xbc]
	add r0, sb, #0x2e
	bl WMSP_CheckMacAddress
	cmp r0, #1
	beq _06001AB0
	ldrh r1, [sb, #0x16]
	ldr r0, _060017EC ; =0x000005E4
	cmp r1, r0
	bhi _06001AB0
	ldrh r1, [r8, #0xae]
	add r0, sb, #0x10
	eor r1, r1, #1
	strh r1, [r8, #0xae]
	ldrh r2, [r8, #0xae]
	ldrh r1, [sb, #0x16]
	add r2, r8, r2, lsl #2
	ldr r4, [r2, #0xb0]
	add r2, r1, #0x2d
	mov r1, r4
	bic r2, r2, #1
	bl __VENEER_MI_CpuCopy8
	add r0, sb, #0x28
	add r1, r4, #0x18
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	add r0, sb, #0x2e
	add r1, r4, #0x1e
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x11
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #0xf
	strh r1, [r0, #4]
	str r4, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_060010DC:
	ldrb r0, [sb, #0x1f]
	bl WMSP_GetRssi8_06001B5C
	ldrh r1, [r8, #0xbe]
	cmp r1, r0
	strhih r0, [r8, #0xbe]
	ldr r0, [r8, #0xc]
	cmp r0, #0
	beq _06001AB0
	ldrh r0, [r8, #0x60]
	cmp r0, #1
	moveq r0, #0
	streqh r0, [r8, #0x60]
	ldrh r0, [r8, #0x70]
	ldrh sl, [r8, #0x8e]
	eor r0, r0, #1
	strh r0, [r8, #0x70]
	ldrh r0, [r8, #0x70]
	ldrh r1, [sb, #0x16]
	add r3, r8, r0, lsl #2
	ldrh r0, [r8, #0x72]
	add r2, r1, #0x30
	ldr r4, [r3, #0x74]
	cmp r0, r2
	movlo r2, r0
	mov r1, r4
	add r0, sb, #0x10
	bl __VENEER_MI_CpuCopy8
	bl __VENEER_OS_DisableInterrupts
	ldrh r1, [r8, #0x84]
	mov fp, r0
	cmp r1, #1
	mov r5, #0
	bne _0600116C
	ldr r0, _060017F0 ; =wmspMPAckAlarm
	mov r5, #1
	bl __VENEER_OS_CancelAlarm
_0600116C:
	mov r7, #1
	strh r7, [r8, #0x84]
	ldrh r0, [r4, #0xa]
	ldr r2, _060017F4 ; =0x000082EA
	strh r0, [r8, #0x82]
	ldrh r6, [sb, #0x18]
	mov r0, #0
	tst r6, #0x2000
	moveq r7, #0
	strh r7, [r8, #0x90]
	str r0, [sp]
	ldrh r1, [r4, #0xa]
	ldrh r0, [r4, #0xc]
	sub r0, r1, r0
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	add r0, r0, #0x80
	mov r3, r0, lsl #4
	umull r1, r0, r3, r2
	mov r3, r3, asr #0x1f
	mla r0, r3, r2, r0
	mov r1, r1, lsr #6
	orr r1, r1, r0, lsl #26
	mov r3, r0, lsr #6
	mov r1, r1, lsr #0xa
	ldr r0, _060017F0 ; =wmspMPAckAlarm
	mov r2, r3, lsr #0xa
	orr r1, r1, r3, lsl #22
	ldr r3, _060017F8 ; =WmspMaMultiPollAckAlarmCallback
	bl __VENEER_OS_SetAlarm
	and r0, r6, #0x2800
	cmp r0, #0x2800
	moveq r1, #1
	and r0, r6, #0x6000
	movne r1, #0
	cmp r0, #0x6000
	moveq r2, #1
	movne r2, #0
	cmp r2, #0
	movne r0, #0
	strneh r0, [r8, #0x8a]
	cmp r1, #0
	movne r0, #1
	moveq r0, #0
	strh r0, [r8, #0x8e]
	cmp r2, #0
	movne r0, #1
	moveq r0, #0
	strh r0, [r8, #0x8c]
	cmp r7, #0
	beq _06001274
	ldrh r0, [r4, #0x2c]
	sub r1, r0, #0x66
	mov r0, r1, asr #1
	add r0, r1, r0, lsr #30
	mov r0, r0, asr #2
	subs r1, r0, #0x20
	bmi _06001274
	ldrh r0, [r8, #0x32]
	cmp r1, #0x200
	movgt r1, #0x200
	cmp r1, r0
	beq _06001274
	mov r0, r1, lsl #0x10
	mov r0, r0, lsr #0x10
	bl WMSP_SetChildSize
_06001274:
	mov r0, fp
	bl __VENEER_OS_RestoreInterrupts
	cmp r5, #0
	beq _060012C0
	cmp sl, #1
	bne _06001298
	mov r0, r5
	mov r1, #0
	bl WMSP_FlushSendQueue
_06001298:
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xe
	strh r1, [r0]
	mov r1, #9
	strh r1, [r0, #2]
	mov r1, #0xd
	strh r1, [r0, #4]
	mov r1, #0
	str r1, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
_060012C0:
	cmp r7, #0
	bne _060012EC
	ldrh r0, [r4, #6]
	cmp r0, #2
	blo _06001AB0
	ldrh r0, [r4, #0x30]
	tst r0, #0x8000
	movne r0, #1
	moveq r0, #0
	strh r0, [r8, #0x5e]
	b _06001AB0
_060012EC:
	add r0, sb, #0x28
	add r1, r4, #0x18
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	add r0, sb, #0x2e
	add r1, r4, #0x1e
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	ldrh r0, [r4, #6]
	cmp r0, #2
	blo _0600137C
	sub r0, r0, #2
	strh r0, [r4, #6]
	ldrh r0, [r4, #0x30]
	tst r0, #0x8000
	movne r0, #1
	moveq r0, #0
	strh r0, [r8, #0x5e]
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xe
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #0xc
	strh r1, [r0, #4]
	str r4, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	ldrh r3, [r4, #6]
	cmp r3, #0
	beq _060013A8
	str r4, [sp]
	ldrh r1, [r4, #0x30]
	add r2, r4, #0x32
	mov r0, #0
	bl WMSP_ParsePortPacket
	b _060013A8
_0600137C:
	mov r0, #0
	strh r0, [r4, #6]
	strh r0, [r8, #0x5e]
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xe
	strh r1, [r0]
	strh r1, [r0, #2]
	mov r1, #0xc
	strh r1, [r0, #4]
	str r4, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
_060013A8:
	ldr r0, [r8, #0x7bc]
	ldr r1, [r8, #0x7b8]
	cmp r0, #0
	cmpeq r1, #0
	beq _06001AB0
	bl __VENEER_OS_GetTick
	orr r0, r0, #1
	str r0, [r8, #0x738]
	orr r0, r1, #0
	str r0, [r8, #0x73c]
	b _06001AB0
_060013D4:
	ldr r1, [r8, #0xc]
	mov r0, #0
	str r0, [sp, #0x10]
	cmp r1, #0
	beq _06001AB0
	ldrh r0, [sb, #0x12]
	cmp r0, #0
	bne _0600140C
	ldrh r1, [r8, #0x66]
	cmp r1, #1
	bne _06001414
	ldrh r0, [sb, #0x10]
	cmp r0, #0
	beq _06001414
_0600140C:
	bl WMSP_RequestResumeMP
	b _06001AB0
_06001414:
	cmp r1, #0
	movne r0, #0
	strneh r0, [r8, #0x66]
	ldrh r1, [sb, #0x16]
	ldrh r0, [sb, #0x14]
	ldrh r3, [r8, #0x70]
	mul r2, r1, r0
	add r1, r8, r3, lsl #2
	ldr r6, [r1, #0x74]
	ldrh r0, [r8, #0x72]
	add r2, r2, #0xa
	cmp r0, r2
	movlo r2, r0
	mov r1, r6
	add r0, sb, #0x10
	bl __VENEER_MI_CpuCopy8
	ldr r0, _060017D8 ; =wmspW + 0x1000
	ldrh r1, [r6]
	ldr r7, [r0, #0x550]
	cmp r1, #0
	ldrh r4, [r7, #0xbe]
	bne _060014B0
	ldrh r0, [sb, #0x14]
	cmp r0, #1
	blo _060014B0
	add sl, sb, #0x1a
	mov r5, #0
	b _060014A0
_06001484:
	ldrh r1, [sb, #0x16]
	mla r0, r1, r5, sl
	ldrb r0, [r0, #3]
	bl WMSP_GetRssi8_06001B5C
	cmp r0, r4
	movlo r4, r0
	add r5, r5, #1
_060014A0:
	ldrh r0, [sb, #0x14]
	cmp r5, r0
	blt _06001484
	strh r4, [r7, #0xbe]
_060014B0:
	bl __VENEER_OS_GetTick
	orr r0, r0, #1
	str r0, [sp, #8]
	ldrh fp, [r6]
	mov r0, #0
	str r0, [sp, #0x14]
	ldr r0, _060017FC ; =0x00008001
	orr r1, r1, #0
	rsb r0, r0, #0x18000
	mov r4, #0
	str r1, [sp, #0xc]
	add sl, r6, #0xa
	str r0, [sp, #0x18]
	mov r5, r4
	b _0600165C
_060014EC:
	ldrh r7, [sl, #4]
	ldrh r1, [sl]
	cmp r7, #1
	blo _06001640
	cmp r7, #0xf
	bhi _06001640
	cmp r1, #2
	blo _06001568
	ldr r0, [sp, #0x18]
	cmp r1, r0
	beq _06001568
	sub r0, r1, #2
	mov r0, r0, lsl #0x10
	movs r3, r0, lsr #0x10
	strh r3, [sl]
	ldrh r2, [r8, #0x86]
	mov r1, #1
	orr r1, r2, r1, lsl r7
	strh r1, [r8, #0x86]
	ldr r1, [sp, #8]
	add r0, r8, r7, lsl #3
	str r1, [r0, #0x738]
	ldr r1, [sp, #0xc]
	str r1, [r0, #0x73c]
	beq _06001640
	mov r0, r7
	str r6, [sp]
	ldrh r1, [sl, #8]
	add r2, sl, #0xa
	bl WMSP_ParsePortPacket
	b _06001640
_06001568:
	cmp r1, #0
	bne _06001640
	add r0, r8, r7, lsl #3
	ldr r2, [r0, #0x738]
	ldr r1, [r0, #0x73c]
	mov r0, #1
	mov r0, r0, lsl r7
	str r0, [sp, #4]
	mov r0, #1
	orr fp, fp, r0, lsl r7
	ldr ip, [r8, #0x7bc]
	ldr r0, [r8, #0x7b8]
	cmp ip, #0
	mov r3, #0
	cmpeq r0, r5
	beq _06001640
	cmp r1, r3
	cmpeq r2, r4
	beq _06001640
	ldr r3, [sp, #8]
	subs r3, r3, r2
	ldr r2, [sp, #0xc]
	sbc r1, r2, r1
	cmp r1, ip
	cmpeq r3, r0
	bls _06001640
	bl WMSP_GetInternalRequestBuf
	movs r1, r0
	add r2, r8, r7, lsl #3
	mov r0, #0
	str r0, [r2, #0x738]
	str r0, [r2, #0x73c]
	beq _06001610
	mov r0, #0x25
	str r0, [r1]
	ldr r0, [sp, #4]
	mov r2, #0
	str r0, [r1, #4]
	ldr r0, _060017FC ; =0x00008001
	str r0, [r1, #8]
	ldr r0, _060017E0 ; =wmspW + 0x88
	bl __VENEER_OS_SendMessage
_06001610:
	cmp r0, #0
	bne _06001640
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x80
	strh r1, [r0]
	mov r1, #8
	strh r1, [r0, #2]
	mov r1, #0x16
	strh r1, [r0, #4]
	mov r1, #0x25
	strh r1, [r0, #6]
	bl __VENEER_WMSP_ReturnResult2Wm9
_06001640:
	ldr r0, [sp, #0x14]
	add r0, r0, #1
	mov r0, r0, lsl #0x10
	mov r0, r0, lsr #0x10
	str r0, [sp, #0x14]
	ldrh r0, [r6, #6]
	add sl, sl, r0
_0600165C:
	ldrh r1, [r6, #4]
	ldr r0, [sp, #0x14]
	cmp r0, r1
	blo _060014EC
	mov r0, fp, lsl #0x10
	mov r1, r0, lsr #0x10
	mov r0, #0
	bl WMSP_FlushSendQueue
	ldrh r0, [r6]
	cmp r0, #0
	movne r0, #1
	strne r0, [sp, #0x10]
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0xe
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #0xb
	strh r1, [r0, #4]
	str r6, [r0, #8]
	bl __VENEER_WMSP_ReturnResult2Wm9
	ldrh r0, [r8, #0x70]
	eor r0, r0, #1
	strh r0, [r8, #0x70]
	bl __VENEER_OS_DisableInterrupts
	ldr r1, [sp, #0x10]
	cmp r1, #0
	ldreqsh r1, [r8, #0x62]
	subeq r1, r1, #1
	streqh r1, [r8, #0x62]
	ldrsh r1, [r8, #0x64]
	cmp r1, #0
	subgt r1, r1, #1
	strgth r1, [r8, #0x64]
	ldrsh r1, [r8, #0x62]
	cmp r1, #0
	ldrgtsh r1, [r8, #0x64]
	cmpgt r1, #0
	movgt r4, #1
	movle r4, #0
	bl __VENEER_OS_RestoreInterrupts
	cmp r4, #0
	beq _06001AB0
	ldr r0, [sp, #0x10]
	cmp r0, #1
	ldreqh r4, [sb, #0x10]
	ldrh r0, [r8, #0x44]
	ldrne r4, _06001800 ; =0x0000FFFF
	cmp r0, #0
	beq _06001748
	ldr r0, _06001804 ; =wmspMPIntervalAlarm
	bl __VENEER_OS_CancelAlarm
	str r4, [sp]
	ldr r1, [r8, #0x48]
	ldr r2, [r8, #0x4c]
	ldr r0, _06001804 ; =wmspMPIntervalAlarm
	ldr r3, _06001808 ; =WmspMPParentIntervalAlarmCallback
	bl __VENEER_OS_SetAlarm
	b _06001AB0
_06001748:
	mov r0, r4
	bl WmspKickMPParent
	b _06001AB0
_06001754:
	ldr r0, [r8, #0xc]
	mov r5, #0
	cmp r0, #0
	beq _06001AB0
	ldrh r0, [sb, #0xe]
	cmp r0, #0
	bne _060017B4
	ldr r1, _0600180C ; =0x048080F8
	mov r4, #1
	ldrh r0, [r1]
	ldrh r3, [r1, #2]
	ldrh r2, [r1]
	cmp r0, r2
	ldrhih r3, [r1, #2]
	mov r0, r2, asr #4
	orr r0, r0, r3, lsl #12
	ldrh r1, [r8, #0x82]
	mov r0, r0, lsl #0x10
	rsb r0, r1, r0, lsr #16
	mov r0, r0, lsl #0x10
	mov r0, r0, asr #0x10
	cmp r0, #0
	ble _06001AB0
	b _060017B8
_060017B4:
	mov r4, r5
_060017B8:
	bl __VENEER_OS_DisableInterrupts
	ldrh r1, [r8, #0x84]
	mov r7, r0
	cmp r1, #0
	bne _06001810
	bl __VENEER_OS_RestoreInterrupts
	b _06001AB0
	.align 2, 0
_060017D4: .word wmspW
_060017D8: .word wmspW + 0x1000
_060017DC: .word 0x00000182
_060017E0: .word wmspW + 0x88
_060017E4: .word 0x027FFF98
_060017E8: .word 0x00008002
_060017EC: .word 0x000005E4
_060017F0: .word wmspMPAckAlarm
_060017F4: .word 0x000082EA
_060017F8: .word WmspMaMultiPollAckAlarmCallback
_060017FC: .word 0x00008001
_06001800: .word 0x0000FFFF
_06001804: .word wmspMPIntervalAlarm
_06001808: .word WmspMPParentIntervalAlarmCallback
_0600180C: .word 0x048080F8
_06001810:
	ldr r0, _060017F0 ; =wmspMPAckAlarm
	mov r1, #0
	strh r1, [r8, #0x84]
	ldrh r6, [r8, #0x90]
	bl __VENEER_OS_CancelAlarm
	mov r0, r7
	bl __VENEER_OS_RestoreInterrupts
	ldrh r3, [r8, #0x8c]
	mov r2, #1
	cmp r3, #0
	beq _0600185C
	cmp r4, #0
	bne _06001858
	add r0, r8, #0x100
	ldrh r1, [sb, #0x3e]
	ldrh r0, [r0, #0x88]
	tst r1, r2, lsl r0
	bne _0600185C
_06001858:
	mov r2, #0
_0600185C:
	cmp r3, #0
	movne r0, #0
	strneh r0, [r8, #0x8c]
	ldrh r0, [r8, #0x8e]
	cmp r0, #0
	beq _06001898
	mov r0, #0
	strh r0, [r8, #0x8e]
	cmp r2, #0
	movne r0, #1
	mov r1, r0, lsl #0x10
	mov r0, r4
	mov r1, r1, lsr #0x10
	bl WMSP_FlushSendQueue
	mov r5, r0
_06001898:
	cmp r6, #0
	beq _06001AB0
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r7, r0
	mov r0, #0xe
	strh r0, [r7]
	cmp r4, #0
	movne r0, #9
	strneh r0, [r7, #2]
	bne _060018E4
	add r0, r8, #0x100
	ldrh r2, [sb, #0x3e]
	ldrh r0, [r0, #0x88]
	mov r1, #1
	tst r2, r1, lsl r0
	movne r0, #0xf
	strneh r0, [r7, #2]
	moveq r0, #0
	streqh r0, [r7, #2]
_060018E4:
	mov r0, #0xd
	strh r0, [r7, #4]
	mov r0, #0
	str r0, [r7, #8]
	cmp r4, #0
	bne _06001944
	ldrh r1, [sb, #0x1c]
	add r0, sb, #0x28
	strh r1, [r7, #0xc]
	ldrh r2, [sb, #0x1e]
	add r1, r7, #0x10
	strh r2, [r7, #0xe]
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	add r0, sb, #0x2e
	add r1, r7, #0x16
	mov r2, #6
	bl __VENEER_MI_CpuCopy8
	ldrh r0, [sb, #0x3a]
	strh r0, [r7, #0x1c]
	ldrh r0, [sb, #0x3c]
	strh r0, [r7, #0x1e]
	ldrh r0, [sb, #0x3e]
	strh r0, [r7, #0x20]
_06001944:
	mov r0, r7
	bl __VENEER_WMSP_ReturnResult2Wm9
	cmp r6, #0
	beq _06001AB0
	cmp r5, #1
	ldrneh r0, [r8, #0x5e]
	cmpne r0, #0
	bne _060019A0
	ldrh r0, [r8, #0x46]
	cmp r0, #0
	beq _06001998
	ldr r0, _06001804 ; =wmspMPIntervalAlarm
	bl __VENEER_OS_CancelAlarm
	mov r0, #0
	str r0, [sp]
	ldr r1, [r8, #0x50]
	ldr r2, [r8, #0x54]
	ldr r0, _06001804 ; =wmspMPIntervalAlarm
	ldr r3, _06001AE0 ; =WmspMPChildIntervalAlarmCallback
	bl __VENEER_OS_SetAlarm
	b _06001AB0
_06001998:
	bl WmspKickMPChild
	b _06001AB0
_060019A0:
	mov r1, #0
	strh r1, [r8, #0x5e]
	mov r0, #1
	strh r0, [r8, #0x60]
	strh r1, [r8, #0x88]
	b _06001AB0
_060019B8:
	add r0, r8, #0x700
	ldrh r0, [r0, #0xcc]
	cmp r0, #1
	ldreqh r0, [sb, #0x10]
	cmpeq r0, #0x20
	bne _06001A2C
	bl __VENEER_OS_DisableInterrupts
	mov r4, r0
	ldr r0, _060017F0 ; =wmspMPAckAlarm
	bl __VENEER_OS_CancelAlarm
	mov r1, #0
	mov r0, r4
	strh r1, [r8, #0x84]
	bl __VENEER_OS_RestoreInterrupts
	mov r1, #0
	strh r1, [r8, #0x8a]
	mov r0, #1
	bl WMSP_FlushSendQueue
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x80
	strh r1, [r0]
	mov r1, #0
	strh r1, [r0, #2]
	mov r1, #0x17
	strh r1, [r0, #4]
	mov r1, #1
	strh r1, [r0, #6]
	bl __VENEER_WMSP_ReturnResult2Wm9
	b _06001AB0
_06001A2C:
	bl WMSP_GetInternalRequestBuf
	movs r1, r0
	moveq r0, #0
	beq _06001A80
	ldr r0, _06001AE4 ; =0x00008003
	mov r2, #0x25
	str r2, [r1]
	str r0, [r1, #8]
	ldrh r0, [r8]
	cmp r0, #9
	cmpne r0, #7
	ldreq r0, _06001AE8 ; =0x00007FFE
	streq r0, [r1, #4]
	beq _06001A74
	cmp r0, #0xa
	cmpne r0, #8
	moveq r0, #1
	streq r0, [r1, #4]
_06001A74:
	ldr r0, _060017E0 ; =wmspW + 0x88
	mov r2, #0
	bl __VENEER_OS_SendMessage
_06001A80:
	cmp r0, #0
	bne _06001AB0
	bl __VENEER_WMSP_GetBuffer4Callback2Wm9
	mov r1, #0x80
	strh r1, [r0]
	mov r1, #8
	strh r1, [r0, #2]
	mov r1, #0x16
	strh r1, [r0, #4]
	mov r1, #0x25
	strh r1, [r0, #6]
	bl __VENEER_WMSP_ReturnResult2Wm9
_06001AB0:
	mov r0, sb
	bl WmspFreeBufOfWL
	b _06000960
_06001ABC:
	ldr r0, _060017D4 ; =wmspW
	mov r1, sb
	add r0, r0, #0x58
	mov r2, #1
	bl __VENEER_OS_SendMessage
	b _06000960
_06001AD4:
	add sp, sp, #0x28
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_06001AE0: .word WmspMPChildIntervalAlarmCallback
_06001AE4: .word 0x00008003
_06001AE8: .word 0x00007FFE
	arm_func_end WMSP_IndicateThread

	arm_func_start __VENEER_OS_ExitThread
__VENEER_OS_ExitThread: ; 0x06001AEC
	ldr pc, _06001AF0 ; =OS_ExitThread
	.align 2, 0
_06001AF0: .word OS_ExitThread
	arm_func_end __VENEER_OS_ExitThread

	arm_func_start __VENEER_MI_CpuFill8
__VENEER_MI_CpuFill8: ; 0x06001AF4
	ldr pc, _06001AF8 ; =MI_CpuFill8
	.align 2, 0
_06001AF8: .word MI_CpuFill8
	arm_func_end __VENEER_MI_CpuFill8

	arm_func_start __VENEER_OS_GetTick
__VENEER_OS_GetTick: ; 0x06001AFC
	ldr pc, _06001B00 ; =OS_GetTick
	.align 2, 0
_06001B00: .word OS_GetTick
	arm_func_end __VENEER_OS_GetTick

	arm_func_start __VENEER_MIi_CpuCopy16
__VENEER_MIi_CpuCopy16: ; 0x06001B04
	ldr pc, _06001B08 ; =MIi_CpuCopy16
	.align 2, 0
_06001B08: .word MIi_CpuCopy16
	arm_func_end __VENEER_MIi_CpuCopy16

	arm_func_start __VENEER_OS_CancelAlarm
__VENEER_OS_CancelAlarm: ; 0x06001B0C
	ldr pc, _06001B10 ; =OS_CancelAlarm
	.align 2, 0
_06001B10: .word OS_CancelAlarm
	arm_func_end __VENEER_OS_CancelAlarm

	arm_func_start __VENEER_OS_SetAlarm
__VENEER_OS_SetAlarm: ; 0x06001B14
	ldr pc, _06001B18 ; =OS_SetAlarm
	.align 2, 0
_06001B18: .word OS_SetAlarm
	arm_func_end __VENEER_OS_SetAlarm
