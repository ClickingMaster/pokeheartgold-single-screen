#include "constants/pokemon.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_12_battle_controller_opponent.inc"
	.include "global.inc"
	.public ov12_02258BA0

	.text
	.public ov12_02258800
	.public ov12_02258BA0
	.public ov12_02258BB4
	.public ov12_02258D74
	.public ov12_02258DB0
	.public ov12_02258E54
	.public ov12_02258E7C
	.public ov12_02258EB0
	.public ov12_02258EB4
	.public ov12_02258EE0
	.public ov12_02258EF4
	.public ov12_02258F08
	.public ov12_02258F1C
	.public ov12_02258F30
	.public ov12_02258F44
	.public ov12_02258F68
	.public ov12_02258F7C
	.public ov12_02258F90
	.public ov12_02258FA0
	.public ov12_02258FB4
	.public ov12_02258FC8
	.public ov12_02258FD8
	.public ov12_02259000
	.public ov12_02259014
	.public ov12_02259028
	.public ov12_0225903C
	.public ov12_02259050
	.public ov12_02259064
	.public ov12_02259078
	.public ov12_0225908C
	.public ov12_022590A0
	.public ov12_022590D4
	.public ov12_022590E8
	.public ov12_022590FC
	.public ov12_02259110
	.public ov12_02259124
	.public ov12_02259134
	.public ov12_02259148
	.public ov12_0225915C
	.public ov12_02259170
	.public ov12_02259184
	.public ov12_02259198
	.public ov12_022591A8
	.public ov12_022591BC
	.public ov12_022591CC
	.public ov12_022591E0
	.public ov12_022591F4
	.public ov12_022592D0
	.public ov12_02259328
	.public ov12_02259358
	.public ov12_022593D4
	.public ov12_022593E8
	.public ov12_022593FC
	.public ov12_022594F4
	.public ov12_02259514
	.public ov12_022595B8
	.public ov12_022595CC
	.public ov12_022595E0
	.public ov12_0225961C
	.public ov12_02259658
	.public ov12_02259694
	.public ov12_022596B8
	.public ov12_02259700
	.public ov12_02259724
	.public ov12_02259738
	.public ov12_02259748
	.public ov12_02259758
	.public ov12_02259768
	.public ov12_0225978C
	.public ov12_022597B0
	.public ov12_022597C4
	.public ov12_022597D8
	.public ov12_022597EC
	.public ov12_022598F8
	.public ov12_02259928
	.public ov12_02259930
	.public ov12_02259944
	.public ov12_02259968
	.public ov12_02259BA8
	.public ov12_02259D48
	.public ov12_02259F30
	.public ov12_0225A018
	.public ov12_0225A07C
	.public ov12_0225A2A0
	.public ov12_0225A334
	.public ov12_0225A37C
	.public ov12_0225A414
	.public ov12_0225A4DC
	.public ov12_0225A524
	.public ov12_0225A604
	.public ov12_0225A674
	.public ov12_0225A700
	.public ov12_0225A7AC
	.public ov12_0225A818
	.public ov12_0225A85C
	.public ov12_0225A8C4
	.public ov12_0225A914
	.public ov12_0225A9B0
	.public ov12_0225A9E0
	.public ov12_0225AA6C
	.public ov12_0225AAE0
	.public ov12_0225ABB8
	.public ov12_0225ABE8
	.public ov12_0225AC1C
	.public ov12_0225ACB0
	.public ov12_0225ACE8
	.public ov12_0225AD44
	.public ov12_0225AD9C
	.public ov12_0225ADF4
	.public ov12_0225AE48
	.public ov12_0225AEA0
	.public ov12_0225AED8
	.public ov12_0225AF74
	.public ov12_0225B028
	.public ov12_0225B060
	.public ov12_0225B0A0
	.public ov12_0225B0E8
	.public ov12_0225B120
	.public ov12_0225B16C
	.public ov12_0225B1A8
	.public ov12_0225B200
	.public ov12_0225B234
	.public ov12_0225B26C
	.public ov12_0225B2A4
	.public ov12_0225B2F8
	.public ov12_0225B34C
	.public ov12_0225B3A0
	.public ov12_0225B3D4
	.public ov12_0225B434
	.public ov12_0225B454
	.public ov12_0225B494
	.public ov12_0225B7B8
	.public ov12_0225B960
	.public ov12_0225BE38
	.public ov12_0225C18C
	.public ov12_0225C6C8
	.public ov12_0225C9BC
	.public ov12_0225CC58
	.public ov12_0225CDB8
	.public ov12_0225CE28
	.public ov12_0225D138
	.public ov12_0225D644
	.public ov12_0225D890
	.public ov12_0225D990
	.public ov12_0225DA18
	.public ov12_0225DA8C
	.public ov12_0225DAD4
	.public ov12_0225E104
	.public ov12_0225E134
	.public ov12_0225E154
	.public ov12_0225E1D4
	.public ov12_0225E1FC
	.public ov12_0225E250
	.public ov12_0225E404
	.public ov12_0225E4CC
	.public ov12_0225E4EC
	.public ov12_0225E568
	.public ov12_0225E6FC
	.public ov12_0225E740
	.public ov12_0225E760
	.public ov12_0225E830
	.public ov12_0225F3A4
	.public ov12_0225F3FC
	.public ov12_0225F434
	.public ov12_0225F4E0
	.public ov12_0225F8AC
	.public ov12_0225F960
	.public ov12_0225F980
	.public ov12_0225FA44
	.public ov12_0225FC80
	.public ov12_0225FCA0
	.public ov12_0225FCC0
	.public ov12_0225FD14
	.public ov12_0225FF80
	.public ov12_0225FFDC
	.public ov12_02260030
	.public ov12_022600F0
	.public ov12_0226037C
	.public ov12_02260418
	.public ov12_02260584
	.public ov12_022605D0
	.public ov12_02260614
	.public ov12_02260668
	.public ov12_022609F8
	.public ov12_02260B30
	.public ov12_02260BA0
	.public ov12_02260C58
	.public ov12_02260CDC
	.public ov12_02260D28
	.public ov12_02260D84
	.public ov12_02260EA4
	.public ov12_02261258
	.public ov12_02261264
	.public ov12_02261270
	.public ov12_02261280
	.public ov12_02261284
	.public ov12_02261294
	.public ov12_022612A4
	.public ov12_02261390
	.public ov12_02261464
	.public ov12_02261544
	.public ov12_022615F0
	.public ov12_02261928
	.public ov12_022619E4
	.public ov12_02261AD4
	.public ov12_02261B2C
	.public ov12_02261B80
	.public ov12_02261CA8
	.public ov12_02261D30
	.public ov12_02261DC8
	.public ov12_02261E40
	.public ov12_02261EB8
	.public ov12_02261ED4
	.public ov12_02261EF0
	.public ov12_02261F38
	.public ov12_0226D010
	.public ov12_0226D120
	.public ov12_0226D128
	.public ov12_0226D140
	.public ov12_0226D141
	.public ov12_0226D15A
	.public ov12_0226D18C
	.public ov12_0226D1E8
	.public ov12_0226D1EA
	.public ov12_0226D350

	thumb_func_start ov12_0225F980
