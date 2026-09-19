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

	thumb_func_start ov12_0225E250
ov12_0225E250: ; 0x0225E250
	push {r4, r5, r6, r7, lr}
	sub sp, #0x4c
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetBgConfig
	ldr r0, [r4]
	bl BattleSystem_GetBattleInput
	add r7, r0, #0
	ldrb r1, [r4, #0x1d]
	ldr r0, [r4]
	bl BattleSystem_GetOpponentData
	str r0, [sp, #0x10]
	ldrb r1, [r4, #0x1d]
	ldr r0, [r4]
	bl BattleSystem_GetBattlerIdPartner
	add r1, r0, #0
	ldrb r0, [r4, #0x1d]
	cmp r1, r0
	beq _0225E28A
	ldr r0, [r4]
	bl BattleSystem_GetHpBar
	add r5, r0, #0
	b _0225E28C
_0225E28A:
	mov r5, #0
_0225E28C:
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	cmp r0, #3
	bls _0225E298
	b _0225E3D0
_0225E298:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225E2A4: ; jump table
	.short _0225E2AC - _0225E2A4 - 2 ; case 0
	.short _0225E350 - _0225E2A4 - 2 ; case 1
	.short _0225E376 - _0225E2A4 - 2 ; case 2
	.short _0225E3C2 - _0225E2A4 - 2 ; case 3
_0225E2AC:
	add r0, r7, #0
	bl BattleInput_CheckFeedbackDone
	cmp r0, #0
	bne _0225E2B8
	b _0225E3F8
_0225E2B8:
	ldr r0, [r4]
	bl BattleSystem_GetMessageLoader
	add r1, r0, #0
	mov r2, #2
	add r0, sp, #0x14
	strb r2, [r0, #0x15]
	ldrb r2, [r4, #0x1f]
	ldrb r3, [r4, #0x1d]
	lsl r2, r2, #8
	orr r2, r3
	str r2, [sp, #0x2c]
	ldr r2, _0225E3FC ; =0x00000399
	mov r3, #0
	strh r2, [r0, #0x16]
	ldr r0, [r4]
	add r2, sp, #0x28
	bl BattleSystem_PrintBattleMessage
	add r0, r7, #0
	bl BattleInput_EnableBallGauge
	mov r0, #7
	mov r1, #5
	bl NARC_New
	str r0, [sp, #0xc]
	mov r0, #8
	mov r1, #5
	bl NARC_New
	add r5, sp, #0x14
	str r0, [sp, #8]
	mov r2, #0
	add r3, r4, #0
	add r6, r5, #0
_0225E300:
	ldrh r0, [r3, #0xc]
	add r1, r4, r2
	add r2, r2, #1
	strh r0, [r5]
	ldrb r0, [r1, #0x14]
	add r3, r3, #2
	add r5, r5, #2
	strb r0, [r6, #8]
	ldrb r0, [r1, #0x18]
	strb r0, [r6, #0xc]
	add r6, r6, #1
	cmp r2, #4
	blt _0225E300
	ldrb r1, [r4, #0x1e]
	add r0, sp, #0x14
	add r2, r7, #0
	strb r1, [r0, #0x10]
	mov r0, #0
	str r0, [sp]
	add r0, sp, #0x14
	str r0, [sp, #4]
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #8]
	mov r3, #0xb
	bl BattleInput_ChangeMenu
	ldr r0, [sp, #0xc]
	bl NARC_Delete
	ldr r0, [sp, #8]
	bl NARC_Delete
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	add r4, #0x20
	add sp, #0x4c
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, r7, pc}
_0225E350:
	add r0, r7, #0
	bl BattleInput_CheckTouch
	mov r1, #0
	mvn r1, r1
	str r0, [r4, #8]
	cmp r0, r1
	beq _0225E3F8
	ldr r0, _0225E400 ; =0x000005DD
	bl PlaySE
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	add r4, #0x20
	add sp, #0x4c
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, r7, pc}
_0225E376:
	ldr r0, [r4, #8]
	cmp r0, #0xff
	beq _0225E3A8
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #2
	tst r0, r1
	bne _0225E3A2
	add r0, r7, #0
	mov r1, #0
	bl BattleInput_Deadstriped_022698AC
	add r0, r5, #0
	bl ov12_02265D74
	ldr r0, [r4, #4]
	bl ov12_02264EB4
	ldr r0, [sp, #0x10]
	bl ov12_02262014
_0225E3A2:
	add r0, r7, #0
	bl BattleInput_DisableBallGauge
_0225E3A8:
	ldrb r1, [r4, #0x1d]
	ldr r0, [r4]
	ldr r2, [r4, #8]
	bl ov12_02262FE0
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	add r4, #0x20
	add sp, #0x4c
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, r7, pc}
_0225E3C2:
	add r0, r4, #0
	add r0, #0x20
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x20
	strb r1, [r0]
_0225E3D0:
	add r0, r7, #0
	bl ov12_022698B0
	cmp r0, #1
	bne _0225E3F8
	ldr r0, [r4]
	mov r1, #1
	bl ov12_0223BB10
	ldrb r1, [r4, #0x1d]
	ldrb r2, [r4, #0x1c]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225E3F8:
	add sp, #0x4c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0225E3FC: .word 0x00000399
_0225E400: .word 0x000005DD
	thumb_func_end ov12_0225E250

	thumb_func_start ov12_0225E404
ov12_0225E404: ; 0x0225E404
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x20
	add r6, r1, #0
	str r0, [sp, #8]
	ldr r0, [r6]
	bl BattleSystem_GetBattleType
	add r4, r0, #0
	ldr r0, [r6]
	bl BattleSystem_GetBattleContext
	str r0, [sp, #0xc]
	ldr r0, _0225E4C8 ; =0x00000101
	tst r0, r4
	bne _0225E43A
	ldr r0, [r6]
	bl BattleSystem_GetBattleSpecial
	mov r1, #1
	tst r0, r1
	bne _0225E43A
	ldrb r1, [r6, #0x1d]
	ldr r0, [r6]
	bl BattleSystem_GetFieldSide
	cmp r0, #0
	bne _0225E44A
_0225E43A:
	ldrb r1, [r6, #0x1d]
	ldr r0, [r6]
	bl ov10_0221BEF4
	cmp r0, #0xff
	beq _0225E4C2
	add r4, r0, #1
	b _0225E4A2
_0225E44A:
	mov r7, #0
	add r4, r7, #0
	add r5, sp, #0x10
_0225E450:
	add r0, r4, #0
	bl MaskOfFlagNo
	ldrh r1, [r6, #0x22]
	tst r0, r1
	bne _0225E462
	add r0, r4, #1
	stmia r5!, {r0}
	add r7, r7, #1
_0225E462:
	add r4, r4, #1
	cmp r4, #4
	blt _0225E450
	ldr r0, [r6]
	bl BattleSystem_Random
	add r1, r7, #0
	bl _s32_div_f
	lsl r1, r1, #2
	add r0, sp, #0x10
	ldr r4, [r0, r1]
	mov r0, #1
	sub r3, r4, #1
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	lsl r3, r3, #1
	add r3, r6, r3
	ldrb r2, [r6, #0x1d]
	ldrh r3, [r3, #0xc]
	ldr r0, [r6]
	ldr r1, [sp, #0xc]
	bl ov12_022506D4
	str r0, [sp]
	ldrb r3, [r6, #0x1d]
	ldr r0, [r6]
	ldr r1, [sp, #0xc]
	mov r2, #0xb
	bl ov12_022582B8
_0225E4A2:
	ldrb r1, [r6, #0x1d]
	ldr r0, [r6]
	add r2, r4, #0
	bl ov12_02262FE0
	ldrb r1, [r6, #0x1d]
	ldrb r2, [r6, #0x1c]
	ldr r0, [r6]
	bl ov12_0226430C
	add r0, r6, #0
	bl Heap_Free
	ldr r0, [sp, #8]
	bl SysTask_Destroy
_0225E4C2:
	add sp, #0x20
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225E4C8: .word 0x00000101
	thumb_func_end ov12_0225E404
