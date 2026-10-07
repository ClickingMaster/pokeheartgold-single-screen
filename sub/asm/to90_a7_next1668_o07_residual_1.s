	.include "asm/macros.inc"
	.include "global.inc"

	.extern FinishPlayer
	.extern GetPlayerTrack
	.extern PlayerSeqMain
	.extern SNDi_SharedWork
	.extern SNDi_Work
	.extern UpdateTrackChannel

.text

	arm_func_start SND_SeqMain
SND_SeqMain: ; 0x037FCEEC
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	mov r6, #0
	mov sb, r0
	mov r5, r6
_037FCEFC:
	mov r0, #0x24
	mul r0, r5, r0
	ldr r2, _037FD044 ; =SNDi_Work + 0x540
	ldrb r1, [r2, r0]
	add r4, r2, r0
	mov r0, r1, lsl #0x1f
	movs r0, r0, lsr #0x1f
	beq _037FD020
	mov r0, r1, lsl #0x1e
	movs r0, r0, lsr #0x1f
	beq _037FD00C
	cmp sb, #0
	beq _037FCFD8
	mov r0, r1, lsl #0x1d
	movs r0, r0, lsr #0x1f
	bne _037FCFD8
	mov r8, #0
	b _037FCF54
_037FCF44:
	ldrh r0, [r4, #0x1c]
	add r8, r8, #1
	sub r0, r0, #0xf0
	strh r0, [r4, #0x1c]
_037FCF54:
	ldrh r0, [r4, #0x1c]
	cmp r0, #0xf0
	bhs _037FCF44
	mov r7, #0
	mov sl, #1
	b _037FCF90
_037FCF6C:
	mov r0, r4
	mov r1, sl
	bl PlayerSeqMain
	cmp r0, #0
	beq _037FCF8C
	mov r0, r4
	bl FinishPlayer
	b _037FCF98
_037FCF8C:
	add r7, r7, #1
_037FCF90:
	cmp r7, r8
	blt _037FCF6C
_037FCF98:
	ldr r0, _037FD048 ; =SNDi_SharedWork
	ldr r2, [r0]
	cmp r2, #0
	beq _037FCFC0
	ldrb r1, [r4, #1]
	mov r0, #0x24
	mla r2, r1, r0, r2
	ldr r0, [r2, #0x40]
	add r0, r0, r7
	str r0, [r2, #0x40]
_037FCFC0:
	ldrh r2, [r4, #0x18]
	ldrh r1, [r4, #0x1a]
	ldrh r0, [r4, #0x1c]
	mul r1, r2, r1
	add r0, r0, r1, asr #8
	strh r0, [r4, #0x1c]
_037FCFD8:
	mov r8, #0
	mov r7, #1
_037FCFE0:
	mov r0, r4
	mov r1, r8
	bl GetPlayerTrack
	cmp r0, #0
	beq _037FD000
	mov r1, r4
	mov r2, r7
	bl UpdateTrackChannel
_037FD000:
	add r8, r8, #1
	cmp r8, #0x10
	blt _037FCFE0
_037FD00C:
	ldrb r0, [r4]
	mov r0, r0, lsl #0x1f
	movs r0, r0, lsr #0x1f
	movne r0, #1
	orrne r6, r6, r0, lsl r5
_037FD020:
	add r5, r5, #1
	cmp r5, #0x10
	blt _037FCEFC
	ldr r0, _037FD048 ; =SNDi_SharedWork
	ldr r0, [r0]
	cmp r0, #0
	strne r6, [r0, #4]
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bx lr
	.align 2, 0
_037FD044: .word SNDi_Work + 0x540
_037FD048: .word SNDi_SharedWork
	arm_func_end SND_SeqMain
