	.include "asm/macros.inc"
	.include "global.inc"

	.extern CARD_GetRomHeader
	.extern CARD_IsPulledOut
	.extern PXI_SendWordByFifo
	.extern VENEER_SVC_WaitByLoop
	.extern nextCount$3668
	.extern skipCheck$3668

.text

	arm_func_start CARD_CheckPullOut_Polling
CARD_CheckPullOut_Polling: ; 0x038007AC
	stmdb sp!, {r3, r4, r5, r6, r7, lr}
	ldr r0, _03800898 ; =skipCheck$3668
	ldr r0, [r0, #4]
	cmp r0, #0
	bne _03800890
	ldr r2, _0380089C ; =0x027FFC40
	ldrh r0, [r2]
	cmp r0, #2
	beq _03800890
	ldr r1, _038008A0 ; =nextCount$3668
	mvn r0, #0
	ldr r3, [r1]
	cmp r3, r0
	ldreq r0, [r2, #-4]
	addeq r0, r0, #0xa
	streq r0, [r1]
	beq _03800890
	ldr r0, [r2, #-4]!
	cmp r0, r3
	blo _03800890
	ldr r0, [r2]
	add r0, r0, #0xa
	str r0, [r1]
	bl CARD_IsPulledOut
	cmp r0, #0
	beq _03800840
	ldr r0, _03800898 ; =skipCheck$3668
	mov r1, #1
	str r1, [r0, #4]
	bl CARD_GetRomHeader
	ldr r0, [r0, #0xc]
	cmp r0, #0
	bne _03800840
	ldr r0, _038008A0 ; =nextCount$3668
	ldr r0, [r0, #4]
	cmp r0, #0
	bne _03800890
_03800840:
	ldr r0, _03800898 ; =skipCheck$3668
	ldr r1, _038008A0 ; =nextCount$3668
	ldr r0, [r0, #4]
	mov r2, #0
	str r2, [r1, #4]
	cmp r0, #0
	beq _03800890
	mov r7, #0x64
	mov r6, #0xe
	mov r5, #0x11
	mov r4, r2
	b _03800878
_03800870:
	mov r0, r7
	bl VENEER_SVC_WaitByLoop
_03800878:
	mov r0, r6
	mov r1, r5
	mov r2, r4
	bl PXI_SendWordByFifo
	cmp r0, #0
	bne _03800870
_03800890:
	ldmia sp!, {r3, r4, r5, r6, r7, lr}
	bx lr
	.align 2, 0
_03800898: .word skipCheck$3668
_0380089C: .word 0x027FFC40
_038008A0: .word nextCount$3668
	arm_func_end CARD_CheckPullOut_Polling
