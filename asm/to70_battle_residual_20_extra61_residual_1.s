	.extern ov12_02261EB8
	.extern ov12_02261ED4
	.include "asm/macros.inc"
	.include "overlay_12_battle_controller_opponent.inc"
	.include "global.inc"

.text
	thumb_func_start ov12_0225FA44
ov12_0225FA44: ; 0x0225FA44
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x38
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetBgConfig
	ldr r0, [r4]
	bl BattleSystem_GetBattleInput
	add r5, r0, #0
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetOpponentData
	str r0, [sp, #0xc]
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetBattlerIdPartner
	add r1, r0, #0
	ldrb r0, [r4, #0xd]
	cmp r1, r0
	beq _0225FA7E
	ldr r0, [r4]
	bl BattleSystem_GetHpBar
	add r7, r0, #0
	b _0225FA80
_0225FA7E:
	mov r7, #0
_0225FA80:
	ldrb r0, [r4, #0xe]
	cmp r0, #4
	bhi _0225FB00
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225FA92: ; jump table
	.short _0225FA9C - _0225FA92 - 2 ; case 0
	.short _0225FAEC - _0225FA92 - 2 ; case 1
	.short _0225FBC6 - _0225FA92 - 2 ; case 2
	.short _0225FBE4 - _0225FA92 - 2 ; case 3
	.short _0225FC4C - _0225FA92 - 2 ; case 4
_0225FA9C:
	add r0, r5, #0
	bl BattleInput_CheckFeedbackDone
	cmp r0, #0
	beq _0225FB00
	ldr r0, [r4, #0x10]
	cmp r0, #0
	beq _0225FAE4
	ldr r0, [r4]
	bl BattleSystem_GetMessageLoader
	add r5, r0, #0
	ldrb r0, [r4, #0xf]
	cmp r0, #5
	add r0, sp, #0x10
	bne _0225FAC6
	mov r1, #0x82
	strb r1, [r0, #5]
	ldr r0, [r4, #0x14]
	str r0, [sp, #0x18]
	b _0225FACA
_0225FAC6:
	mov r1, #0
	strb r1, [r0, #5]
_0225FACA:
	ldr r1, [r4, #0x10]
	add r0, sp, #0x10
	strh r1, [r0, #6]
	ldr r0, [r4]
	bl BattleSystem_GetTextFrameDelay
	add r3, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r2, sp, #0x14
	bl BattleSystem_PrintBattleMessage
	strh r0, [r4, #0x1a]
_0225FAE4:
	mov r0, #1
	add sp, #0x38
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225FAEC:
	ldrh r0, [r4, #0x1a]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	beq _0225FB02
	ldr r0, [r4, #0x10]
	cmp r0, #0
	beq _0225FB02
_0225FB00:
	b _0225FC76
_0225FB02:
	mov r0, #7
	mov r1, #5
	bl NARC_New
	add r7, r0, #0
	mov r0, #8
	mov r1, #5
	bl NARC_New
	add r6, r0, #0
	add r0, r5, #0
	bl BattleInput_DisableBallGauge
	ldrh r1, [r4, #0x18]
	add r0, sp, #0x10
	strh r1, [r0]
	ldrb r0, [r4, #0xf]
	cmp r0, #5
	bhi _0225FBAE
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225FB34: ; jump table
	.short _0225FB40 - _0225FB34 - 2 ; case 0
	.short _0225FB56 - _0225FB34 - 2 ; case 1
	.short _0225FB6C - _0225FB34 - 2 ; case 2
	.short _0225FB82 - _0225FB34 - 2 ; case 3
	.short _0225FB98 - _0225FB34 - 2 ; case 4
	.short _0225FB40 - _0225FB34 - 2 ; case 5
_0225FB40:
	mov r0, #0
	str r0, [sp]
	add r0, sp, #0x10
	str r0, [sp, #4]
	add r0, r7, #0
	add r1, r6, #0
	add r2, r5, #0
	mov r3, #0xd
	bl BattleInput_ChangeMenu
	b _0225FBB2
_0225FB56:
	mov r0, #0
	str r0, [sp]
	add r0, sp, #0x10
	str r0, [sp, #4]
	add r0, r7, #0
	add r1, r6, #0
	add r2, r5, #0
	mov r3, #0xe
	bl BattleInput_ChangeMenu
	b _0225FBB2
_0225FB6C:
	mov r0, #0
	str r0, [sp]
	add r0, sp, #0x10
	str r0, [sp, #4]
	add r0, r7, #0
	add r1, r6, #0
	add r2, r5, #0
	mov r3, #0xf
	bl BattleInput_ChangeMenu
	b _0225FBB2
_0225FB82:
	mov r0, #0
	str r0, [sp]
	add r0, sp, #0x10
	str r0, [sp, #4]
	add r0, r7, #0
	add r1, r6, #0
	add r2, r5, #0
	mov r3, #0x10
	bl BattleInput_ChangeMenu
	b _0225FBB2
_0225FB98:
	mov r0, #0
	str r0, [sp]
	add r0, sp, #0x10
	str r0, [sp, #4]
	add r0, r7, #0
	add r1, r6, #0
	add r2, r5, #0
	mov r3, #0x11
	bl BattleInput_ChangeMenu
	b _0225FBB2
_0225FBAE:
	bl GF_AssertFail
_0225FBB2:
	mov r0, #2
	strb r0, [r4, #0xe]
	add r0, r7, #0
	bl NARC_Delete
	add r0, r6, #0
	bl NARC_Delete
	add sp, #0x38
	pop {r3, r4, r5, r6, r7, pc}
_0225FBC6:
	add r0, r5, #0
	bl BattleInput_CheckTouch
	mov r1, #0
	mvn r1, r1
	str r0, [r4, #8]
	cmp r0, r1
	beq _0225FC76
	ldr r0, _0225FC7C ; =0x000005DD
	bl PlaySE
	mov r0, #3
	add sp, #0x38
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225FBE4:
	add r0, r5, #0
	bl BattleInput_CheckFeedbackDone
	cmp r0, #1
	bne _0225FC76
	mov r0, #7
	mov r1, #5
	bl NARC_New
	str r0, [sp, #8]
	mov r0, #8
	mov r1, #5
	bl NARC_New
	add r6, r0, #0
	ldr r0, [r4, #4]
	bl ov12_02264EB4
	ldr r0, [sp, #0xc]
	bl ov12_02262014
	add r0, r7, #0
	bl ov12_02265D74
	add r0, r5, #0
	bl BattleInput_DisableBallGauge
	mov r3, #0
	str r3, [sp]
	ldr r0, [sp, #8]
	str r3, [sp, #4]
	add r1, r6, #0
	add r2, r5, #0
	bl BattleInput_ChangeMenu
	ldr r0, [r4, #8]
	cmp r0, #1
	bne _0225FC38
	add r0, r5, #0
	mov r1, #0
	bl BattleInput_Deadstriped_022698AC
_0225FC38:
	mov r0, #4
	strb r0, [r4, #0xe]
	ldr r0, [sp, #8]
	bl NARC_Delete
	add r0, r6, #0
	bl NARC_Delete
	add sp, #0x38
	pop {r3, r4, r5, r6, r7, pc}
_0225FC4C:
	add r0, r5, #0
	bl ov12_022698B0
	cmp r0, #1
	bne _0225FC76
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	ldr r2, [r4, #8]
	bl ov12_02262F24
	ldrb r1, [r4, #0xd]
	ldrb r2, [r4, #0xc]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225FC76:
	add sp, #0x38
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225FC7C: .word 0x000005DD
	thumb_func_end ov12_0225FA44
