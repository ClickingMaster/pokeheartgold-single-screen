	.include "asm/macros.inc"
	.include "global.inc"

	.extern CTRDG_IsExisting
	.extern CTRDG_IsPulledOut
	.extern OS_Sleep
	.extern PXI_SendWordByFifo
	.extern isFirstCheck$3769
	.extern lock_id

.text

	arm_func_start CTRDG_CheckPullOut_Polling
CTRDG_CheckPullOut_Polling: ; 0x038036E8
	stmdb sp!, {r3, r4, r5, r6, r7, lr}
	ldr r1, _038037D8 ; =isFirstCheck$3769
	mvn r0, #0
	ldr r3, [r1, #4]
	cmp r3, r0
	ldreq r0, _038037DC ; =0x027FFC3C
	ldreq r0, [r0]
	addeq r0, r0, #0xa
	streq r0, [r1, #4]
	beq _038037D0
	ldr r0, _038037E0 ; =lock_id
	ldr r2, [r0, #0x10]
	cmp r2, #0
	ldreq r0, [r0, #0xc]
	cmpeq r0, #0
	bne _038037D0
	ldr r2, _038037DC ; =0x027FFC3C
	ldr r0, [r2]
	cmp r0, r3
	blo _038037D0
	ldr r0, [r2]
	add r0, r0, #0xa
	str r0, [r1, #4]
	bl CTRDG_IsPulledOut
	ldr r1, _038037E0 ; =lock_id
	str r0, [r1, #0xc]
	bl CTRDG_IsExisting
	cmp r0, #0
	bne _03803780
	ldr r0, _038037D8 ; =isFirstCheck$3769
	mov r1, #1
	ldr r0, [r0]
	cmp r0, #0
	ldrne r0, _038037E0 ; =lock_id
	strne r1, [r0, #0x10]
	bne _038037D0
	ldr r0, _038037E0 ; =lock_id
	str r1, [r0, #0xc]
_03803780:
	ldr r0, _038037E0 ; =lock_id
	ldr r1, _038037D8 ; =isFirstCheck$3769
	ldr r0, [r0, #0xc]
	mov r2, #0
	str r2, [r1]
	cmp r0, #0
	beq _038037D0
	mov r7, #0x64
	mov r6, #0xd
	mov r5, #0x11
	mov r4, r2
	b _038037B8
_038037B0:
	mov r0, r7
	bl OS_Sleep
_038037B8:
	mov r0, r6
	mov r1, r5
	mov r2, r4
	bl PXI_SendWordByFifo
	cmp r0, #0
	bne _038037B0
_038037D0:
	ldmia sp!, {r3, r4, r5, r6, r7, lr}
	bx lr
	.align 2, 0
_038037D8: .word isFirstCheck$3769
_038037DC: .word 0x027FFC3C
_038037E0: .word lock_id
	arm_func_end CTRDG_CheckPullOut_Polling