ov12_0225F980: ; 0x0225F980
	push {r3, r4, r5, r6, lr}
	sub sp, #4
	add r4, r1, #0
	ldrb r1, [r4, #9]
	add r6, r0, #0
	ldr r0, [r4]
	add r2, sp, #0
	bl ov12_0223BE0C
	cmp r0, #1
	bne _0225F99C
	ldr r0, [r4]
	bl ov12_02261ED4
_0225F99C:
	add r0, sp, #0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0225F9A8
	cmp r0, #6
	bls _0225F9B0
_0225F9A8:
	ldr r0, [r4]
	bl ov12_02261EB8
	b _0225FA18
_0225F9B0:
	ldrb r1, [r4, #9]
	sub r5, r0, #1
	ldr r0, [r4]
	bl BattleSystem_GetParty
	ldrb r1, [r4, #9]
	add r1, r4, r1
	ldrb r1, [r1, #0xc]
	cmp r5, r1
	beq _0225F9CA
	ldrb r1, [r4, #0x16]
	cmp r5, r1
	bne _0225F9D2
_0225F9CA:
	ldr r0, [r4]
	bl ov12_02261EB8
	b _0225FA18
_0225F9D2:
	add r1, sp, #0
	ldrb r5, [r1]
	bl Party_GetCount
	cmp r5, r0
	ldr r0, [r4]
	ble _0225F9E6
	bl ov12_02261EB8
	b _0225FA18
_0225F9E6:
	ldrb r1, [r4, #9]
	sub r2, r5, #1
	bl BattleSystem_GetPartyMon
	mov r1, #0xa3
	mov r2, #0
	add r5, r0, #0
	bl GetMonData
	cmp r0, #0
	bne _0225FA02
	ldr r0, [r4]
	bl ov12_02261EB8
_0225FA02:
	add r0, r5, #0
	mov r1, #0xae
	mov r2, #0
	bl GetMonData
	ldr r1, _0225FA40 ; =0x000001EE
	cmp r0, r1
	bne _0225FA18
	ldr r0, [r4]
	bl ov12_02261EB8
_0225FA18:
	add r2, sp, #0
	ldrb r1, [r4, #9]
	ldrb r2, [r2]
	ldr r0, [r4]
	bl ov12_02263360
	ldrb r1, [r4, #9]
	ldrb r2, [r4, #8]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
	add sp, #4
	pop {r3, r4, r5, r6, pc}
	nop
_0225FA40: .word 0x000001EE
	thumb_func_end ov12_0225F980

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
