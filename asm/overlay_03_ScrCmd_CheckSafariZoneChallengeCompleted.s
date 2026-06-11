	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

	thumb_func_start ScrCmd_CheckSafariZoneChallengeCompleted
ScrCmd_CheckSafariZoneChallengeCompleted: ; 0x02258C00
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	ldr r2, _02258CD4 ; =ov03_02259808
	add r1, sp, #0x14
	ldrh r3, [r2]
	ldrh r2, [r2, #2]
	add r5, r0, #0
	strh r3, [r1]
	strh r2, [r1, #2]
	add r1, r5, #0
	add r1, #0x80
	ldr r2, [r5, #8]
	ldr r4, [r1]
	add r1, r2, #1
	str r1, [r5, #8]
	ldrb r1, [r2]
	str r1, [sp, #8]
	bl ScriptReadHalfword
	add r5, #0x80
	add r1, r0, #0
	ldr r0, [r5]
	bl GetVarPointer
	str r0, [sp]
	ldr r0, [r4, #0xc]
	bl SaveArray_Party_Get
	str r0, [sp, #4]
	bl Party_GetCount
	add r6, r0, #0
	ldr r0, [r4, #0xc]
	bl Save_PlayerData_GetProfile
	bl PlayerProfile_GetTrainerID
	str r0, [sp, #0xc]
	ldr r0, _02258CD8 ; =0x00000165
	bl MapHeader_GetMapSec
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r5, #0
	cmp r6, #0
	ble _02258CCA
	ldr r0, [sp, #8]
	lsl r0, r0, #1
	str r0, [sp, #0x10]
_02258C62:
	ldr r0, [sp, #4]
	add r1, r5, #0
	bl Party_GetMonByIndex
	mov r1, #0x4c
	mov r2, #0
	add r4, r0, #0
	bl GetMonData
	cmp r0, #0
	bne _02258CC4
	add r0, r4, #0
	mov r1, #7
	mov r2, #0
	bl GetMonData
	ldr r1, [sp, #0xc]
	cmp r1, r0
	bne _02258CC4
	add r0, r4, #0
	mov r1, #5
	mov r2, #0
	bl GetMonData
	ldr r1, [sp, #0x10]
	add r2, sp, #0x14
	ldrh r1, [r2, r1]
	cmp r1, r0
	bne _02258CC4
	add r0, r4, #0
	mov r1, #0x98
	mov r2, #0
	bl GetMonData
	cmp r0, #0
	bne _02258CC4
	add r0, r4, #0
	mov r1, #0x99
	mov r2, #0
	bl GetMonData
	cmp r7, r0
	bne _02258CC4
	ldr r0, [sp]
	mov r1, #1
	strh r1, [r0]
	add sp, #0x18
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_02258CC4:
	add r5, r5, #1
	cmp r5, r6
	blt _02258C62
_02258CCA:
	ldr r1, [sp]
	mov r0, #0
	strh r0, [r1]
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02258CD4: .word ov03_02259808
_02258CD8: .word 0x00000165
	thumb_func_end ScrCmd_CheckSafariZoneChallengeCompleted