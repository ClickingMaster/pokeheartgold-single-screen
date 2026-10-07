	.include "asm/macros.inc"
	.include "global.inc"

	.extern CAM_GetStaState
	.extern CAM_SetPowerMgtMode
	.extern CAM_SetStaState
	.extern DeleteTxFrames
	.extern MLME_IssueDeAuthIndication
	.extern MakeDeAuthFrame
	.extern TxManCtrlFrame
	.extern WClearAids
	.extern WSetStaState

.text

	arm_func_start CAM_TimerTask
CAM_TimerTask: ; 0x0600EB20
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	ldr fp, _0600ECD0 ; =0x0380FFF4
	mov sb, #0
	ldr r2, [fp]
	mov r8, #1
	ldr r1, [r2, #0x31c]
	add r0, r2, #0x500
	ldrh sl, [r0, #0x2c]
	add r0, r2, #0x12c
	add r7, r1, #0x1c
	add r5, r0, #0x400
	add r4, r2, #0x300
	b _0600ECBC
_0600EB54:
	ldrh r0, [r7]
	cmp r0, #0
	beq _0600ECAC
	ldrh r1, [r7, #0x18]
	cmp r1, #0
	ldrne r0, _0600ECD4 ; =0x0000FFFF
	cmpne r1, r0
	beq _0600ECA8
	sub r0, r1, #1
	strh r0, [r7, #0x18]
	ldrh r0, [r7, #0x18]
	cmp r0, #0
	bne _0600ECA8
	ldrh r0, [r7]
	cmp r0, #0x20
	blo _0600EC94
	mov r0, r8
	bl CAM_GetStaState
	mov r6, r0
	mov r0, r8, lsl #0x10
	mov r0, r0, lsr #0x10
	mov r1, #0x20
	bl CAM_SetStaState
	mov r0, r8
	bl DeleteTxFrames
	ldrh r0, [r4, #0x50]
	cmp r0, #1
	bne _0600EC44
	cmp r6, #0x20
	bls _0600EC94
	ldr r1, [fp]
	mov r0, r8, lsl #0x10
	add r3, r1, #0x500
	ldrh r6, [r3, #0x34]
	mov r2, #1
	orr r2, r6, r2, lsl r8
	strh r2, [r3, #0x34]
	mov r0, r0, lsr #0x10
	mov r1, #0
	bl CAM_SetPowerMgtMode
	ldr r1, [fp]
	add r0, r7, #4
	add r3, r1, #0x500
	mov r1, #1
	ldrh r6, [r3, #0x30]
	mov r2, r1
	orr r2, r6, r2, lsl r8
	strh r2, [r3, #0x30]
	mov r2, #0
	bl MakeDeAuthFrame
	cmp r0, #0
	mov r1, #1
	beq _0600EC38
	strh r1, [r0]
	bl TxManCtrlFrame
	add sb, sb, #1
	b _0600ECB4
_0600EC38:
	add r0, r7, #4
	bl MLME_IssueDeAuthIndication
	b _0600EC94
_0600EC44:
	ldrh r0, [r4, #0xcc]
	cmp r8, r0
	bne _0600EC94
	add r0, r7, #4
	mov r1, #1
	mov r2, #0
	bl MakeDeAuthFrame
	cmp r0, #0
	beq _0600EC7C
	mov r1, #1
	strh r1, [r0]
	bl TxManCtrlFrame
	add sb, sb, #1
	b _0600ECB4
_0600EC7C:
	mov r0, #0x20
	bl WSetStaState
	bl WClearAids
	add r0, r7, #4
	mov r1, #1
	bl MLME_IssueDeAuthIndication
_0600EC94:
	mov r0, #0
	strh r0, [r7]
	ldrh r0, [r5]
	sub r0, r0, #1
	strh r0, [r5]
_0600ECA8:
	add sb, sb, #1
_0600ECAC:
	cmp sb, sl
	bhs _0600ECC8
_0600ECB4:
	add r8, r8, #1
	add r7, r7, #0x1c
_0600ECBC:
	ldrh r0, [r4, #0x22]
	cmp r8, r0
	blo _0600EB54
_0600ECC8:
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_0600ECD0: .word 0x0380FFF4
_0600ECD4: .word 0x0000FFFF
	arm_func_end CAM_TimerTask
