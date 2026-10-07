	.include "asm/macros.inc"
	.include "global.inc"

	.extern CTRDGi_ChangeLatestAccessCycle
	.extern CTRDGi_EnableFlag
	.extern CTRDGi_LockByProcessor
	.extern CTRDGi_UnlockByProcessor
	.extern MI_SetCartridgeRomCycle1st
	.extern MI_SetCartridgeRomCycle2nd
	.extern OS_RestoreInterrupts

.text

	arm_func_start CTRDG_IsExisting
CTRDG_IsExisting: ; 0x03802FD0
	stmdb sp!, {r4, lr}
	sub sp, sp, #0x10
	ldr r2, _038030E8 ; =0x027FFC30
	mov r4, #1
	ldrh r1, [r2]
	rsb r0, r4, #0x10000
	cmp r1, r0
	moveq r0, #0
	beq _038030DC
	ldrb r0, [r2, #5]
	mov r0, r0, lsl #0x1e
	mov r0, r0, lsr #0x1f
	cmp r0, #1
	moveq r0, #0
	beq _038030DC
	ldr r0, _038030EC ; =CTRDGi_EnableFlag
	add r1, sp, #8
	ldrh r0, [r0, #6]
	bl CTRDGi_LockByProcessor
	cmp r0, #0
	bne _03803034
	ldr r0, [sp, #0xc]
	bl OS_RestoreInterrupts
	mov r0, r4
	b _038030DC
_03803034:
	add r0, sp, #0
	bl CTRDGi_ChangeLatestAccessCycle
	mov r0, #0x8000000
	ldrb r2, [r0, #0xb2]
	cmp r2, #0x96
	bne _03803060
	ldr r1, _038030E8 ; =0x027FFC30
	ldrh r0, [r0, #0xbe]
	ldrh r1, [r1]
	cmp r1, r0
	bne _038030A4
_03803060:
	cmp r2, #0x96
	ldrne r1, _038030E8 ; =0x027FFC30
	ldrne r0, _038030F0 ; =0x0801FFFE
	ldrneh r1, [r1]
	ldrneh r0, [r0]
	cmpne r1, r0
	bne _038030A4
	ldr r2, _038030E8 ; =0x027FFC30
	mov r0, #0x8000000
	ldr r1, [r2, #8]
	ldr r0, [r0, #0xac]
	cmp r1, r0
	beq _038030B8
	ldrb r0, [r2, #5]
	mov r0, r0, lsl #0x1f
	movs r0, r0, lsr #0x1f
	beq _038030B8
_038030A4:
	ldr r1, _038030E8 ; =0x027FFC30
	mov r4, #0
	ldrb r0, [r1, #5]
	orr r0, r0, #2
	strb r0, [r1, #5]
_038030B8:
	ldr r0, [sp]
	bl MI_SetCartridgeRomCycle1st
	ldr r0, [sp, #4]
	bl MI_SetCartridgeRomCycle2nd
	ldr r0, _038030EC ; =CTRDGi_EnableFlag
	add r1, sp, #8
	ldrh r0, [r0, #6]
	bl CTRDGi_UnlockByProcessor
	mov r0, r4
_038030DC:
	add sp, sp, #0x10
	ldmia sp!, {r4, lr}
	bx lr
	.align 2, 0
_038030E8: .word 0x027FFC30
_038030EC: .word CTRDGi_EnableFlag
_038030F0: .word 0x0801FFFE
	arm_func_end CTRDG_IsExisting
